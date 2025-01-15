QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32: {
    include("D:/workspace/ffmpeg/ffmpeg.pri")
}


# linux
LIBS += -L/home/FFmpeg-n7.1/build/lib/ -lavutil -lavformat -lavcodec -lswscale -lswresample
INCLUDEPATH += /home/FFmpeg-n7.1/build/include
DEPENDPATH += /home/FFmpeg-n7.1/build/include

SOURCES += \
    FFmpegPlayer.cpp \
    QAudioPlayer.cpp \
    ZPlayWidget.cpp \
    main.cpp \
    mainwindow.cpp \
    openmediawindow.cpp \
    videoprocessor.cpp

HEADERS += \
    FFmpegPlayer.h \
    QAudioPlayer.h \
    QIORingBuffer.h \
    ZPlayWidget.h \
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

