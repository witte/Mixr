import QtQuick 2.7
import QtQuick.Controls 2.0

Item {
    property var transport

    id: control
    height: 36

    Keys.enabled: true
    Keys.onSpacePressed: {
        event.accepted = true
        if (!event.isAutoRepeat) btn_Start_ma.clicked(event);
    }
    Keys.onReleased: {
        event.accepted = true
    }

    Rectangle {
        anchors.fill: parent
        color: "#7ca28c"
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 6

        Button {
            id: btn_Start
            y: 4
            height: 28
            width: 28
            checkable: true
            opacity: enabled || highlighted || checked ? 1 : 0.3
            hoverEnabled: true

            background: Rectangle {
                height: 28
                width: 28
                color: (btn_Start_ma.pressed || btn_Start.checked) ? "#587263" : btn_Start.hovered ? "#70927e" : "transparent"
            }

            Image {
                x: btn_Start.checked ? 6 : 7
                y: 6
                opacity: 0.6
                source: btn_Start.checked ? "../img/media-playback-pause-symbolic.svg" : "../img/media-playback-start-symbolic.svg"
            }

            MouseArea {
                id: btn_Start_ma
                property int mouseDeltaX: 0
                property bool stopBubble: false

                anchors.fill: parent

                onClicked:  {
                    btn_Start.checked = !btn_Start.checked

                    if (btn_Start.checked) {
                        tmr_Transport.start()
                        transport.start()
                    }
                    else {
                        transport.stop()
                    }
                }
                onReleased: { stopBubble = false }

                onPositionChanged: {

                    if ((mouse.x < -20) && (!btn_Start_ma.stopBubble)) {

                        transport.stop()
                        transport.rewind()

                        btn_Start.checked = false
                        tmr_Transport.start()

                        stopBubble = true;
                    }
                }

            }
        }

        Column {
            y: 6

            TextEdit {
                id: textEdit1
                y: 3
                width: 86
                height: 13
                text: ""
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 12
            }

            TextEdit {
                id: textEdit2
                x: 0
                y: 18
                width: 86
                height: 13
                text: "1.1.000"
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 12
            }
        }
        Row {
            TextEdit {
                y: 8
                width: 48
                height: 20
                text: qsTr("120.00")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 12
            }
            TextEdit {
                x: 0
                y: 8
                width: 38
                height: 20
                text: qsTr("4/4")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 12
            }
        }
    }

    Timer {
        id: tmr_Transport
        interval: 20; running: true; repeat: true

        onTriggered: {
            textEdit1.text = transport.getTime()
            textEdit2.text = transport.getBBT()
        }
    }
}
