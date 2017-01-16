import QtQuick 2.7
import QtQuick.Controls 2.0

Slider {
    anchors.horizontalCenter: parent.horizontalCenter

    orientation: Qt.Vertical
    height: 120
    handle.scale: 0.55

    value: 1
    wheelEnabled: true
}
