import QtQuick 2.0
import Game 1.0

GridView {
    id: root

    cellHeight: height / root.model.dimension
    cellWidth: width / root.model.dimension

    model: GameBoardModel {}

    interactive: false

    delegate: Item {
        id: _background
        width: root.cellWidth
        height: root.cellHeight

        visible: root.model.hiddenElementValue !== display

        Tile {
            anchors.fill: _background
            anchors.margins: 1

            internalText.text: display.toString()
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.model.move(index);
                }
            }
        }
    }

    move: Transition {
        NumberAnimation {
            properties: "x,y"
            duration: 200
        }
    }
}
