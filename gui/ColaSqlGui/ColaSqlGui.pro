QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

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
    ../../core/src/colasqltool.cpp \
    ../../core/src/database.cpp \
    ../../core/src/dataprocessor.cpp \
    ../../core/src/table.cpp \
    ../../core/src/user.cpp \
    ../../core/src/constraint/constraint.cpp \
    ../../core/src/constraint/default_constraint.cpp \
    ../../core/src/constraint/foreign_key_constraint.cpp \
    ../../core/src/constraint/not_null_constraint.cpp \
    ../../core/src/constraint/primary_key_constraint.cpp \
    ../../core/src/constraint/unique_constraint.cpp \
    ../../file/src/filemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    ui_login.cpp \
    ui_register.cpp \
    columndialog.cpp

HEADERS += \
    columndialog.h \
    mainwindow.h \
    ui_login.h \
    ui_register.h

FORMS += \
    mainwindow.ui \
    ui_login.ui \
    ui_register.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH +=$$quote("../../core/include")
INCLUDEPATH +=$$quote("../../core/include/constraint")
INCLUDEPATH +=$$quote("../../file/include/")