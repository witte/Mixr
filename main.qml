import QtQuick.Controls 2.0
import QtQuick 2.7


ApplicationWindow {
    id: applicationWindow1
    visible: true

    minimumHeight: 320
    minimumWidth: 640
    height: 480
    width: 1040

    header: TransportBar { transport: c_transport }

//    background: Rectangle { color: "#f5f5f5" }

    ListView {
        id: row1
        anchors.fill: parent
        anchors.margins: 4
        anchors.bottomMargin: 8

//        Uncomment these to see how Qt handles the delegates creation and destruction
//        anchors.leftMargin: 250
//        anchors.rightMargin: 250

        orientation: ListView.Horizontal
        layoutDirection: ListView.RightToLeft
        model: MixrModel

        cacheBuffer: 0

        delegate: ChannelStrip { csModel: MixrModel }

//        Button {
//            id: btn_AddChannel

//            anchors.verticalCenter: parent.verticalCenter

//            height: 22
//            width: 22

//            text: "+"
//            enabled: false
//        }
        ScrollBar.horizontal: scr_Bottom

    }
    ScrollBar {
        id: scr_Bottom
        orientation: Qt.Horizontal

        anchors.bottom: parent.bottom

    }

    footer: Rectangle { height: 8; color: "#7ca28c" }
}
