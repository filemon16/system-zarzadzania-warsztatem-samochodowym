QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Czesc.cpp \
    DialogKlient.cpp \
    Klient.cpp \
    Osoba.cpp \
    Pracownik.cpp \
    Samochod.cpp \
    Warsztat.cpp \
    Zlecenie.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Czesc.h \
    DialogKlient.h \
    Klient.h \
    Osoba.h \
    Pracownik.h \
    Samochod.h \
    Warsztat.h \
    Zlecenie.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
