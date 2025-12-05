#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , usersModel(new QStandardItemModel(this))
    , messagesModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
    setWindowTitle("Chat Server - Admin Panel");

    usersModel->setHorizontalHeaderLabels({"Username", "IP", "Status"});
    ui->UsersView->setModel(usersModel);
    ui->UsersView->setHeaderHidden(false);
    ui->UsersView->setRootIsDecorated(false);

    ui->UsersView->setColumnWidth(0, 70);
    ui->UsersView->setColumnWidth(1, 70);
    ui->UsersView->setColumnWidth(2, 40);

    messagesModel->setHorizontalHeaderLabels({"Sender", "Receiver", "Message", "Time"});
    ui->MessagesView->setModel(messagesModel);
    ui->MessagesView->setHeaderHidden(false);
    ui->MessagesView->setRootIsDecorated(false);

    ui->MessagesView->setColumnWidth(0, 50);
    ui->MessagesView->setColumnWidth(1, 50);
    ui->MessagesView->setColumnWidth(2, 60);
    ui->MessagesView->setColumnWidth(3, 70);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateUsersList);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateMessagesList);
    timer->start(2000);

    updateUsersList();
    updateMessagesList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateUsersList()
{
    usersModel->removeRows(0, usersModel->rowCount());

    QSqlDatabase db = QSqlDatabase::database("chat_server_connection");
    if (!db.isOpen()) {
        db = QSqlDatabase::addDatabase("QSQLITE", "chat_server_connection");
        db.setDatabaseName("chat_server.db");
        if (!db.open()) {
            return;
        }
    }

    QSqlQuery query(db);
    query.prepare("SELECT username, name, is_banned FROM users ORDER BY username");

    if (query.exec()) {
        while (query.next()) {
            QString username = query.value(0).toString();
            QString name = query.value(1).toString();
            bool isBanned = query.value(2).toBool();

            QList<QStandardItem*> rowItems;

            QStandardItem *usernameItem = new QStandardItem(username);
            rowItems.append(usernameItem);

            QStandardItem *nameItem = new QStandardItem(name);
            rowItems.append(nameItem);

            QStandardItem *statusItem = new QStandardItem();
            if (isBanned) {
                statusItem->setText("Banned");
                statusItem->setForeground(Qt::red);
                usernameItem->setForeground(Qt::red);
            } else {
                statusItem->setText("Active");
                statusItem->setForeground(Qt::darkGreen);
            }
            rowItems.append(statusItem);

            usersModel->appendRow(rowItems);
        }
    }

    usersModel->setHorizontalHeaderLabels({"Username", "Name", "Status"});
}

void MainWindow::updateMessagesList()
{
    messagesModel->removeRows(0, messagesModel->rowCount());

    QSqlDatabase db = QSqlDatabase::database("chat_server_connection");
    if (!db.isOpen()) {
        db = QSqlDatabase::addDatabase("QSQLITE", "chat_server_connection");
        db.setDatabaseName("chat_server.db");
        if (!db.open()) {
            return;
        }
    }

    QSqlQuery query(db);
    query.prepare("SELECT sender, getter, text, tag, timestamp FROM messages ORDER BY timestamp DESC LIMIT 50");

    if (query.exec()) {
        while (query.next()) {
            QString sender = query.value(0).toString();
            QString receiver = query.value(1).toString();
            QString text = query.value(2).toString();
            QString tag = query.value(3).toString();
            QDateTime timestamp = query.value(4).toDateTime();

            QList<QStandardItem*> rowItems;

            QStandardItem *senderItem = new QStandardItem(sender);
            rowItems.append(senderItem);

            QStandardItem *receiverItem = new QStandardItem(receiver);
            rowItems.append(receiverItem);

            QString displayText = text;
            if (displayText.length() > 20) {
                displayText = displayText.left(20) + "...";
            }
            QStandardItem *textItem = new QStandardItem(displayText);
            textItem->setToolTip(text);
            rowItems.append(textItem);

            QString timeStr = timestamp.toString("hh:mm");
            QStandardItem *timeItem = new QStandardItem(timeStr);
            rowItems.append(timeItem);

            messagesModel->appendRow(rowItems);
        }
    }

    messagesModel->setHorizontalHeaderLabels({"Sender", "To", "Message", "Time"});
}

void MainWindow::on_BanButton_clicked()
{
    QString username = ui->UserLine->text().trimmed();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите имя пользователя!");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("chat_server_connection");
    if (!db.isOpen()) {
        db = QSqlDatabase::addDatabase("QSQLITE", "chat_server_connection");
        db.setDatabaseName("chat_server.db");
        if (!db.open()) {
            QMessageBox::warning(this, "Ошибка", "Не удалось подключиться к базе данных!");
            return;
        }
    }

    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT username FROM users WHERE username = ?");
    checkQuery.addBindValue(username);

    if (!checkQuery.exec() || !checkQuery.next()) {
        QMessageBox::warning(this, "Ошибка", "Пользователь не найден!");
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE users SET is_banned = 1 WHERE username = ?");
    query.addBindValue(username);

    if (query.exec()) {
        QMessageBox::information(this, "Бан", "Пользователь " + username + " забанен");
        ui->UserLine->clear();
        updateUsersList();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось забанить пользователя!");
    }
}

void MainWindow::on_UnbanButton_clicked()
{
    QString username = ui->UserLine->text().trimmed();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите имя пользователя!");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("chat_server_connection");
    if (!db.isOpen()) {
        db = QSqlDatabase::addDatabase("QSQLITE", "chat_server_connection");
        db.setDatabaseName("chat_server.db");
        if (!db.open()) {
            QMessageBox::warning(this, "Ошибка", "Не удалось подключиться к базе данных!");
            return;
        }
    }

    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT username FROM users WHERE username = ?");
    checkQuery.addBindValue(username);

    if (!checkQuery.exec() || !checkQuery.next()) {
        QMessageBox::warning(this, "Ошибка", "Пользователь не найден!");
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE users SET is_banned = 0 WHERE username = ?");
    query.addBindValue(username);

    if (query.exec()) {
        QMessageBox::information(this, "Разбан", "Пользователь " + username + " разбанен");
        ui->UserLine->clear();
        updateUsersList();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось разбанить пользователя!");
    }
}
