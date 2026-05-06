import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import QtQuick.Shapes 1.12

ApplicationWindow {
    id: mainWindow
    width: 1400  // Can be changed to 1400
    height: 900 // Can be changed to 900
    visible: true
    title: "ESP32-CAM Stream - FPS: " + (camStream ? camStream.fps : 0)

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Left control panel (30%)
        Rectangle {
            Layout.preferredWidth: parent.width * 0.3
            Layout.fillHeight: true
            color: "#2d2d2d"
            ScrollView {
                anchors.fill: parent
                anchors.margins: 10
                clip: true
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

                ColumnLayout {
                    width: parent.width
                    spacing: 15

                    GroupBox {
                        title: "Stream Control"
                        Layout.fillWidth: true
                        background: Rectangle { color: "#3d3d3d"; radius: 5 }

                        ColumnLayout {
                            width: parent.width
                            spacing: 10

                            Button {
                                text: camStream && camStream.running ? "■ Stop Stream" : "▶ Start Stream"
                                Layout.fillWidth: true
                                onClicked: {
                                    if (camStream) camStream.running = !camStream.running

                                }

                            }

                            TextField {
                                Layout.fillWidth: true
                                placeholderText: "Stream URL"
                                text: camStream ? camStream.streamUrl : ""
                                onEditingFinished: if (camStream) camStream.streamUrl = text
                                color: "white"
                                background: Rectangle {
                                    color: "#404040"
                                    radius: 5
                                }
                            }

                            Label {
                                text: "Status: " + (camStream && camStream.running ? "● Streaming" : "○ Stopped")
                                color: camStream && camStream.running ? "lightgreen" : "red"
                                font.bold: true
                            }

                            Label {
                                text: "FPS: " + (camStream ? camStream.fps : 0)
                                color: "white"
                                font.bold: true
                            }

                            Button {
                                text: "Get Video Size"
                                Layout.fillWidth: true
                                onClicked: {
                                    console.log("Video Size:");
                                    console.log("Width: " + videoOutput.videoRect.width.toFixed(2) + " px");
                                    console.log("Height: " + videoOutput.videoRect.height.toFixed(2) + " px");
                                }
                            }

                            Button {
                                text: "Distance to center"
                                Layout.fillWidth: true
                                onClicked: {
                                    // Center of the video (dot)
                                    var dotX = videoOutput.videoRect.x + videoOutput.videoRect.width / 2;
                                    var dotY = videoOutput.videoRect.y + videoOutput.videoRect.height / 2;

                                    // Overlay edges
                                    var top = overlayRect.y;
                                    var bottom = overlayRect.y + overlayRect.height;
                                    var left = overlayRect.x;
                                    var right = overlayRect.x + overlayRect.width;

                                    // Distances from dot
                                    var distTop = dotY - top;
                                    var distBottom = bottom - dotY;
                                    var distLeft = dotX - left;
                                    var distRight = right - dotX;

                                    console.log("Distance from dot to overlay edges:");
                                    console.log("Top: " + distTop.toFixed(2));
                                    console.log("Bottom: " + distBottom.toFixed(2));
                                    console.log("Left: " + distLeft.toFixed(2));
                                    console.log("Right: " + distRight.toFixed(2));

                                    innerRect.visible = true

                                    // Center the innerRect inside overlayRect
                                            innerRect.width = 100
                                            innerRect.height = 150
                                            innerRect.x = (overlayRect.width - innerRect.width) / 2
                                            innerRect.y = (overlayRect.height - innerRect.height) / 2
                                            innerRect.visible = true
                                }
                            }

                            Button {
                                text: " Inner Rectangle pixels"
                                Layout.fillWidth: true
                                onClicked: {
                                    if (!innerRect.visible) {
                                        console.log("Inner rectangle is not visible.");
                                        return;
                                    }

                                    let x = overlayRect.x + innerRect.x;
                                    let y = overlayRect.y + innerRect.y;
                                    let w = innerRect.width;
                                    let h = innerRect.height;

                                    console.log("Inner Rectangle Info:");
                                    console.log("Top-Left: (" + x.toFixed(2) + ", " + y.toFixed(2) + ")");
                                    console.log("Bottom-Right: (" + (x + w).toFixed(2) + ", " + (y + h).toFixed(2) + ")");
                                    console.log("Width: " + w.toFixed(2) + " px");
                                    console.log("Height: " + h.toFixed(2) + " px");
                                }
                            }
                        }
                    }

                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "black"



            Image {
                id: videoOutput
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "image://live/cam?t=" + updateCounter
                cache: false
                asynchronous: false

                property int updateCounter: 0
                Connections {
                    target: camStream
                    onFrameChanged: {
                        videoOutput.updateCounter++;
                        videoOutput.source = "";
                        videoOutput.source = "image://live/cam?t=" + videoOutput.updateCounter;
                    }
                }
                property rect videoRect: {
                    if (!sourceSize.width || !sourceSize.height)
                        return Qt.rect(0, 0, width, height);

                    var ratio = sourceSize.width / sourceSize.height;
                    var containerRatio = width / height;

                    if (containerRatio > ratio) {
                        var actualWidth = height * ratio;
                        return Qt.rect((width - actualWidth) / 2, 0, actualWidth, height);
                    } else {
                        var actualHeight = width / ratio;
                        return Qt.rect(0, (height - actualHeight) / 2, width, actualHeight);
                    }
                }
                Rectangle {
                    id: centerDot
                    width: 10
                    height: 10
                    radius: 5
                    color: "red"
                    z: 10
                    visible: camStream && camStream.running

                    // Center it relative to the video image rectangle
                    x: videoOutput.videoRect.x + videoOutput.videoRect.width / 2 - width / 2
                    y: videoOutput.videoRect.y + videoOutput.videoRect.height / 2 - height / 2

                }
                Rectangle {
                    id: overlayRect
                    visible: camStream && camStream.running
                    z: 5
                    color: "transparent"
                    border.color: "red"
                    border.width: 2
                    x: videoOutput.videoRect.x + (videoOutput.videoRect.width - width) / 2
                    y: videoOutput.videoRect.y + (videoOutput.videoRect.height - height) / 2
                    width: 250
                    height: 250

                    // Common dragging state variables
                    property real dragStartX
                    property real dragStartY

                    // LEFT EDGE
                    Rectangle {
                        width: 10; height: parent.height
                        x: -5; y: 0
                        color: "transparent"
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.SizeHorCursor
                            onPressed: overlayRect.dragStartX = mouse.x
                            onPositionChanged: {
                                let delta = mouse.x - overlayRect.dragStartX
                                overlayRect.x += delta
                                overlayRect.width -= delta
                            }
                        }
                    }

                    // RIGHT EDGE
                    Rectangle {
                        width: 10; height: parent.height
                        x: parent.width - 5; y: 0
                        color: "transparent"
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.SizeHorCursor
                            onPressed: overlayRect.dragStartX = mouse.x
                            onPositionChanged: {
                                let delta = mouse.x - overlayRect.dragStartX
                                overlayRect.width += delta
                                overlayRect.dragStartX = mouse.x
                            }
                        }
                    }

                    // TOP EDGE
                    Rectangle {
                        width: parent.width; height: 10
                        x: 0; y: -5
                        color: "transparent"
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.SizeVerCursor
                            onPressed: overlayRect.dragStartY = mouse.y
                            onPositionChanged: {
                                let delta = mouse.y - overlayRect.dragStartY
                                overlayRect.y += delta
                                overlayRect.height -= delta
                            }
                        }
                    }

                    // BOTTOM EDGE
                    Rectangle {
                        width: parent.width; height: 10
                        x: 0; y: parent.height - 5
                        color: "transparent"
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.SizeVerCursor
                            onPressed: overlayRect.dragStartY = mouse.y
                            onPositionChanged: {
                                let delta = mouse.y - overlayRect.dragStartY
                                overlayRect.height += delta
                                overlayRect.dragStartY = mouse.y
                            }
                        }
                    }



                   // Inner draggable & resizable rectangle (initially hidden)
                        Rectangle {
                            id: innerRect
                            visible: false
                            color: "transparent"
                            border.color: "blue"
                            border.width: 2
                            x: 20
                            y: 20
                            width: 100
                            height: 150
                            z: 10

                            property real dragStartX
                            property real dragStartY

                            // Dragging the inner rectangle inside overlayRect
                            MouseArea {
                                anchors.fill: parent
                                drag.target: parent
                                drag.axis: Drag.XAndYAxis

                                onPositionChanged: {
                                    // Clamp innerRect inside overlayRect boundaries
                                    if (innerRect.x < 0) innerRect.x = 0
                                    if (innerRect.y < 0) innerRect.y = 0
                                    if (innerRect.x + innerRect.width > overlayRect.width)
                                        innerRect.x = overlayRect.width - innerRect.width
                                    if (innerRect.y + innerRect.height > overlayRect.height)
                                        innerRect.y = overlayRect.height - innerRect.height
                                }
                            }

                            // Resizable edges similar to your overlayRect, but constrained inside overlayRect

                            // LEFT EDGE resize
                            Rectangle {
                                width: 10; height: parent.height
                                x: -5; y: 0
                                color: "transparent"
                                MouseArea {
                                    anchors.fill: parent
                                    cursorShape: Qt.SizeHorCursor
                                    onPressed: innerRect.dragStartX = mouse.x
                                    onPositionChanged: {
                                        let delta = mouse.x - innerRect.dragStartX
                                        let newX = innerRect.x + delta
                                        let newWidth = innerRect.width - delta

                                        // constrain within overlayRect
                                        if (newX < 0) return
                                        if (newWidth < 20) return  // minimal width

                                        innerRect.x = newX
                                        innerRect.width = newWidth
                                        innerRect.dragStartX = mouse.x
                                    }
                                }
                            }

                            // RIGHT EDGE resize
                            Rectangle {
                                width: 10; height: parent.height
                                x: parent.width - 5; y: 0
                                color: "transparent"
                                MouseArea {
                                    anchors.fill: parent
                                    cursorShape: Qt.SizeHorCursor
                                    onPressed: innerRect.dragStartX = mouse.x
                                    onPositionChanged: {
                                        let delta = mouse.x - innerRect.dragStartX
                                        let newWidth = innerRect.width + delta
                                        if (innerRect.x + newWidth > overlayRect.width) return
                                        if (newWidth < 20) return
                                        innerRect.width = newWidth
                                        innerRect.dragStartX = mouse.x
                                    }
                                }
                            }

                            // TOP EDGE resize
                            Rectangle {
                                width: parent.width; height: 10
                                x: 0; y: -5
                                color: "transparent"
                                MouseArea {
                                    anchors.fill: parent
                                    cursorShape: Qt.SizeVerCursor
                                    onPressed: innerRect.dragStartY = mouse.y
                                    onPositionChanged: {
                                        let delta = mouse.y - innerRect.dragStartY
                                        let newY = innerRect.y + delta
                                        let newHeight = innerRect.height - delta

                                        if (newY < 0) return
                                        if (newHeight < 20) return

                                        innerRect.y = newY
                                        innerRect.height = newHeight
                                        innerRect.dragStartY = mouse.y
                                    }
                                }
                            }

                            // BOTTOM EDGE resize
                            Rectangle {
                                width: parent.width; height: 10
                                x: 0; y: parent.height - 5
                                color: "transparent"
                                MouseArea {
                                    anchors.fill: parent
                                    cursorShape: Qt.SizeVerCursor
                                    onPressed: innerRect.dragStartY = mouse.y
                                    onPositionChanged: {
                                        let delta = mouse.y - innerRect.dragStartY
                                        let newHeight = innerRect.height + delta
                                        if (innerRect.y + newHeight > overlayRect.height) return
                                        if (newHeight < 20) return
                                        innerRect.height = newHeight
                                        innerRect.dragStartY = mouse.y
                                    }
                                }
                            }
                        }
                }

            }
        }

    }
}

