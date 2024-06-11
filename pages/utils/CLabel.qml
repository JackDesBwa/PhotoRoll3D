import QtQuick 2.0

PLabel {
    signal clicked(var mouse)
    signal pressAndHold(var mouse)
    MouseArea{
        anchors.fill: parent
        onClicked: mouse => parent.clicked(mouse)
        onPressAndHold: mouse => parent.pressAndHold(mouse)
    }
}
