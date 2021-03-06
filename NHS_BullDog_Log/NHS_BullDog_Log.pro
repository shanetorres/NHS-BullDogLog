#-------------------------------------------------
#
# Project created by QtCreator 2017-06-14T11:20:00
#
#-------------------------------------------------

QT       += core gui
QT       += gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NHS_BullDog_Log
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwidget.cpp \
    officerdelegate.cpp \
    addcontributiondialog.cpp \
    contributiondelegate.cpp \
    admindelegate.cpp \
    servicedelegate.cpp \
    addmeetingsdialog.cpp \
    meetingsdelegate.cpp


HEADERS  += mainwidget.h \
    officerdelegate.h \
    contributiondelegate.h \
    addcontributiondialog.h \
    admindelegate.h \
    servicedelegate.h \
    addmeetingsdialog.h \
    meetingsdelegate.h



FORMS    += mainwidget.ui \
    addcontributiondialog.ui \
    addmeetingsdialog.ui

RESOURCES += \
    pictures.qrc

DISTFILES += \
    data/currentstudents.csv \
    data/contributions.csv \
    data/currentprospectstudents.csv \
    ../../../../Desktop/nhslogo.png \
    png files/nhslogo.png \
    png files/nhslogo.png \
    nhslogo.png \
    nhslogo_a2I_icon.ico\
    data/contributions.csv \
    data/currentprospectstudents.csv \
    data/currentstudents.csv \
    data/meetings.csv \
    data/serviceprojects.csv





