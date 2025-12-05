/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTextBrowser *MessageHistory;
    QLineEdit *MessageText;
    QLabel *MessageHelp;
    QLabel *GetterHelp;
    QLineEdit *GetterText;
    QPushButton *SendButton;
    QLabel *LoginHelp;
    QLineEdit *LoginText;
    QLabel *PasswordHelp;
    QLineEdit *PasswordText;
    QPushButton *RegButton;
    QPushButton *LogButton;
    QLabel *PasswordHelp_2;
    QRadioButton *RadioMin;
    QRadioButton *RadioMed;
    QRadioButton *RadioMax;
    QLineEdit *NameText;
    QLabel *NameHelp;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 844);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        MessageHistory = new QTextBrowser(centralwidget);
        MessageHistory->setObjectName("MessageHistory");
        MessageHistory->setGeometry(QRect(0, 390, 781, 281));
        MessageText = new QLineEdit(centralwidget);
        MessageText->setObjectName("MessageText");
        MessageText->setGeometry(QRect(10, 40, 441, 51));
        MessageHelp = new QLabel(centralwidget);
        MessageHelp->setObjectName("MessageHelp");
        MessageHelp->setGeometry(QRect(180, 0, 104, 36));
        QFont font;
        font.setPointSize(20);
        MessageHelp->setFont(font);
        GetterHelp = new QLabel(centralwidget);
        GetterHelp->setObjectName("GetterHelp");
        GetterHelp->setGeometry(QRect(60, 100, 335, 36));
        GetterHelp->setFont(font);
        GetterText = new QLineEdit(centralwidget);
        GetterText->setObjectName("GetterText");
        GetterText->setGeometry(QRect(10, 140, 441, 51));
        SendButton = new QPushButton(centralwidget);
        SendButton->setObjectName("SendButton");
        SendButton->setGeometry(QRect(10, 310, 441, 71));
        QFont font1;
        font1.setPointSize(22);
        SendButton->setFont(font1);
        LoginHelp = new QLabel(centralwidget);
        LoginHelp->setObjectName("LoginHelp");
        LoginHelp->setGeometry(QRect(590, 100, 67, 36));
        LoginHelp->setFont(font);
        LoginText = new QLineEdit(centralwidget);
        LoginText->setObjectName("LoginText");
        LoginText->setGeometry(QRect(460, 140, 321, 51));
        PasswordHelp = new QLabel(centralwidget);
        PasswordHelp->setObjectName("PasswordHelp");
        PasswordHelp->setGeometry(QRect(570, 200, 111, 36));
        PasswordHelp->setFont(font);
        PasswordText = new QLineEdit(centralwidget);
        PasswordText->setObjectName("PasswordText");
        PasswordText->setGeometry(QRect(460, 250, 321, 51));
        RegButton = new QPushButton(centralwidget);
        RegButton->setObjectName("RegButton");
        RegButton->setGeometry(QRect(460, 310, 171, 71));
        RegButton->setFont(font1);
        LogButton = new QPushButton(centralwidget);
        LogButton->setObjectName("LogButton");
        LogButton->setGeometry(QRect(640, 310, 141, 71));
        LogButton->setFont(font1);
        PasswordHelp_2 = new QLabel(centralwidget);
        PasswordHelp_2->setObjectName("PasswordHelp_2");
        PasswordHelp_2->setGeometry(QRect(160, 200, 136, 36));
        PasswordHelp_2->setFont(font);
        RadioMin = new QRadioButton(centralwidget);
        RadioMin->setObjectName("RadioMin");
        RadioMin->setGeometry(QRect(20, 250, 113, 33));
        QFont font2;
        font2.setPointSize(16);
        RadioMin->setFont(font2);
        RadioMed = new QRadioButton(centralwidget);
        RadioMed->setObjectName("RadioMed");
        RadioMed->setEnabled(true);
        RadioMed->setGeometry(QRect(180, 250, 101, 33));
        RadioMed->setFont(font2);
        RadioMax = new QRadioButton(centralwidget);
        RadioMax->setObjectName("RadioMax");
        RadioMax->setGeometry(QRect(330, 250, 117, 33));
        RadioMax->setFont(font2);
        NameText = new QLineEdit(centralwidget);
        NameText->setObjectName("NameText");
        NameText->setGeometry(QRect(460, 40, 321, 51));
        NameHelp = new QLabel(centralwidget);
        NameHelp->setObjectName("NameHelp");
        NameHelp->setGeometry(QRect(590, 0, 71, 36));
        NameHelp->setFont(font);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        MessageHistory->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">No message recived</span></p></body></html>", nullptr));
        MessageHelp->setText(QCoreApplication::translate("MainWindow", "Message", nullptr));
        GetterHelp->setText(QCoreApplication::translate("MainWindow", "Getter (nothing to send ALL)", nullptr));
        SendButton->setText(QCoreApplication::translate("MainWindow", "SEND", nullptr));
        LoginHelp->setText(QCoreApplication::translate("MainWindow", "Login", nullptr));
        PasswordHelp->setText(QCoreApplication::translate("MainWindow", "Password", nullptr));
        RegButton->setText(QCoreApplication::translate("MainWindow", "REGIESTER", nullptr));
        LogButton->setText(QCoreApplication::translate("MainWindow", "LOGON", nullptr));
        PasswordHelp_2->setText(QCoreApplication::translate("MainWindow", "Importance", nullptr));
        RadioMin->setText(QCoreApplication::translate("MainWindow", "Minimum", nullptr));
        RadioMed->setText(QCoreApplication::translate("MainWindow", "Medium", nullptr));
        RadioMax->setText(QCoreApplication::translate("MainWindow", "Maximum", nullptr));
        NameText->setText(QString());
        NameHelp->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
