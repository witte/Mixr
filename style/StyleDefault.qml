pragma Singleton
import QtQuick 2.7

QtObject {
    property int textSize: 20
    property color textColor: "green"

    property var csColors: [
        { bgColor: "#3892e0", textColor: "#fff" },
        { bgColor: "#da4d45", textColor: "#fff" },
        { bgColor: "#f37329", textColor: "#fff" },
        { bgColor: "#fbd25d", textColor: "#333333" },
        { bgColor: "#93d844", textColor: "#333333" },
        { bgColor: "#8a4ebf", textColor: "#fff" },
        { bgColor: "#ffffff", textColor: "#333333" },
        { bgColor: "#444", textColor: "#fff" }
    ]
//    property QtObject ChannelStripColors: [ "#3892e0", "#da4d45", "#f37329", "#fbd25d", "#93d844", "#8a4ebf", "#ffffff", "#333333" ]
//    property QtObject ChannelStripFontColors: [ "#fff", "#fff", "#fff", "#333333", "#333333", "#8a4ebf", "#ffffff", "#333333" ]
}
