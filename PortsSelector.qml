import QtQuick 2.7
import QtQuick.Controls 2.0
import "controls" as Controls


Popup {
    id: control

    signal s_ConnectPort(string port, bool connect, int side)
    property ListModel model: ListModel {}

    property int centerX: 0
    property string text: ""


    y: -1
    x: -151 + centerX
    width: 300
    height: (listview.contentHeight > 400) ? 400 : listview.contentHeight
    padding: 2
    background: Rectangle { color: "#f8f8f8" }

    Button {
        height: 18
        width: 296
        text: control.text
        onClicked: control.close()
    }

    ListView {
        anchors.fill: parent
        anchors.topMargin: 20

        id: listview
        clip: true

        model: control.model

        delegate: Controls.CheckPort {
            text: modelData
            onConnected1Changed: { s_ConnectPort(portName, connected1, 1) }
            onConnected2Changed: { s_ConnectPort(portName, connected2, 2) }
        }

        ScrollBar.vertical: ScrollBar {}

    }
}
