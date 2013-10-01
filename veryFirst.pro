#-------------------------------------------------
#
# Project created by QtCreator 2013-09-12T22:28:58
#
#-------------------------------------------------

QT       += core gui testlib network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = veryFirst
TEMPLATE = app


#INCLUDEPATH += /opt/opencv/include/opencv
#INCLUDEPATH += /opt/opencv/include
#INCLUDEPATH += /opt/opencv/lib
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include

#LIBS += -L/opt/opencv/lib

LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_contrib
LIBS += -lopencv_objdetect
#LIBS += -lopencv_features2d
#LIBS += -lopencv_calib3d
#LIBS += -lopencv_ml
#LIBS += -lopencv_video
#LIBS += -lopencv_flann

SOURCES += main.cpp\
        mainwindow.cpp \
    networker.cpp \
    clientserverinterface.cpp \
    mainworker.cpp

HEADERS  += mainwindow.h \
    networker.h \
    clientserverinterface.h \
    mainworker.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    haarcascade_frontalface_default.xml \
    1380502074_camera.png
