import QtQuick.Controls 2.0
import QtQuick 2.7


MouseArea {
    id: control

    property bool connected1: false
    property bool connected2: false
    property int __midWidth: 150
    property string text: ""


    width: 300
    height: 22
    hoverEnabled: true

    Rectangle {
        id: bgd_Left

        x: 2
        y: 2
        width: 148
        height: 18
        color: connected1 ? "#2b4b7a" : "#467ac6"
    }

    Rectangle {
        id: bgd_Right

        x: 150
        y: 2
        width: 148
        height: 18
        color: connected2 ? "#2b4b7a" : "#467ac6"
    }


    Image {
        id: prt_Left
        y: 4
        x: 5
        width: 14
        height: 14
        opacity: connected1 ? 1 : 0.5

        source: connected1 ? "../img/port-on.svg" : "../img/port-off.svg"
    }

    Text {
        id: txt_PortName

        height: 22
        width: 300
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        color: "#ffffff"
        text: control.text
    }

    Image {
        id: prt_Right

        anchors.right: parent.right
        anchors.rightMargin: 4

        y: 4
        width: 14
        height: 14
        opacity: connected2 ? 1 : 0.5

        source: connected2 ? "../img/port-on.svg" : "../img/port-off.svg"
    }

    Rectangle {
        id: bgd_background
        anchors.fill: parent
        color: control.containsMouse ? "#0f000000" : "transparent"
    }


    onClicked: {
        if (mouseX > __midWidth)
            connected2 = !connected2

        else
            connected1 = !connected1

    }
}
