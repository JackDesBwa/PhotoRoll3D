import QtQuick
import QtQuick.Controls
import "utils"
import "utils/displaymodes.js" as DisplayModes

Page {
    id: page
    view_stereo: Item {
        property alias contentY: flickable.contentY
        Item {
            width: page.pageWidth * 2/3
            height: page.pageHeight
            Image {
                width: Math.min(parent.width * 0.8, implicitWidth * 1.5)
                height: width / implicitWidth * implicitHeight
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: direction_stereo
                source: "../images/logo" + (direction_stereo < 0 ? "_r" : "") + ".png"
            }
        }
        Flickable {
            id: flickable
            x: page.pageWidth * 2/3
            width: page.pageWidth/ 3
            height: page.pageHeight
            contentWidth: column.width
            contentHeight: column.height
            boundsBehavior: Flickable.OvershootBounds
            flickableDirection: Flickable.VerticalFlick
            topMargin: 25 * app.factor
            bottomMargin: 25 * app.factor
            contentY: other_side.contentY

            Column {
                id: column
                anchors.centerIn: parent
                Repeater {
                    model: DisplayModes.names
                    CLabel {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.horizontalCenterOffset: app.displayMode === index ? direction_stereo : 0
                        width: Math.max(implicitWidth, 260 * app.factor)
                        height: 25 * app.factor
                        text: modelData
                        font.bold: app.displayMode === index
                        onClicked: app.displayMode = index
                    }
                }
            }
        }
    }

    Keys.onPressed: event => {
        if ((event.key === Qt.Key_Up) || (event.key === Qt.Key_PageUp)) {
            DisplayModes.increment(app, -1);
        }
        if ((event.key === Qt.Key_Down) || (event.key === Qt.Key_PageDown)) {
            DisplayModes.increment(app, +1);
        }
    }
}
