import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.3
import QtQuick.Shapes 1.1
import QtGraphicalEffects 1.0
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4
import QtQml 2.2
import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Window 2.12




Item {
    width: 50
    height: 50

    BusyIndicator {
        running: true
        anchors.centerIn: parent
    }




    //    ListView {
    //            id: listView
    //            width: parent.width
    //            height: parent.height
    //            model: 3 // Number of options in the list
    //            currentIndex: -1 // Start with no selection

    //            delegate: Item {
    //                width: parent.width
    //                height: 30

    //                RadioButton {
    //                    anchors.verticalCenter: parent.verticalCenter
    //                    checked: index === listView.currentIndex
    //                    text: "Option " + (index + 1)
    //                    onClicked: {
    //                        if (index !== listView.currentIndex) {
    //                            listView.currentIndex = index;
    //                        }
    //                    }
    //                }
    //            }
    //        }


    Button{
        id :butt3
        x:350
        y:500
        text: qsTr("progress")
        height: 20
        width :90
        onClicked: {
            qmlButtonClicked4();
            progressBar.value = 0; // Reset progress bar
            progressBar.visible = true;
            //            progressBarAnimation.running = true; // Start animation
        }
    }



    ProgressBar {
        id: progressBar
        x: 750
        y: 500
        width: 300
        height: 10
        indeterminate: true
        value: 0.5
        visible: false

        background: Rectangle {
            radius: 5
            color: "lightblue"
            border.color: "gray"
            border.width: 1
            implicitWidth: 200
            implicitHeight: 25
        }



        //        background: Rectangle {
        //               implicitWidth: 200
        //               implicitHeight: 6
        //               border.color: "#999999"
        //               radius: 5
        //           }
        //           contentItem: Item {
        //               implicitWidth: 200
        //               implicitHeight: 4

        //               Rectangle {
        //                   id: bar
        //                   width: progressBar.visualPosition * parent.width
        //                   height: parent.height
        //                   radius: 5
        //               }

        //               LinearGradient {
        //                   anchors.fill: bar
        //                   start: Qt.point(0, 0)
        //                   end: Qt.point(bar.width, 0)
        //                   source: bar
        //                   gradient: Gradient {
        //                       GradientStop { position: 0.0; color: "#17a81a" }
        //                       GradientStop { id: grad; position: 0.5; color: Qt.lighter("#17a81a", 2) }
        //                       GradientStop { position: 1.0; color: "#17a81a" }
        //                   }
        //                   PropertyAnimation {
        //                       target: grad
        //                       property: "position"
        //                       from: 0.1
        //                       to: 0.9
        //                       duration: 1000
        //                       running: true
        //                       loops: Animation.Infinite
        //                   }
        //               }
        //               LinearGradient {
        //                   anchors.fill: bar
        //                   start: Qt.point(0, 0)
        //                   end: Qt.point(0, bar.height)
        //                   source: bar
        //                   gradient: Gradient {
        //                       GradientStop { position: 0.0; color: Qt.rgba(0,0,0,0) }
        //                       GradientStop { position: 0.5; color: Qt.rgba(1,1,1,0.3) }
        //                       GradientStop { position: 1.0; color: Qt.rgba(0,0,0,0.05) }
        //                   }
        //               }
        //           }
        //           PropertyAnimation {
        //               target: progressBar
        //               property: "value"
        //               from: 0
        //               to: 1
        //               duration: 5000
        //               running: true
        //               loops: Animation.Infinite
        //           }
    }

    function qmlButtonClicked4() {
        console.log("QML Button clicked4")
    }

    function qmlButtonClicked5() {
        console.log("QML Button clicked5")
    }

    function qmlButtonClicked6() {
        console.log("QML Button clicked6")
    }
    function qmlButtonClicked7() {
        console.log("QML Button clicked7")
    }



    Button {
        id: butt6
        x:650
        y:500
        width: 90
        height: 20
        text: qsTr("D_pro")


        onClicked: {
            qmlButtonClicked7()
            shape.visible =true;
        }
    }
    Shape {
        id: shape
        x: 1150
        y: 325
        width: 200
        height: 200
        visible: false

        property real progressAngle: 0 // Define the progressAngle property here

        // Enable multisampled rendering
        layer.enabled: true
        layer.samples: 4

        ShapePath {
            id: grayArc
            fillColor: "transparent"
            strokeColor: "gray"
            strokeWidth: 20
            capStyle: ShapePath.RoundCap
            PathAngleArc {
                centerX: 100; centerY: 100
                radiusX: 100 - 20 / 2; radiusY: 100 - 20 / 2
                startAngle: 135
                sweepAngle: 270 // Use progressAngle here

            }
        }

        ShapePath {
            id: blueArc
            fillColor: "transparent"
            strokeColor: "lightBlue"
            strokeWidth: 20
            capStyle: ShapePath.RoundCap
            PathAngleArc {
                centerX: 100; centerY: 100
                radiusX: 100 - 20 / 2; radiusY: 100 - 20 / 2
                startAngle: 135
                sweepAngle: shape.progressAngle  // Use progressAngle here
            }
        }

        Timer {
            id: progressTimer // Define the progressTimer here
            interval: 100 // Adjust interval as needed for smoother animation
            running: false
            repeat: true
            onTriggered: {
                if (shape.progressAngle < 180) { // Adjust the maximum angle as needed
                    shape.progressAngle += 1; // Increase the progress angle
                } else {
                    progressTimer.stop(); // Stop the timer when reaching the desired angle
                }
            }
        }

        Component.onCompleted: {
            progressTimer.start(); // Start the timer when the component is completed
        }
    }
    Button {
        id: butt5
        x:550
        y:500
        width: 90
        height: 20
        text: qsTr("dial")


        onClicked: {
            qmlButtonClicked6()
            dialer.visible =true;
        }
    }

    Dial{
        id:dialer
        x:750
        y:325
        width:100
        height:100
        palette.dark: "lightblue"
        visible:false

    }


    Button {
        id: butt4
        x:450
        y:500
        width: 90
        height: 20
        text: qsTr("R.slider")


        onClicked: {
            qmlButtonClicked5();
            sli.visible = true;
        }
    }


    RangeSlider {
        id: sli
        x: 750
        y: 475
        width: 300
        height: 10
        from: 0
        to: 100
        first.value: 15
        second.value: 85
        visible: false

        Rectangle {
            anchors.left: sli.first.handle.right
            anchors.right: sli.second.handle.left
            anchors.verticalCenter: sli.verticalCenter
            width: 10 // Adjust the width here
            height: 8
            color: "lightblue"

            clip: true // Clip the handle to the slider bounds

//            first.handle: Rectangle {
//                   x: control.leftPadding + control.first.visualPosition * (control.availableWidth - width)
//                   y: control.topPadding + control.availableHeight / 2 - height / 2
//                   implicitWidth: 26
//                   implicitHeight: 26
//                   radius: 13
//                   color: control.first.pressed ? "#f0f0f0" : "#f6f6f6"
//                   border.color: "#bdbebf"
//               }

//               second.handle: Rectangle {
//                   x: control.leftPadding + control.second.visualPosition * (control.availableWidth - width)
//                   y: control.topPadding + control.availableHeight / 2 - height / 2
//                   implicitWidth: 26
//                   implicitHeight: 26
//                   radius: 13
//                   color: control.second.pressed ? "#f0f0f0" : "#f6f6f6"
//                   border.color: "#bdbebf"
//               }

            Rectangle {
                id: firstHandle
                implicitWidth: parent.width
                implicitHeight: parent.height
                radius: 7
                color: sli.first.pressed ? "black" : "lightblue"
                border.color: "yellow"
            }

//            Rectangle {
//                id: secondtHandle
//                implicitWidth: parent.width
//                implicitHeight: parent.height
//                radius: 7
//                color: sli.second.pressed ? "black" : "lightblue"
//                border.color: "yellow"
//            }

            MouseArea {
                anchors.fill: parent
                property real dx: 0
                onPressed: dx = mouseX
                onPositionChanged: {
                    var d = ((mouseX - dx) / parent.width) * Math.abs(sli.to - sli.from)
                    if ((d + sli.first.value) < sli.from) d = sli.from - sli.first.value
                    if ((d + sli.second.value) > sli.to) d = sli.to - sli.second.value
                    sli.first.value += d
                    sli.second.value += d
                    dx = mouseX
                }
            }
        }

        Text {
            text: sli.first.value.toFixed(2) + " - " + sli.second.value.toFixed(2)
            font.pixelSize: 12
            color: "red"
            x: 320
            y:-2
        }
    }

    Button {
        id: butt2
        x:250
        y:500
        width: 90
        height: 20
        text: qsTr("slider")


        onClicked: {
            qmlButtonClicked2();
            slider.visible = true;
            slider.value = 1;
        }
    }

    Slider {
          id: slider
          x: 750
          y: 450
          width: 300
          height: 10
          from: 0
          to: 100
          stepSize: 1
          visible: false
          background: Rectangle {
                  x: slider.leftPadding
                  y: slider.topPadding + slider.availableHeight / 2 - height / 2
                  implicitWidth: 200
                  implicitHeight: 4
                  width: slider.availableWidth
                  height: implicitHeight
                  radius: 2
                  color: "gray"

                  Rectangle {
                      width: slider.visualPosition * parent.width
                      height: parent.height
                      color: "lightblue"
                      radius: 2
                  }
              }

              handle: Rectangle {
                  x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
                  y: slider.topPadding + slider.availableHeight / 2 - height / 2
                  implicitWidth: 15
                  implicitHeight: 15
                  radius: 7
                  color: slider.pressed ? "Black" : "lightblue"
                  border.color: "yellow"
              }
              Text {
                      text: slider.value.toFixed()
                      font.pixelSize: 12
                      color: "red"
                      x: 320
                      y:-2
                  }
      }

//    Slider {
//        id: temperatureControlSlider
//        x: 750
//        y: 450
//        width: 300
//        height: 10
//        from: 0
//        to: 100
//        visible: false
//        stepSize: 1
//        orientation: Qt.Horizontal
//        Rectangle {
//            color: "transparent"
//            border.color: "black"
//            gradient: Gradient {
//                GradientStop { position: 0.0; color: temperatureControlSlider.pressed ? "lightblue" : "blue" }
//                GradientStop { position: 1.0; color: temperatureControlSlider.pressed ? "blue" : "lightblue" }
//            }
//        }
//        background: Rectangle {
//            implicitWidth:400
//            implicitHeight: 6
//            border.color: "transparent"
//            radius: height / 2
//            gradient: Gradient {
//                GradientStop { position: 0.0; color: "lightblue" }
//                GradientStop { position: temperatureControlSlider.visualPosition /*/ slider.width*/; color: "blue" }
//                GradientStop { position: temperatureControlSlider.visualPosition /*/ slider.width*/; color: "lightblue" }
//                GradientStop { position: 1.0; color: "lightblue" }

//            }
//        }
//    }

    function qmlButtonClicked2() {
        console.log("QML Button clicked3")
    }

    Button {
        id: butt1
        x:50
        y:500
        width: 90
        height: 20
        text: qsTr("message")


        onClicked: {
            qmlButtonClicked();
            messageDialog.visible = true
            messageDialog.x = messageDialog.width
            messageDialog.y = (1200 - messageDialog.height) / 2
            leftToCenterAnimation.start()
            colorAnimation.start()

        }


    }



    Dialog {
        id: messageDialog
        visible: false

        ColumnLayout {
            anchors.fill: parent
            Rectangle {
                id: dialogRect
                width: 200
                height: 60
                color: "lightgray"
                radius: 10

                Text {
                    text: "Hello, this is a message dialog!"
                    anchors.centerIn: parent
                    wrapMode: Text.WordWrap
                }
            }
        }


        SequentialAnimation {
            id: leftToCenterAnimation

            NumberAnimation {
                target: messageDialog
                property: "x"
                from: 1000
                to: (1000- messageDialog.width) /2
                duration: 1000
            }
        }
        ColorAnimation {
            id: colorAnimation
            target: dialogRect
            property: "color"
            from: "gray"
            to: "lightblue"
            duration: 1000
        }
    }
    function qmlButtonClicked() {
        console.log("QML Button clicked");

    }

    function qmlButtonClicked1() {
        console.log("QML Button clicked");
    }
    Button{
        id :butt
        x:150
        y:500
        text: qsTr("popup")
        height: 20
        width :90

        onClicked: {
            qmlButtonClicked1()
            popup.x = popup.width
            popup.y = (600 - popup.height) / 2
            popup.visible = true
            popupAnimation.start()
        }
    }

    Popup {
        id: popup
        modal: true
        closePolicy: Popup.CloseOnPressOutside
        contentItem: Rectangle {
            width: 200
            height: 60
            color: "lightgray"
            radius: 30
            visible: true

            Text {
                text: "Hello, this is a popup message!"
                anchors.centerIn: parent
            }
        }

        NumberAnimation {
            id: popupAnimation
            target: popup
            property: "x"
            from: 1000
            to: (1000 - popup.width) / 2
            duration: 1000
            easing.type: Easing.OutBack
        }
    }

}
