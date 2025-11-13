#ifndef CLIENT_HPP
#define CLIENT_HPP

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <sstream>

#include <QString>
#include <QDateTime>

class Message {
public:
    std::string Getter;
    std::string Sender;
    std::string Text;
    std::string Tag;

    Message() = default;
    Message(const std::string& g, const std::string& s, const std::string& t, const std::string& tg)
        : Getter(g), Sender(s), Text(t), Tag(tg) {}

    std::string getData() const {
        return Sender + ";" + Getter + ";" + Text + ";" + Tag;
    }

    static Message getMessage(const std::string& data) {
        std::istringstream ss(data);
        std::string parts[4];
        for (int i = 0; i < 4 && std::getline(ss, parts[i], ';'); ++i);
        if (!parts[0].empty() && !parts[1].empty() && !parts[2].empty() && !parts[3].empty())
            return Message(parts[1], parts[0], parts[2], parts[3]);
        return Message();
    }

    QString toQString() const {
        QString priorityColor;
        QString priorityText;

        if (Tag == "High" || Tag == "Maximum") {
            priorityColor = "red";
            priorityText = "Maximum";
        } else if (Tag == "Medium") {
            priorityColor = "orange";
            priorityText = "Medium";
        } else {
            priorityColor = "green";
            priorityText = "Minimum";
        }

        QString receiver = (Getter == "ALL") ? "Everyone" : QString::fromStdString(Getter);

        return QString("<b>From:</b> %1 | <b>To:</b> %2 | <b style='color:%3;'>Priority:</b> %4<br><b>Message:</b> %5<br>---<br>")
            .arg(QString::fromStdString(Sender))
            .arg(receiver)
            .arg(priorityColor)
            .arg(priorityText)
            .arg(QString::fromStdString(Text));
    }
};

class ChatClient {
private:
    SOCKET clientSocket;
    std::string serverIP;
    unsigned short serverPort;
    std::string currentUser;
    std::atomic<bool> connected;
    std::thread receiveThread;
    std::vector<std::string> onlineUsers;
    std::vector<Message> receivedMessages;

public:
    ChatClient() : clientSocket(INVALID_SOCKET), connected(false) {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    }

    ~ChatClient() {
        disconnect();
        WSACleanup();
    }

    bool connectToServer(const std::string& ip, unsigned short port) {
        serverIP = ip;
        serverPort = port;

        clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (clientSocket == INVALID_SOCKET) {
            return false;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

        if (::connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            closesocket(clientSocket);
            return false;
        }

        connected = true;
        receiveThread = std::thread(&ChatClient::receiveMessages, this);

        return true;
    }

    void disconnect() {
        connected = false;
        if (clientSocket != INVALID_SOCKET) {
            closesocket(clientSocket);
            clientSocket = INVALID_SOCKET;
        }
        if (receiveThread.joinable()) {
            receiveThread.join();
        }
    }

    bool login(const std::string& username, const std::string& password) {
        std::string message = "LOGIN:" + username + ":" + password;
        if (send(clientSocket, message.c_str(), message.length(), 0) == SOCKET_ERROR) {
            return false;
        }
        return true;
    }

    bool registerUser(const std::string& username, const std::string& password, const std::string& name) {
        std::string message = "REGISTER:" + username + ":" + password + ":" + name;
        if (send(clientSocket, message.c_str(), message.length(), 0) == SOCKET_ERROR) {
            return false;
        }
        return true;
    }

    void sendMessage(const Message& msg) {
        std::string message = "MESSAGE:" + msg.getData();
        send(clientSocket, message.c_str(), message.length(), 0);
    }

    void requestUserList() {
        std::string message = "GET_USERS";
        send(clientSocket, message.c_str(), message.length(), 0);
    }

    void setCurrentUser(const std::string& username) {
        currentUser = username;
    }

    std::string getCurrentUser() const {
        return currentUser;
    }

    const std::vector<std::string>& getOnlineUsers() const {
        return onlineUsers;
    }

    const std::vector<Message>& getReceivedMessages() const {
        return receivedMessages;
    }

    void clearMessages() {
        receivedMessages.clear();
    }

    bool isConnected() const {
        return connected;
    }

    bool hasNewMessages() const {
        return !receivedMessages.empty();
    }

private:
    void receiveMessages() {
        char buffer[4096];

        while (connected) {
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived <= 0) {
                connected = false;
                break;
            }

            buffer[bytesReceived] = '\0';
            std::string message(buffer);
            processServerMessage(message);
        }
    }

    void processServerMessage(const std::string& message) {
        if (message.find("LOGIN_SUCCESS:") == 0) {
            currentUser = message.substr(14);
        }
        else if (message.find("BANNED:") == 0) {
        }
        else if (message == "REGISTER_SUCCESS") {
        }
        else if (message == "REGISTER_FAILED:Username exists") {
        }
        else if (message.find("MESSAGE:") == 0) {
            std::string data = message.substr(8);
            Message msg = Message::getMessage(data);

            if (msg.Sender != currentUser) {
                receivedMessages.push_back(msg);
            }
        }
        else if (message.find("USERS_LIST:") == 0) {
            std::string usersStr = message.substr(11);
            onlineUsers.clear();
            std::istringstream ss(usersStr);
            std::string user;
            while (std::getline(ss, user, ',')) {
                if (!user.empty()) {
                    onlineUsers.push_back(user);
                }
            }
        }
    }
};

#endif
