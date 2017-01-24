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

        ChannelStrip { c_ChannelStrip: c_ChannelStrip_01 }

        ChannelStrip { c_ChannelStrip: c_ChannelStrip_02 }

        ChannelStrip { c_ChannelStrip: c_ChannelStrip_03 }

        ChannelStrip { c_ChannelStrip: c_ChannelStrip_04 }

        ChannelStrip { c_ChannelStrip: c_ChannelStrip_05 }

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
