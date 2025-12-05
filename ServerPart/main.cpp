#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <thread>
#include "server.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ChatServer server(8888);

    if (!server.initialize()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не удалось инициализировать сервер!");
        return 1;
    }

    std::thread serverThread([&server]() {
        server.run();
    });

    MainWindow w;
    w.setFixedSize(444, 652);
    w.show();

    int result = a.exec();

    server.stop();
    if (serverThread.joinable()) {
        serverThread.join();
    }

    return result;
}
