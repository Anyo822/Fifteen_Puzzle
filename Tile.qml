import QtQuick 2.0

Rectangle {
    id: root
    property alias internalText: textValue
    color: "violet"
    border.color: Qt.darker("purple")
    border.width: 1

    radius: 10

    Text {
        id: textValue
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        text: qsTr("1")
        font {
            pointSize: Math.min(root.width, root.height) / 3
            bold: true
        }
    }
}
