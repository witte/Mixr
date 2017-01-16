import QtQuick 2.7
import QtQuick.Controls 2.0

ComboBox {
    id: control
    height: 18
    width: 80

    indicator: null

    hoverEnabled: true

    delegate: ItemDelegate {
        width: 300
        height: 22

        contentItem: Text {
            id: txt_PortName
            height: 18
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter

            text: control.model[index]
            font: control.font
        }

        highlighted: (cmb_To.highlightedIndex === index) || hovered
        hoverEnabled: true
    }

    popup: Popup {
        id: listPopup
        y: control.height + 2
        width: 300
        padding: 2
        background: Rectangle { color: "#f8f8f8" }
        implicitHeight: listview.contentHeight

        contentItem: ListView {
            id: listview
            clip: true

            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

    ToolTip.delay: 600
    ToolTip.visible: (hovered && !listPopup.visible)
    ToolTip.text: control.currentText
}
