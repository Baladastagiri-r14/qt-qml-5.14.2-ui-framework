import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.0

Button {
    id: button6
    width: 100
    height: 16
    text: "Click me"
    anchors.verticalCenterOffset: -459
    anchors.horizontalCenterOffset: 0
    anchors.centerIn: parent
    onClicked: {
        popup.x = -popup.width // Start position off the screen
        popup.open()
        popupAnimation.start()
    }
}
