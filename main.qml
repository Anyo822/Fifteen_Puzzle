import QtQuick 2.14
import QtQuick.Window 2.14

Window {
    id: root
    visible: true
    width: 480
    height: 480

    GameBoard {
        id: gameBoard
        anchors.fill: parent
        anchors.topMargin: 75
    }

    Rectangle {
        width: 100
        height: 50
        color: "violet"
        border.color: Qt.darker("purple")
        border.width: 1

        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            text: "SHUFFLE"
            anchors.centerIn: parent

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.error("!!!!!!!!!!! 1", gameBoard.model);
                    gameBoard.model.shuffle()
                }
            }
        }
    }
}
