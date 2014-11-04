#-------------------------------------------------
#
# Project created by QtCreator 2014-10-28T21:30:39
#
#-------------------------------------------------

VERSION = 1.0.0

TARGET = d3dscreencapture

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += release

#Uncomment this to test
#CONFIG += D3DCAP_APP

D3DCAP_APP {
DEFINES += D3DCAPTURE_APP
TEMPLATE = app
CONFIG += console
} else {
DEFINES += D3DCAPTURE_LIBRARY
TEMPLATE = lib
}

#DEFINES += DEEP_DEBUG

DX_SDK_DIR = "D:/Programs/Microsoft DirectX SDK (June 2010)"
DX_SDK_LIB_DIR = $$DX_SDK_DIR/Lib/x86
DX_SDK_INC_DIR = $$DX_SDK_DIR/Include

win32:LIBS += $$DX_SDK_LIB_DIR/d3d9.lib
win32:LIBS += $$DX_SDK_LIB_DIR/d3dx9.lib

build_pass:CONFIG(debug, debug|release) {
    DESTDIR = debug
} else {
    DESTDIR = release
}

INCLUDEPATH += .
INCLUDEPATH += $$DX_SDK_INC_DIR
DEPENDPATH = $$INCLUDEPATH

D3DCAP_APP {
SOURCES += main.cpp
}
SOURCES += d3dcapture.cpp

HEADERS += d3dcapture.h
HEADERS += d3dcapture_global.h
HEADERS += deepdebug.h
