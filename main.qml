import QtQuick 2.14
import QtQuick.Window 2.14

Window {
    id: root
    visible: true
    width: 480
    height: 480

    GameBoard {
        anchors.fill: parent
    }
}
