QT += core gui network

CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h \
    client.hpp

FORMS += \
    mainwindow.ui

win32 {
    LIBS += -lws2_32
    DEFINES += _WINSOCK_DEPRECATED_NO_WARNINGS WIN32_LEAN_AND_MEAN
}

QMAKE_CXXFLAGS += -Wno-unknown-pragmas
