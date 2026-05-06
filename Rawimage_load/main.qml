import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 800
    height: 640
    title: "Display Raw Image"

    // Image element to display QImage
    Image {
            id: img
            anchors.fill: parent
            source: "image://rawImage"
            fillMode: Image.PreserveAspectFit
            smooth:true
        }
    }
