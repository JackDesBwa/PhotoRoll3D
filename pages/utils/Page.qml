import QtQuick 2.0

Rectangle {
    property Component view_left;
    property Component view_right;
    property Component view_stereo;
    property Component view_flat;
    property int pageWidth: width/2
    property int pageHeight: height
    color: "#333333"

    Item {
        id: left
        width: parent.width/2
        height: parent.height
        clip: true

        Loader {
            anchors.fill: parent
            sourceComponent: view_left
        }
        Loader {
            id: leftStereo
            property int direction_stereo: +1
            property alias other_side: rightStereo.item
            anchors.fill: parent
            sourceComponent: view_stereo
        }
        Loader {
            id: leftFlat
            anchors.fill: parent
            sourceComponent: view_flat
        }
    }
    Item {
        x: parent.width/2
        width: parent.width/2
        height: parent.height
        clip: true

        Loader {
            anchors.fill: parent
            sourceComponent: view_right
        }
        Loader {
            id: rightStereo
            anchors.fill: parent
            property int direction_stereo: -1
            property alias other_side: leftStereo.item
            sourceComponent: view_stereo
        }
        ShaderEffectSource {
            anchors.fill: parent
            sourceItem: leftFlat
        }
    }
}
