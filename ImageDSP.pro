#-------------------------------------------------
#
# Project created by QtCreator 2016-04-21T07:46:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageDSP
TEMPLATE = app

SOURCES += \
	src/main.cpp \
	src/MainWindow.cpp \
	src/ImageViewer.cpp \
	src/ItemWidget.cpp \
	src/InImgItem.cpp \
	src/ParamItem.cpp \
	src/UniversalItemDelegate.cpp \
	src/TableModel.cpp \
	src/Project.cpp

HEADERS += \
	src/MainWindow.h \
	src/ImageViewer.h \
	src/ItemWidget.h \
	src/InImgItem.h \
	src/ParamItem.h \
	src/UniversalItemDelegate.h \
	src/TableModel.h \
	src/Project.h

FORMS += \
	src/MainWindow.ui \
	src/InImgItem.ui \
	src/ParamItem.ui
