QT_VERSION = 5

QT += qml quick

CONFIG += c++11

SOURCES += main.cpp \
    transport.cpp \
    ChannelStrip.cpp \
    ChannelStripModel.cpp

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
    ChannelStrip.h \
    ChannelStripModel.h

DISTFILES +=
