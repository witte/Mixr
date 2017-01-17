QT += qml quick

CONFIG += c++11

SOURCES += main.cpp \
    transport.cpp \
    mixer.cpp \
    Fader.cpp

RESOURCES += qml.qrc

LIBS += -ljack

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    transport.h \
    mixer.h \
    Fader.h

DISTFILES +=
