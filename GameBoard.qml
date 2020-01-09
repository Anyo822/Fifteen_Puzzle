import QtQuick 2.0
import Game 1.0

GridView {
    id: root
    model: GameBoardModel {

    }

    cellHeight: height / root.model.dimension
    cellWidth: width / root.model.dimension

    delegate: Item {
        id: _background
        width: root.cellWidth
        height: root.cellHeight

        visible: root.model.hiddenElementValue != display

        Behavior on x {
            NumberAnimation {duration: 4000}
        }

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
}
