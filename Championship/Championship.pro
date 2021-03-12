QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    createchampionship.cpp \
    main.cpp \
    mainapp.cpp \
    mainregister.cpp \
    mainwindow.cpp \
    tournamentobj.cpp \
    tournamentwindow.cpp

HEADERS += \
    createchampionship.h \
    mainapp.h \
    mainregister.h \
    mainwindow.h \
    tournamentobj.h \
    tournamentwindow.h

FORMS += \
    createchampionship.ui \
    mainapp.ui \
    mainregister.ui \
    mainwindow.ui \
    tournamentwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
