import QtQuick 2.0

Text {
    property bool small: false
    width: implicitWidth
    color: "white"
    font.pixelSize: (small ? 10 : 13) * app.factor
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    textFormat: Text.RichText;
    wrapMode: Text.WordWrap
    onLinkActivated: link => Qt.openUrlExternally(link)
}
