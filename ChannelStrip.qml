import QtQuick 2.7
import QtQuick.Controls 2.0
import "controls" as Controls

Item {
    id: control
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    width: 82

    property QtObject fader

    Rectangle {
        anchors.fill: parent
        color: "#eee"
    }

    PortsSelector {
        id: cmb_From
        anchors.top: parent.top
        x: 1

        model: fader.getJackOutputPorts()
        onCurrentIndexChanged: fader.connectFrom( cmb_From.model[cmb_From.currentIndex] )
    }

    Text {
        anchors.top: cmb_From.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: "+"
    }

    Controls.Knob {
        id: dial_Volume
        anchors.bottom: sld_Fader.top
        onPositionChanged: fader.setPan(dial_Volume.position)
    }

    Controls.Fader {
        id: sld_Fader
        anchors.bottom: rowLayout1.top
        onPositionChanged: fader.setVolume(sld_Fader.position)
    }

    Row {
        id: rowLayout1
        anchors.bottom: cmb_To.top
        anchors.bottomMargin: 4
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 2

        Controls.MuteButton {
            text: "M"
            onClicked: fader.setMute(this.checked);
        }
        Controls.SoloButton {
            text: "S"
        }
    }

    PortsSelector {
        id: cmb_To
        anchors.bottom: parent.bottom
        model: fader.getJackInputPorts()
        x: 1

        onCurrentIndexChanged: fader.connectTo( cmb_To.model[cmb_To.currentIndex] )
    }

}
