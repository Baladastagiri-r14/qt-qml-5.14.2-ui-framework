import QtQuick 2.0
import QtQuick.Controls 2.15
Item {
    width: 200
    height: 100

    Rectangle {
        color: "lightblue"
        width: parent.width
        height: parent.height

        Text {
            anchors.centerIn: parent
            text: "Hello from QML!"
            font.pixelSize: 16
        }
    }
}
