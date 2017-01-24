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

    ListView {
        id: row1
        anchors.fill: parent
        anchors.margins: 4

//        Uncomment these to see how Qt handles the delegates creation and destruction
//        anchors.leftMargin: 250
//        anchors.rightMargin: 250

        spacing: 2
        orientation: ListView.Horizontal
        layoutDirection: ListView.RightToLeft
        model: MixrModel

        delegate: ChannelStrip { csModel: row1.model }


//        Button {
//            id: btn_AddChannel

//            anchors.verticalCenter: parent.verticalCenter

//            height: 22
//            width: 22

//            text: "+"
//            enabled: false
//        }

    }

    footer: Rectangle { height: 8; color: "#7ca28c" }
}
