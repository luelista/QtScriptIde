#-------------------------------------------------
#
# Project created by QtCreator 2018-03-14T20:44:30
#
#-------------------------------------------------

QT       += core gui qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScriptIDE
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    sidedocument.cpp \
    macrorecorder.cpp

HEADERS += \
        mainwindow.h \
    sidedocument.h \
    macrorecorder.h

FORMS += \
        mainwindow.ui

message(OUT PWD:  $$OUT_PWD)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../scintilla/qt/ScintillaEdit/release/ -lScintillaEdit4
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../scintilla/qt/ScintillaEdit/debug/ -lScintillaEdit4
else:unix: LIBS += -L$$OUT_PWD/../scintilla/bin/ -lScintillaEdit4

INCLUDEPATH += $$PWD/../scintilla/qt/ScintillaEdit \
    $$PWD/../scintilla/qt/ScintillaEditBase \
    $$PWD/../scintilla/include
DEPENDPATH += $$PWD/../scintilla/qt/ScintillaEdit

RESOURCES += \
    sideres.qrc
