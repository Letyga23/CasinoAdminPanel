QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminpanelwindow.cpp \
    creategametable.cpp \
    existingtables.cpp \
    filterdialog.cpp \
    loanapplications.cpp \
    loginform.cpp \
    main.cpp \
    mythread.cpp \
    role.cpp

HEADERS += \
    adminpanelwindow.h \
    creategametable.h \
    existingtables.h \
    filterdialog.h \
    gamePluginInterface.h \
    loanapplications.h \
    loginform.h \
    mythread.h \
    role.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Database/casino.sqlite \
    Database/name.sqlite \
    assets/добавить фильтр.png \
    assets/очистить поиск.png \
    assets/поиск.png \
    assets/сбросить фильтр.png \
    plugins/Card.dll \
    plugins/GamePlugins/BlackDjack.dll \
    plugins/GamePlugins/Devytka.dll \
    plugins/GamePlugins/libBlackDjack.a \
    plugins/GamePlugins/libDevytka.a \
    plugins/libCard.a

RESOURCES += \
    assets.qrc

FORMS +=
