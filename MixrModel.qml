import QtQuick 2.0

ListModel {
    ListElement {
                                    sName: "Master";     vol: 0.8; pan: 0.0; sChildren: [
            ListElement {           sName: "Drums";      vol: 1.0; pan: 0.0; sChildren: [
                ListElement {       sName: "Kick";       vol: 1.0; pan: 0.0; sChildren: [] },
                ListElement {       sName: "Snare";      vol: 1.2; pan: 0.2; sChildren: [
                    ListElement {   sName: "Top";        vol: 1.0; pan: 0.5; sChildren: [] },
                    ListElement {   sName: "Bottom";     vol: 1.0; pan: 0.5; sChildren: [] } ]},
                ListElement {       sName: "Hat";        vol: 1.0; pan: 0.3; sChildren: [] },
                ListElement {       sName: "Tom";        vol: 1.0; pan: 0.6; sChildren: [] },
                ListElement {       sName: "Floor Tom";  vol: 1.0; pan: 0.6; sChildren: [] },
                ListElement {       sName: "OH";         vol: 0.4; pan: 0.5; sChildren: [] },
                ListElement {       sName: "Room";       vol: 0.2; pan: 0.5; sChildren: [] } ]
            } // Drums
        ] // Master children
    }
    ListElement { sName: "Monitor 02";     vol: 0.8; pan: 0.0; }
}
