#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_SendButton_clicked();
    void on_RegButton_clicked();
    void on_LogButton_clicked();
    void checkForNewMessages();

private:
    Ui::MainWindow *ui;
    ChatClient client;
    bool isLoggedIn;
    QTimer *messageTimer;
};

#endif
