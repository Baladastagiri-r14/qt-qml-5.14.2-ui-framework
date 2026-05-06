

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
Window {
    width: 1400
    height: 900
    visible: true
    title: "ESP32-CAM Server"

    Image {
        id: cameraView
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        source: "image://live/frame?" + Date.now()

        Timer {
            interval: 33
            running: true
            repeat: true
            onTriggered: cameraView.source = "image://live/frame?" + Date.now()
        }
    }


    Column {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10
        padding: 10

        Button {
            text: "Send Ping"
            onClicked: {
                tcpServer.sendDataToClient("PING")
                console.log("Sent ping to client")
            }
        }

        Button {
            text: "Toggle LED"
            onClicked: {
                tcpServer.sendDataToClient(JSON.stringify({
                                                              command: "toggle_led",
                                                              timestamp: Date.now()
                                                          }))
                console.log("Sent LED toggle command")
            }
        }
        Button {
            text: "Rectangle"
            onClicked: {
                let rectData = 200+ "," + 25 + "," + 150 + "," + 150;
                tcpServer.sendDataToClient(rectData);
            }
        }
        Text {
            id: connectionStatus
            text: "✖ No Clients"
            color: "red"
            font.pixelSize: 16
        }

        Connections {
            target: tcpServer
            function onClientConnectedChanged(connected) {
                connectionStatus.text = connected ? "✔ Client Connected" : "✖ No Clients"
                connectionStatus.color = connected ? "green" : "red"
            }
        }
    }
}
