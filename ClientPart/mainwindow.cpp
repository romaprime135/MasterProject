#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isLoggedIn(false)
{
    ui->setupUi(this);

    setWindowTitle("Chat Client");

    ui->RadioMed->setChecked(true);

    if (!client.connectToServer("127.0.0.1", 8888)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к серверу!");
    }

    messageTimer = new QTimer(this);
    connect(messageTimer, &QTimer::timeout, this, &MainWindow::checkForNewMessages);
    messageTimer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_SendButton_clicked()
{
    if (!isLoggedIn) {
        QMessageBox::warning(this, "Ошибка", "Сначала войдите в систему!");
        return;
    }

    QString messageText = ui->MessageText->text().trimmed();
    QString getter = ui->GetterText->text().trimmed();

    if (messageText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите сообщение!");
        return;
    }

    if (getter.isEmpty()) {
        getter = "ALL";
    }

    QString priority;
    if (ui->RadioMin->isChecked()) priority = "Low";
    else if (ui->RadioMed->isChecked()) priority = "Medium";
    else if (ui->RadioMax->isChecked()) priority = "High";
    else priority = "Medium";

    Message msg(getter.toStdString(),
                client.getCurrentUser(),
                messageText.toStdString(),
                priority.toStdString());

    client.sendMessage(msg);

    QString myMessage = QString("<b>[You → %1]</b> (%2): %3<br>---<br>")
                            .arg(getter)
                            .arg(priority)
                            .arg(messageText);

    ui->MessageHistory->append(myMessage);

    ui->MessageText->clear();
}

void MainWindow::on_RegButton_clicked()
{
    QString login = ui->LoginText->text().trimmed();
    QString password = ui->PasswordText->text().trimmed();
    QString name = ui->NameText->text().trimmed();

    if (login.isEmpty() || password.isEmpty() || name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля для регистрации!");
        return;
    }

    if (client.registerUser(login.toStdString(), password.toStdString(), name.toStdString())) {
        QTimer::singleShot(1000, [this]() {
            QMessageBox::information(this, "Успех", "Регистрация завершена!");
            ui->LoginText->clear();
            ui->PasswordText->clear();
            ui->NameText->clear();
        });
    }
}

void MainWindow::on_LogButton_clicked()
{
    QString login = ui->LoginText->text().trimmed();
    QString password = ui->PasswordText->text().trimmed();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите логин и пароль!");
        return;
    }

    if (client.login(login.toStdString(), password.toStdString())) {
        QTimer::singleShot(1000, [this, login]() {
            if (client.getCurrentUser() == login.toStdString()) {
                isLoggedIn = true;
                setWindowTitle("Chat Client - " + login);
                QMessageBox::information(this, "Успех", "Вход выполнен!");

                ui->LoginText->clear();
                ui->PasswordText->clear();
                ui->NameText->clear();
            } else {
                QMessageBox::warning(this, "Ошибка", "Неверные данные для входа!");
            }
        });
    }
}

void MainWindow::checkForNewMessages()
{
    if (isLoggedIn && client.hasNewMessages()) {
        auto messages = client.getReceivedMessages();
        for (const auto& msg : messages) {
            ui->MessageHistory->append(msg.toQString());
        }
        client.clearMessages();

        if (ui->MessageHistory->toPlainText().contains("No message recived")) {
            ui->MessageHistory->clear();
        }
    }
}
