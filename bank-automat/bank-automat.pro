QT += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    creditlimitmenu.cpp \
    depositmenu.cpp \
    expandablebutton.cpp \
    inputmenu.cpp \
    loginmenu.cpp \
    main.cpp \
    mainmenu.cpp \
    mainwindow.cpp \
    transactionmenu.cpp \
    transfermenu.cpp \
    withdrawmenu.cpp \

HEADERS += \
    expandablebutton.h \
    mainwindow.h \
    menus.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../RestAPIDLL/build/release/ -lRestAPI
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../RestAPIDLL/build/debug/ -lRestAPI
else:unix: LIBS += -L$$PWD/../RestAPIDLL/build/ -lRestAPI

INCLUDEPATH += $$PWD/../RestAPIDLL
DEPENDPATH += $$PWD/../RestAPIDLL


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../RFID_DLL/build/release/ -lRFID_DLL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../RFID_DLL/build/debug/ -lRFID_DLL
else:unix: LIBS += -L$$PWD/../RFID_DLL/build/ -lRFID_DLL

INCLUDEPATH += $$PWD/../RFID_DLL
DEPENDPATH += $$PWD/../RFID_DLL
