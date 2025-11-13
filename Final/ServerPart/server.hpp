#ifndef SERVER_HPP
#define SERVER_HPP

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <sstream>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
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
};

class ChatServer {
private:
    SOCKET serverSocket;
    unsigned short port;
    std::atomic<bool> running;
    std::vector<std::thread> clientThreads;
    std::mutex clientsMutex;

    // Qt SQL Database
    QSqlDatabase db;

    struct OnlineUser {
        SOCKET socket;
        std::string username;
        std::string ip;
    };
    std::vector<OnlineUser> onlineUsers;

public:
    ChatServer(unsigned short port) : port(port), running(false) {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    }

    ~ChatServer() {
        stop();
        if (db.isOpen()) {
            db.close();
        }
        WSACleanup();
    }

    bool initialize() {
        if (!initializeDatabase()) {
            return false;
        }

        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket == INVALID_SOCKET) {
            return false;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            closesocket(serverSocket);
            return false;
        }

        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            closesocket(serverSocket);
            return false;
        }

        return true;
    }

    void run() {
        running = true;

        while (running) {
            sockaddr_in clientAddr;
            int clientAddrSize = sizeof(clientAddr);

            SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
            if (clientSocket == INVALID_SOCKET) {
                continue;
            }

            clientThreads.emplace_back(&ChatServer::handleClient, this, clientSocket);
        }
    }

    void stop() {
        running = false;
        closesocket(serverSocket);

        for (auto& thread : clientThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        clientThreads.clear();
    }

private:
    void handleClient(SOCKET clientSocket) {
        char buffer[4096];

        try {
            while (running) {
                int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                if (bytesReceived <= 0) {
                    break;
                }

                buffer[bytesReceived] = '\0';
                std::string messageData(buffer);

                if (messageData.find("LOGIN:") == 0) {
                    std::string credentials = messageData.substr(6);
                    size_t pos = credentials.find(':');
                    if (pos != std::string::npos) {
                        std::string username = credentials.substr(0, pos);
                        std::string password = credentials.substr(pos + 1);

                        if (authenticateUser(username, password)) {
                            if (isUserBanned(username)) {
                                std::string response = "BANNED:User is banned";
                                send(clientSocket, response.c_str(), response.length(), 0);
                            } else {
                                OnlineUser onlineUser;
                                onlineUser.socket = clientSocket;
                                onlineUser.username = username;

                                char clientIP[INET_ADDRSTRLEN];
                                sockaddr_in addr;
                                int addrLen = sizeof(addr);
                                getpeername(clientSocket, (sockaddr*)&addr, &addrLen);
                                inet_ntop(AF_INET, &addr.sin_addr, clientIP, INET_ADDRSTRLEN);
                                onlineUser.ip = clientIP;

                                {
                                    std::lock_guard<std::mutex> lock(clientsMutex);
                                    onlineUsers.push_back(onlineUser);
                                }

                                std::string response = "LOGIN_SUCCESS:" + username;
                                send(clientSocket, response.c_str(), response.length(), 0);
                                sendUserList(clientSocket);
                            }
                        } else {
                            std::string response = "LOGIN_FAILED:Invalid credentials";
                            send(clientSocket, response.c_str(), response.length(), 0);
                        }
                    }
                }
                else if (messageData.find("REGISTER:") == 0) {
                    std::string data = messageData.substr(9);
                    size_t pos1 = data.find(':');
                    size_t pos2 = data.find(':', pos1 + 1);

                    if (pos1 != std::string::npos && pos2 != std::string::npos) {
                        std::string username = data.substr(0, pos1);
                        std::string password = data.substr(pos1 + 1, pos2 - pos1 - 1);
                        std::string name = data.substr(pos2 + 1);

                        if (registerUser(username, password, name)) {
                            std::string response = "REGISTER_SUCCESS";
                            send(clientSocket, response.c_str(), response.length(), 0);
                        } else {
                            std::string response = "REGISTER_FAILED:Username exists";
                            send(clientSocket, response.c_str(), response.length(), 0);
                        }
                    }
                }
                else if (messageData.find("MESSAGE:") == 0) {
                    std::string data = messageData.substr(8);
                    Message msg = Message::getMessage(data);

                    if (!isUserBanned(msg.Sender)) {
                        processMessage(msg);
                        logMessage(msg);
                    }
                }
                else if (messageData == "GET_USERS") {
                    sendUserList(clientSocket);
                }
                else if (messageData.find("BAN:") == 0) {
                    std::string username = messageData.substr(4);
                    if (banUser(username)) {
                        std::lock_guard<std::mutex> lock(clientsMutex);
                        for (auto it = onlineUsers.begin(); it != onlineUsers.end(); ) {
                            if (it->username == username) {
                                std::string banMsg = "BANNED:You have been banned";
                                send(it->socket, banMsg.c_str(), banMsg.length(), 0);
                                closesocket(it->socket);
                                it = onlineUsers.erase(it);
                            } else {
                                ++it;
                            }
                        }
                    }
                }
                else if (messageData.find("UNBAN:") == 0) {
                    std::string username = messageData.substr(6);
                    unbanUser(username);
                }
            }
        }
        catch (...) {
        }

        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            for (auto it = onlineUsers.begin(); it != onlineUsers.end(); ) {
                if (it->socket == clientSocket) {
                    it = onlineUsers.erase(it);
                    break;
                } else {
                    ++it;
                }
            }
        }

        closesocket(clientSocket);
    }

    void processMessage(const Message& msg) {
        if (msg.Getter == "ALL") {
            broadcastMessage(msg, msg.Sender);
        } else {
            std::lock_guard<std::mutex> lock(clientsMutex);
            for (const auto& user : onlineUsers) {
                if (user.username == msg.Getter) {
                    std::string messageData = "MESSAGE:" + msg.getData();
                    send(user.socket, messageData.c_str(), messageData.length(), 0);
                    break;
                }
            }
        }
    }

    void broadcastMessage(const Message& msg, const std::string& excludeUser) {
        std::string messageData = "MESSAGE:" + msg.getData();

        std::lock_guard<std::mutex> lock(clientsMutex);
        for (const auto& user : onlineUsers) {
            if (user.username != excludeUser && !isUserBanned(user.username)) {
                send(user.socket, messageData.c_str(), messageData.length(), 0);
            }
        }
    }

    void sendUserList(SOCKET clientSocket) {
        std::lock_guard<std::mutex> lock(clientsMutex);

        std::string userList = "USERS_LIST:";
        for (const auto& user : onlineUsers) {
            userList += user.username + ",";
        }

        if (!userList.empty() && userList.back() == ',') {
            userList.pop_back();
        }

        send(clientSocket, userList.c_str(), userList.length(), 0);
    }

    bool initializeDatabase() {
        db = QSqlDatabase::addDatabase("QSQLITE", "chat_server_connection");
        db.setDatabaseName("chat_server.db");

        if (!db.open()) {
            return false;
        }

        QSqlQuery query(db);

        bool success = query.exec(
            "CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "username TEXT UNIQUE NOT NULL,"
            "password TEXT NOT NULL,"
            "name TEXT NOT NULL,"
            "is_banned INTEGER DEFAULT 0,"
            "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)"
            );

        if (!success) {
            return false;
        }

        // Таблица сообщений
        success = query.exec(
            "CREATE TABLE IF NOT EXISTS messages ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "sender TEXT NOT NULL,"
            "getter TEXT NOT NULL,"
            "text TEXT NOT NULL,"
            "tag TEXT NOT NULL,"
            "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP)"
            );

        return success;
    }

    bool registerUser(const std::string& username, const std::string& password, const std::string& name) {
        if (!db.isOpen()) return false;

        QSqlQuery query(db);
        query.prepare("INSERT INTO users (username, password, name) VALUES (?, ?, ?)");
        query.addBindValue(QString::fromStdString(username));
        query.addBindValue(QString::fromStdString(password));
        query.addBindValue(QString::fromStdString(name));

        return query.exec();
    }

    bool authenticateUser(const std::string& username, const std::string& password) {
        if (!db.isOpen()) return false;

        QSqlQuery query(db);
        query.prepare("SELECT username FROM users WHERE username = ? AND password = ?");
        query.addBindValue(QString::fromStdString(username));
        query.addBindValue(QString::fromStdString(password));

        if (query.exec() && query.next()) {
            return true;
        }

        return false;
    }

    bool banUser(const std::string& username) {
        if (!db.isOpen()) return false;

        QSqlQuery query(db);
        query.prepare("UPDATE users SET is_banned = 1 WHERE username = ?");
        query.addBindValue(QString::fromStdString(username));

        return query.exec();
    }

    bool unbanUser(const std::string& username) {
        if (!db.isOpen()) return false;

        QSqlQuery query(db);
        query.prepare("UPDATE users SET is_banned = 0 WHERE username = ?");
        query.addBindValue(QString::fromStdString(username));

        return query.exec();
    }

    bool isUserBanned(const std::string& username) {
        if (!db.isOpen()) return false;

        QSqlQuery query(db);
        query.prepare("SELECT is_banned FROM users WHERE username = ?");
        query.addBindValue(QString::fromStdString(username));

        if (query.exec() && query.next()) {
            return query.value(0).toInt() == 1;
        }

        return false;
    }

    void logMessage(const Message& msg) {
        if (!db.isOpen()) return;

        QSqlQuery query(db);
        query.prepare("INSERT INTO messages (sender, getter, text, tag) VALUES (?, ?, ?, ?)");
        query.addBindValue(QString::fromStdString(msg.Sender));
        query.addBindValue(QString::fromStdString(msg.Getter));
        query.addBindValue(QString::fromStdString(msg.Text));
        query.addBindValue(QString::fromStdString(msg.Tag));
        query.exec();
    }
};

#endif
