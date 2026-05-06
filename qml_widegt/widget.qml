import QtQuick 2.0
import QtQuick.Controls 2.12


Item {
    width: 300
    height: 300

    Rectangle {
        x: 0
        y: 0
        color: "#000000"
        width: parent.width
        height: parent.height

        BusyIndicator {
            id: busyIndicator
            x: 111
            y: 89
        }
    }
}
