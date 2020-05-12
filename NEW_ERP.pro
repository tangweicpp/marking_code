#-------------------------------------------------
#
# Project created by QtCreator 2020-02-24T15:10:33
#
#-------------------------------------------------

QT       += core gui sql axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NEW_ERP
TEMPLATE = app


SOURCES += main.cpp\
        frmlogon.cpp \
    commapi.cpp \
    frmmainwindow.cpp \
    frmmarkingcoderep.cpp \
    frmdefault.cpp \
    frmworep.cpp \
    frmcustwodata.cpp

HEADERS  += frmlogon.h \
    commapi.h \
    frmmainwindow.h \
    frmmarkingcoderep.h \
    frmdefault.h \
    frmworep.h \
    frmcustwodata.h

FORMS    += frmlogon.ui \
    frmmainwindow.ui \
    frmmarkingcoderep.ui \
    frmdefault.ui \
    frmworep.ui \
    frmcustwodata.ui

RESOURCES += \
    res.qrc

CONFIG += c++11
