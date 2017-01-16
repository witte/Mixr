import QtQuick.Controls 2.0
import QtQuick 2.7

ApplicationWindow {
    visible: true

    minimumHeight: 320
    minimumWidth: 640
    height: 480
    width: 640

    header: TransportBar { transport: c_transport }

    Row {
        id: row1
        anchors {
            fill: parent
            topMargin: 8
            leftMargin: 2
            rightMargin: 2
            bottomMargin: 8
        }
        spacing: 2

        ChannelStrip { fader: c_fader_01 }

        ChannelStrip { fader: c_fader_02 }

        ChannelStrip { fader: c_fader_03 }

        ChannelStrip { fader: c_fader_04 }

        Button {
            id: btn_AddChannel

            anchors.verticalCenter: parent.verticalCenter

            height: 22
            width: 22

            text: "+"
            enabled: false
        }

    }

    footer: Rectangle { height: 8; color: "#7ca28c" }
}
