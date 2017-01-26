import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import "controls" as Controls
import "style"

ItemDelegate {
    id: control
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    width: 81

    property QtObject csModel

    background: Rectangle {
        anchors.fill: mst_Channel
        anchors.bottomMargin: -4
        color: "#f0f0f0"//"#eee"
    }

    Item {
        id: mst_Channel
        width: 80
        anchors.top: parent.top
        anchors.bottom: ldr_BottomSpacer.top


        Button {
            id: btn_From
            anchors.top: parent.top
            anchors.topMargin: 1
            x: 1
            height: 20
            width: 78
            text: csName


            background: Rectangle {
                color: StyleDefault.csColors[csColor].bgColor
                opacity: enabled ? 1 : 0.3
            }


            contentItem: Text {
                text: btn_From.text
                color: StyleDefault.csColors[csColor].textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }



            onPressed: {
                if (!cmb_From.visible) {

                    var mdl_Source = csModel.getJackOutputPorts()
                    var mdl_Sorted = portsModel.createObject(this);

                    for (var i = 0; i < mdl_Source.length; i++)
                        mdl_Sorted.append(  {"portName": mdl_Source[i]}  )

                    cmb_From.model = mdl_Sorted
                    cmb_From.open()
                }
            }

            Component {  id: portsModel; ListModel { }  }

            PortsSelector {
                id: cmb_From
                centerX: 41
                text: csName

                onS_ConnectPort: {

                    if (connect)
                        csModel.connectFrom(index, port, side)

                    else
                        csModel.disconnectFrom(index, port, side)

                }
            }
        }


        Text {
            anchors.top: btn_From.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            text: "+"
        }

        Controls.Knob {
            id: knb_Pan
            anchors.bottom: sld_Volume.top

            value: csPan
            onPositionChanged: {
                if (csPan !== knb_Pan.position)
                    csModel.setPan(index, knb_Pan.position)
            }

        }

        Controls.Fader {
            id: sld_Volume
            anchors.bottom: rowLayout1.top

            value: csVolume

            onPositionChanged: {
                if (csVolume !== sld_Volume.position)
                    csModel.setVolume(index, sld_Volume.position)
            }
        }

        Row {
            id: rowLayout1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 4
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 2

            Controls.MuteButton {
                text: "M"
                onClicked: {
                    if (csMute !== this.checked)
                        csModel.setMute(index, this.checked)

                }

            }
            Controls.SoloButton {
                text: "S"
            }
        }

    }

    Loader {
        id: ldr_BottomSpacer
        anchors.bottom: parent.bottom

        sourceComponent: spc_Parents
    }

    Component {
        id: spc_Parents
        Item {
            id: rct_parentColors
            height: (8 * csLevel)

            Repeater {
                model: csParentColors.toString().split(",")

                Rectangle {
                    y: (8 * index)
                    width: (index === 0) ? 80 : 81
                    height: 6

                    color: StyleDefault.csColors[ modelData ].bgColor
                }
            }
        }
    }
}
