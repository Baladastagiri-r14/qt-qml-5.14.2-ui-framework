import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12
import QtQuick.Shapes 1.12

ApplicationWindow {
    id: mainWindow
    width: 1400
    height: 900
    visible: true
    title: "ESP32-CAM Stream - FPS: " + (camStream ? camStream.fps : 0)
     property bool fovSetActive: false

    Component.onCompleted: {
        if (camStream) camStream.running = true;
    }

    Rectangle {
        anchors.fill: parent
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

            Rectangle {
                id: centerDot
                width: 10
                height: 10
                radius: 5
                color: "red"
                z: 10
                visible: camStream && camStream.running

                // Center relative to the painted (scaled) image
                x: videoOutput.x + (videoOutput.width - videoOutput.paintedWidth) / 2 + videoOutput.paintedWidth / 2 - width / 2
                y: videoOutput.y + (videoOutput.height - videoOutput.paintedHeight) / 2 + videoOutput.paintedHeight / 2 - height / 2
            }

            Rectangle {
                id: overlayRect
                visible: false  // Start hidden
                z: 5
                color: "transparent"
                border.color: "red"
                border.width: 2
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

            }
        }

        property int cropLeft: 250
        property int cropRight: 250
        property int cropTop: 406
        property int cropBottom: 530

        Rectangle {
            id: cropRegion
            visible: false
            width: parent.cropLeft + parent.cropRight
            height: parent.cropTop + parent.cropBottom
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            clip: true
            color: "transparent"
            border.color: "white"
            border.width: 1

            Image {
                id: crop_videoOutput
                visible: false
                width: parent.parent.width
                height: parent.parent.height
                x: -cropRegion.x
                y: -cropRegion.y
                fillMode: Image.PreserveAspectFit
                source: "image://live/cam?t=" + updateCounter
                cache: false
                asynchronous: false

                property int updateCounter: 0
                Connections {
                    target: camStream
                    onFrameChanged: {
                        crop_videoOutput.updateCounter++;
                        crop_videoOutput.source = "";
                        crop_videoOutput.source = "image://live/cam?t=" + crop_videoOutput.updateCounter;
                    }
                }
                Rectangle {
                    id: pixel_rect
                    visible: true  // Must be visible to see it
                    z: 5
                    color: "transparent"
                    border.color: "red"
                    border.width: 2
                    width: 250
                    height: 250

                    // Center it inside the parent (crop_videoOutput)
                    x: (parent.width - width) / 2
                    y: (parent.height - height) / 2

                    // Dragging variables
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
                            onPressed: pixel_rect.dragStartX = mouse.x
                            onPositionChanged: {
                                let delta = mouse.x - pixel_rect.dragStartX
                                pixel_rect.x += delta
                                pixel_rect.width -= delta
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
                            onPressed: pixel_rect.dragStartX = mouse.x
                            onPositionChanged: {
                                let delta = mouse.x - pixel_rect.dragStartX
                                pixel_rect.width += delta
                                pixel_rect.dragStartX = mouse.x
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
                            onPressed: pixel_rect.dragStartY = mouse.y
                            onPositionChanged: {
                                let delta = mouse.y - pixel_rect.dragStartY
                                pixel_rect.y += delta
                                pixel_rect.height -= delta
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
                            onPressed: pixel_rect.dragStartY = mouse.y
                            onPositionChanged: {
                                let delta = mouse.y - pixel_rect.dragStartY
                                pixel_rect.height += delta
                                pixel_rect.dragStartY = mouse.y
                            }
                        }
                    }
                }
            }
        }


        Rectangle {
            id: bottomPanel
            width: parent.width
            height: parent.height * 0.10  // 10% of the height
            anchors.bottom: parent.bottom
            color: "#222222"

            Row {
                anchors.centerIn: parent
                spacing: 40

                Button {
                    text: "SetFOV"
                    onClicked: {
                        // Calculate center of painted video
                        const centerX = videoOutput.x + (videoOutput.width - videoOutput.paintedWidth) / 2 + videoOutput.paintedWidth / 2;
                        const centerY = videoOutput.y + (videoOutput.height - videoOutput.paintedHeight) / 2 + videoOutput.paintedHeight / 2;

                        // Position overlayRect centered on the video
                        overlayRect.x = centerX - overlayRect.width / 2;
                        overlayRect.y = centerY - overlayRect.height / 2;

                        overlayRect.visible = true;
                         fovSetActive = true;
                    }
                }

                Button {
                    text: "Crop"
                    onClicked: {
                        console.log("Crop clicked");
                        if (camStream) camStream.running = false;  // pause stream to switch cleanly

                        videoOutput.visible = false;                // hide full video output

                        cropRegion.visible = true;
                        crop_videoOutput.visible = true;
                        pixel_rect.visible = true
                        if (camStream) camStream.running = true;   // resume streaming (cropped)

                        console.log("cropRegion visible: " + cropRegion.visible);
                                                console.log("cropRegion geometry: x=" + cropRegion.x + ", y=" + cropRegion.y + ", w=" + cropRegion.width + ", h=" + cropRegion.height);

                    }
                }



                Button {
                    text: "Settings"
                    onClicked: {
                        console.log("Settings clicked");
                    }
                }
                Button {
                    visible: fovSetActive
                    text: "Save"
                    onClicked: {
                        console.log("Save clicked - implement your save logic here");
                        let centerX = centerDot.x + centerDot.width / 2;
                               let centerY = centerDot.y + centerDot.height / 2;

                               // overlayRect edges
                               let left = overlayRect.x;
                               let right = overlayRect.x + overlayRect.width;
                               let top = overlayRect.y;
                               let bottom = overlayRect.y + overlayRect.height;

                               // Distances (positive values)
                               let distLeft = centerX - left;
                               let distRight = right - centerX;
                               let distTop = centerY - top;
                               let distBottom = bottom - centerY;

                               console.log("Distances from center to overlayRect edges:");
                               console.log("Left: " + distLeft);
                               console.log("Right: " + distRight);
                               console.log("Top: " + distTop);
                               console.log("Bottom: " + distBottom);

                        fovSetActive = false;
                    }
                }
            }
        }
    }
}
