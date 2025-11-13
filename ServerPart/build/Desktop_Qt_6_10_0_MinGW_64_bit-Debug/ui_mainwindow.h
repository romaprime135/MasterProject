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
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *Name;
    QLabel *UserText;
    QLineEdit *UserLine;
    QPushButton *BanButton;
    QPushButton *UnbanButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(444, 218);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        Name = new QLabel(centralwidget);
        Name->setObjectName("Name");
        Name->setGeometry(QRect(180, 0, 84, 38));
        QFont font;
        font.setPointSize(22);
        Name->setFont(font);
        UserText = new QLabel(centralwidget);
        UserText->setObjectName("UserText");
        UserText->setGeometry(QRect(20, 60, 49, 28));
        QFont font1;
        font1.setPointSize(16);
        UserText->setFont(font1);
        UserLine = new QLineEdit(centralwidget);
        UserLine->setObjectName("UserLine");
        UserLine->setGeometry(QRect(80, 60, 321, 31));
        BanButton = new QPushButton(centralwidget);
        BanButton->setObjectName("BanButton");
        BanButton->setGeometry(QRect(10, 110, 201, 61));
        BanButton->setFont(font);
        UnbanButton = new QPushButton(centralwidget);
        UnbanButton->setObjectName("UnbanButton");
        UnbanButton->setGeometry(QRect(230, 110, 201, 61));
        UnbanButton->setFont(font);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 444, 21));
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
        Name->setText(QCoreApplication::translate("MainWindow", "Admin", nullptr));
        UserText->setText(QCoreApplication::translate("MainWindow", "USER", nullptr));
        BanButton->setText(QCoreApplication::translate("MainWindow", "BAN", nullptr));
        UnbanButton->setText(QCoreApplication::translate("MainWindow", "UNBAN", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
