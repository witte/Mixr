QT += qml quick

CONFIG += c++11

SOURCES += main.cpp \
    transport.cpp \
    mixer.cpp \
    ChannelStrip.cpp

QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -pedantic-errors

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
    ChannelStrip.h

DISTFILES +=
