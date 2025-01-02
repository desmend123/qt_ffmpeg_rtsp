QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -LD:/gstreamer/1.0/msvc_x86_64/lib -lgobject-2.0 \
    -lgstreamer-1.0 -lgstbase-1.0 -lglib-2.0 -lgstapp-1.0

#gstaudio-1.0;gstvideo-1.0;gstapp-1.0;gstbase-1.0;gstreamer-1.0;gobject-2.0;glib-2.0

win32: {
    include("D:/gstreamer/gstreamer.pri")
}

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    openmediawindow.cpp \
    videoprocessor.cpp

HEADERS += \
    mainwindow.h \
    openmediawindow.h \
    videoprocessor.h

FORMS += \
    mainwindow.ui \
    openmediawindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
