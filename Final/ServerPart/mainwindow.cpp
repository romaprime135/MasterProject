#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Chat Server - Ban Management");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BanButton_clicked()
{
    QString username = ui->UserLine->text().trimmed();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите имя пользователя!");
        return;
    }

    QMessageBox::information(this, "Бан", "Пользователь " + username + " забанен");
    ui->UserLine->clear();
}

void MainWindow::on_UnbanButton_clicked()
{
    QString username = ui->UserLine->text().trimmed();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите имя пользователя!");
        return;
    }

    QMessageBox::information(this, "Разбан", "Пользователь " + username + " разбанен");
    ui->UserLine->clear();
}
