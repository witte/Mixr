import QtQuick 2.7
import QtQuick.Controls 2.0
import "controls" as Controls

Item {
    id: control
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    width: 82

    property QtObject c_ChannelStrip

    Rectangle {
        anchors.fill: parent
        color: "#eee"
    }

    Button {
        id: btn_From
        anchors.top: parent.top
        anchors.topMargin: 1
        height: 20
        width: 80
        x: 1

        onPressed: {  if (!cmb_From.visible) cmb_From.open()  }
        Component.onCompleted: this.text = c_ChannelStrip.getName()

        Component {  id: portsModel; ListModel { }  }

        PortsSelector {
            id: cmb_From
            y: 1
            x: 1


            onS_ConnectPort: {

                if (connect)
                    c_ChannelStrip.connectFrom(port, side)

                else
                    c_ChannelStrip.disconnectFrom(port, side)

            }

            Component.onCompleted: {
                var mdl_Source = c_ChannelStrip.getJackOutputPorts()
                var mdl_Sorted = portsModel.createObject(this);

                for (var i = 0; i < mdl_Source.length; i++)
                    mdl_Sorted.append(  {"portName": mdl_Source[i]}  )

                this.model = mdl_Sorted
            }
        }

    }



    Text {
        anchors.top: btn_From.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: "+"
    }

    Controls.Knob {
        id: dial_Volume
        anchors.bottom: sld_Fader.top
        onPositionChanged: c_ChannelStrip.setPan(dial_Volume.position)
    }

    Controls.Fader {
        id: sld_Fader
        anchors.bottom: rowLayout1.top
        onPositionChanged: c_ChannelStrip.setVolume(sld_Fader.position)
    }

    Row {
        id: rowLayout1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 2

        Controls.MuteButton {
            text: "M"
            onClicked: c_ChannelStrip.isMuted(this.checked);
        }
        Controls.SoloButton {
            text: "S"
        }
    }
/*
    Button {
        id: btn_To
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 1
        height: 20
        width: 80
        x: 1

        onPressed: {  if (!cmb_To.visible) cmb_To.open()  }

        Component {  id: portsModel2; ListModel { }  }

        PortsSelector {
            id: cmb_To
//            y: 1
            y: -this.height + 20
            x: 1


            onS_ConnectPort: {

                if (connect)
                    c_ChannelStrip.connectTo(port, side)

                else
                    c_ChannelStrip.disconnectTo(port, side)

            }

            Component.onCompleted: {
                var mdl_Source = c_ChannelStrip.getJackInputPorts()
                var mdl_Sorted = portsModel2.createObject(this);

                for (var i = 0; i < mdl_Source.length; i++)
                    mdl_Sorted.append(  {"portName": mdl_Source[i]}  )

                this.model = mdl_Sorted
            }
        }

    }
*/
}
