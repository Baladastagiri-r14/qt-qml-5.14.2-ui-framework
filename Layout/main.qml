import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.3
import QtQuick.Shapes 1.1
import QtGraphicalEffects 1.0

import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4
//import QtQuick.Controls 1.4

Window {
    id:window
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    color: "gray"


     Button{
        id :butt
        x:200
        y:130
        text: qsTr("popup")
        height: 20
        width :140

        //        onClicked: {
        //                    popup.open()
        //                }
        //            }

        //    Popup {
        //            id: popup
        //            width: 200
        //            height: 100
        //            modal: true
        //            focus: true
        //            // Center the popup in the application window
        //            x: (parent.width - width) / 2
        //            y: (parent.height - height) / 2

        //            contentItem: Text {
        //                text: "Hello, this is a popup message!"
        //                wrapMode: Text.WordWrap
        //                width: parent.width - 20
        //                anchors.centerIn: parent
        //            }


        onClicked: {
            popup.x = popup.width
            popup.y = (parent.height - popup.height) / 2
            popup.visible = true
            popupAnimation.start()
        }
    }


    Popup {
        id: popup
        closePolicy: Popup.CloseOnPressOutside
        contentItem: Rectangle {
            width: 200
            height: 60
            color: "lightgray"
            radius: 30
            //                     width: parent.width
            //                     height: parent.height
            visible: true

            Text {
                text: "Hello, this is a popup message!"
                anchors.centerIn: parent
            }
        }



        //    Rectangle {
        //        id: popup
        //        width: 180
        //        height: 60
        //        color: "lightgray"
        //        radius: 30
        //        visible: false

        //        Text {
        //            text: "Hello, this is a popup message!"
        //            anchors.centerIn: parent
        //        }

        NumberAnimation {
            id: popupAnimation
            target: popup
            property: "x"
            from: window.width
            to: (window.width - popup.width) / 2
            duration: 1000
            easing.type: Easing.OutBack
        }
    }

    Button {
        id: butt1
        x: 368
        y: 129
        width: 140
        height: 20
        text: qsTr("message")


    //                onClicked: {
    //                    messageDialog.visible = true
    //                }
    //            }

    //            MessageDialog {
    //                id: messageDialog
    //                title: "Message"
    //                text: "Hello, this is a message box!"
    //                onAccepted: {
    //                    console.log("Accepted")
    //                }
    //                onRejected: {
    //                    console.log("Rejected")
    //                }
    //                visible: false // Initially invisible
    //            }

        onClicked: {
            messageDialog.visible = true
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
                from: window.width
                to: (window.width - messageDialog.width) /2
                duration: 1000
            }
        }
        ColorAnimation {
                id: colorAnimation
                target: dialogRect
                property: "color"
                from: "red"
                to: "green"
                duration: 1000
            }
    }

    Button {
        id: butt2
        x: 542
        y: 129
        width: 140
        height: 20
        text: qsTr("slider")
        onClicked: {
            temperatureControlSlider.visible = true;
            temperatureControlSlider.value = 1;
        }
    }

       Slider {
            id: temperatureControlSlider
            x: 750
            y: 135
            width: 300
            height: 10
            from: 0
            to: 100
            visible: false
            stepSize: 1
            orientation: Qt.Horizontal
//            style: SliderStyle {
//                        groove: Rectangle{
//                            id: grooveMainrect
//                            height: 5
//                            antialiasing: true
//                            radius: 5
//                            color: "#847878"
//                            Rectangle {
//                                id: groovechildrect
//                                width: (parent.width * theslider.value)/theslider.maximumValue
//                                height: parent.height
//                                color: Qt.lighter("#7ea6d6", 1.2)
//                                radius: parent.radius
//                            }
//                        }
//                        handle: Rectangle{
//                            id:handlemainrect
//                            height: (handlechildrect.height/3)+2
//                            width: (handlechildrect.width/3)+2
//                            radius: handlechildrect.width/2
//                            anchors.verticalCenter: parent.verticalCenter
//                            color: Qt.lighter("#7ea6d6", 1.2)
//                            Rectangle {
//                                id: handlechildrect
//                                width: 25
//                                height: 25
//                                anchors.centerIn: parent
//                                color: "#ffffff"
//                                radius: width/2
//                                opacity: 0.5
//                            }
//                        }
//                    }


//            SliderStyle {
//                groove: Item {
//                    anchors.verticalCenter:slider.verticalCenter
//                    implicitWidth: 500
//                    implicitHeight: 10
//                    Rectangle {
//                        radius: height/2
//                        anchors.fill: parent
//                        border.width: 1
//                        border.color: "#888"
//                        layer.enabled: true
//                        layer.effect: LinearGradient {
//                            start: Qt.point(0, 0)
//                            end: Qt.point(500, 0)
//                            gradient: Gradient {
//                                GradientStop { position: 0.0; color: "blue" }
//                                GradientStop { position: 1.0; color: "lightblue" }
//                            }
//                        }
//                    }
//                }
//            }

            Rectangle {
                color: "transparent"
                 border.color: "black"
                gradient: Gradient {
                    GradientStop { position: 0.0; color: slider.pressed ? "lightblue" : "blue" }
                    GradientStop { position: 1.0; color: slider.pressed ? "blue" : "lightblue" }
                }
            }
            background: Rectangle {
                implicitWidth:400
                implicitHeight: 6
                border.color: "transparent"
                radius: height / 2
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "lightblue" }
                    GradientStop { position: slider.visualPosition /*/ slider.width*/; color: "blue" }
                    GradientStop { position: slider.visualPosition /*/ slider.width*/; color: "lightblue" }
                    GradientStop { position: 1.0; color: "lightblue" }

                }
            }
    }

    GroupBox {
        id: groupBox1
        x: 200
        y: 155
        width: 1543
        height: 1000
        visible: true
        title: qsTr("Group Box")

        Grid {
            id: grid
            x: 43
            y: 0
            width: 1471
            height: 965
        }

        Image {
            id: image
            x: 8
            y: 0
            width: 800
            height: 450
            source: "file:///D:/dastagiri/png/Screenshot 2024-03-05 152435.png"
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: image1
            x: 801
            y: 0
            width: 800
            height: 450
            source: "file:///D:/dastagiri/png/Screenshot 2024-03-05 152643.png"
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: image2
            x: 5
            y: 496
            width: 800
            height: 450
            source: "file:///D:/dastagiri/png/Screenshot 2024-03-05 152945.png"
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: image3
            x: 801
            y: 496
            width: 800
            height: 450
            source: "file:///D:/dastagiri/png/Screenshot 2024-03-05 153005.png"
            fillMode: Image.PreserveAspectFit
        }
    }

    GroupBox {
        id: groupBox2
        x: 1758
        y: 150
        width: 145
        height: 1000
        title: qsTr("Group Box")

        ColumnLayout {
            x: 6
            y: 31

        }

        GroupBox {
            id: groupBox6
            x: 1
            y: 6
            width: 117
            height: 186
            title: qsTr("Group Box")

            RadioButton {
                id: radioButton
                x: 17
                y: 13
                width: 37
                height: 40
                text: qsTr("Radio Button")
            }

            Switch {
                id: switch1
                x: 7
                y: 83
                width: 64
                height: 40
                text: qsTr("Switch")
            }
        }

        GroupBox {
            id: groupBox7
            x: 0
            y: 219
            width: 121
            height: 209
            title: qsTr("Group Box")

            Button {
                id: button13
                x: 21
                y: 23
                text: qsTr("Button")
                width: 34
                height: 34

            }

            RadioDelegate {
                id: radioDelegate
                x: 12
                y: 96
                width: 55
                height: 41
                text: qsTr("Radio Delegate")
            }
        }

        GroupBox {
            id: groupBox17
            x: 0
            y: 458
            width: 121
            height: 231
            title: qsTr("Group Box")

            Button {
                id: button10
                x: 25
                y: 48
                text: qsTr("Button")
                width: 34
                height: 34
            }

            RoundButton {
                id: roundButton
                x: 25
                y: 115
                text: "+"
            }
        }

        GroupBox {
            id: groupBox18
            x: 0
            y: 727
            width: 115
            height: 237
            title: qsTr("Group Box")

            Button {
                id: button9
                x: 20
                y: 48
                text: qsTr("Button")
                width: 34
                height: 34
            }

            SpinBox {
                id: spinBox
                x: -10
                y: 147
                width: 109
                height: 40
            }
        }
    }


    GroupBox {
        id: groupBox3
        x: 0
        y: 0
        width: 1876
        height: 127
        title: qsTr("Group Box")

        RowLayout {
            x: 10
            y: 0

            Rectangle{
                id:rectangle5
                x:14
                y:12
                Layout.preferredHeight: 93
                Layout.preferredWidth: 200
                color: "black"

                GroupBox {
                    id: groupBox8
                    x: 0
                    y: 0
                    width: parent.width
                    height: parent.height
                    anchors.fill: parent
                    title: qsTr("Group Box")
                    Button {
                        id: button
                        x: 14
                        y: 12
                        text: qsTr("Button")
                        width: 34
                        height: 34
                    }

                    Button {
                        id: button16
                        x: 79
                        y: 12
                        width: 34
                        height: 34
                        text: qsTr("Button")
                    }

                    Button {
                        id: button17
                        x: 142
                        y: 12
                        width: 34
                        height: 34
                        text: qsTr("Button")
                    }

                }
            }
            Rectangle{
                id:rectangle6
                x:14
                y:12
                Layout.preferredHeight: 93
                Layout.preferredWidth: 200
                color: "black"

                GroupBox {
                    id: groupBox9
                    x: 0
                    y: 0
                    width: parent.width
                    height: parent.height
                    anchors.fill: parent
                    title: qsTr("Group Box")
                    Button {
                        id: button1
                        x: 14
                        y: 12
                        text: qsTr("Button")
                        width: 34
                        height: 34
                    }

                    Button {
                        id: button18
                        x: 71
                        y: 12
                        width: 34
                        height: 34
                        text: qsTr("Button")
                    }

                    Button {
                        id: button19
                        x: 129
                        y: 12
                        width: 34
                        height: 34
                        text: qsTr("Button")
                    }

                }
            }
            Rectangle{
                id:rectangle7
                x:14
                y:12
                Layout.preferredHeight: 93
                Layout.preferredWidth: 200
                color: "black"

                GroupBox {
                    id: groupBox10
                    x: 0
                    y: 0
                    width: parent.width
                    height: parent.height
                    anchors.fill: parent
                    title: qsTr("Group Box")
                    Button {
                        id: button2
                        x: 14
                        y: 12
                        text: qsTr("Button")
                        width: 34
                        height: 34
                    }

                    Button {
                        id: button20
                        x: 71
                        y: 12
                        width: 34
                        height: 34
                        text: qsTr("Button")
                    }

                    Button {
                        id: button21
                        x: 128
                        y: 12
                        width: 34
                        height: 34
                        text: qsTr("Button")
                    }

                }
            }
            Rectangle{
                id:rectangle8
                x:14
                y:12
                Layout.preferredHeight: 93
                Layout.preferredWidth: 200
                color: "black"

                GroupBox {
                    id: groupBox11
                    x: 0
                    y: 0
                    width: parent.width
                    height: parent.height
                    anchors.fill: parent
                    title: qsTr("Group Box")
                    Button {
                        id: button3
                        x: 14
                        y: 12
                        text: qsTr("Button")
                        width: 34
                        height: 34
                    }

                    Button {
                        id: button22
                        x: 71
                        y: 12
                        width: 34
                        height: 34
                        text: qsTr("Button")
                    }

                    Button {
                        id: button23
                        x: 124
                        y: 12
                        width: 34
                        height: 34
                        text: qsTr("Button")
                    }

                }
            }
            Rectangle{
                id:rectangle9
                x:14
                y:12
                Layout.preferredHeight: 93
                Layout.preferredWidth: 200
                color: "black"

                GroupBox {
                    id: groupBox12
                    x: 0
                    y: 0
                    width: parent.width
                    height: parent.height
                    anchors.fill: parent
                    title: qsTr("Group Box")
                    Button {
                        id: button4
                        x: 14
                        y: 12
                        text: qsTr("Button")
                        width: 34
                        height: 34
                    }

                }
            }

            Rectangle{
                id:rectangle10
                x:14
                y:12
                Layout.preferredHeight: 93
                Layout.preferredWidth: 200
                color: "black"

                GroupBox {
                    id: groupBox13
                    x: 0
                    y: 0
                    width: parent.width
                    height: parent.height
                    anchors.fill: parent
                    title: qsTr("Group Box")
                    Button {
                        id: button5
                        x: 14
                        y: 12
                        text: qsTr("Button")
                        width: 34
                        height: 34
                    }

                }
            }
            Rectangle{
                id:rectangle11
                x:14
                y:12
                Layout.preferredHeight: 93
                Layout.preferredWidth: 200
                color: "black"

                GroupBox {
                    id: groupBox14
                    x: 0
                    y: 0
                    width: parent.width
                    height: parent.height
                    anchors.fill: parent
                    title: qsTr("Group Box")
                    TextInput {
                        x: 5
                        y: 5
                        width: 150
                        height: 25
                        font.pixelSize: 16
                        cursorVisible: true
                        color: "white"
                        onTextChanged: {
                            console.log("Entered text:", text)
                        }
                    }
                }
            }

            Rectangle{
                id:rectangle12
                x:14
                y:12
                Layout.preferredHeight: 93
                Layout.preferredWidth: 200
                color: "black"

                GroupBox {
                    id: groupBox15
                    x: 0
                    y: 0
                    width: parent.width
                    height: parent.height
                    anchors.fill: parent
                    title: qsTr("Group Box")
                    TextField {
                        id: lineEdit
                        x: 0
                        y: 0
                        width: 176
                        height: 26
                        placeholderText: "Enter text here"
                        onTextChanged: {
                            console.log("Enteredtext:",lineEdit.text)
                        }
                    }
                    TextField {
                        id: lineEdit1
                        x: 0
                        y: 31
                        width: 176
                        height: 26
                        placeholderText: "Enter text here"
                        onTextChanged: {
                            console.log("Enteredtext:",lineEdit.text)
                        }
                    }

                }
            }
        }

        Rectangle{
            id:rectangle13
            x: 1653
            y: 0
            width: 211
            height: 93
            color: "black"

            GroupBox {
                id: groupBox16
                x: 0
                y: 0
                width: parent.width
                height: parent.height
                anchors.fill: parent


                title: qsTr("Group Box")

                ComboBox {
                    id: comboBox
                    x: 5
                    y: 10
                    width: 94
                    height: 40
                    model: ["Option 1", "Option 2", "Option 3"]
                    currentIndex: 0
                    onCurrentIndexChanged: {
                        console.log("Selected option:", comboBox.currentText)
                    }
                }

            }
        }


    }

    GroupBox {
        id: groupBox4
        x: 0
        y: 150
        width: 177
        height: 1000
        title: qsTr("Group Box")

        Rectangle{
            id:rectangle1
            x:6
            y:0
            width: 126
            height: 211
            color: "black"

            GroupBox {
                id: groupBox
                x: 0
                y: 0
                width: parent.width
                height: parent.height
                anchors.fill: parent
                title: qsTr("Group Box")

                Button {
                    id: button0
                    x: 0
                    y: 12
                    text: qsTr("Button")
                    width: 34
                    height: 34
                    onClicked: {
                        popup1.open()
                    }
                }

                Popup {
                    id: popup1
                    width: 200
                    height: 100
                    modal: true
                    focus: true

                    contentItem: Text {
                        text: "Hello, this is a popup message!"
                        wrapMode: Text.WordWrap
                        width: parent.width - 20
                    }
                }

                Button {
                    id: button11
                    x: 0
                    y: 68
                    width: 34
                    height: 34
                    text: qsTr("Button")
                }

                Button {
                    id: button12
                    x: 0
                    y: 120
                    width: 34
                    height: 34
                    text: qsTr("Button")
                }
            }

        }

        Rectangle{
            id:rectangle
            x:6
            y:215
            width: 126
            height: 211
            color: "black"
            GroupBox {
                id: groupBox5
                x: 0
                y: 0
                width: parent.width
                height: parent.height
                anchors.fill: parent
                title: qsTr("Group Box")

                CheckBox {
                    id: checkBox
                    x: 6
                    y: 85
                    text: qsTr("Check Box")
                }
            }
        }

        Rectangle{
            id:rectangle2
            x:7
            y:440
            width: 126
            height: 211
            color: "black"

            GroupBox {
                id: groupBox19
                x: 0
                y: 0
                width: parent.width
                height: parent.height
                anchors.fill: parent
                title: qsTr("Group Box")

                DelayButton {
                    id: delayButton
                    x: 0
                    y: 131
                    width: 34
                    height: 34
                    text: qsTr("Delay Button")
                }

                Button {
                    id: button14
                    x: 0
                    y: 68
                    width: 34
                    height: 34
                    text: qsTr("Button")
                }
            }
        }
        Rectangle{
            id:rectangle3
            x:7
            y:680
            width: 126
            height: 211
            color: "black"

            GroupBox {
                id: groupBox20
                x: 0
                y: 0
                width: parent.width
                height: parent.height
                anchors.fill: parent
                title: qsTr("Group Box")
                DelayButton {
                    id: delayButton1
                    x: 0
                    y: 54
                    width: 34
                    height: 34
                    text: qsTr("Delay Button")
                }

                Button {
                    id: button15
                    x: 0
                    y: 110
                    width: 34
                    height: 34
                    text: qsTr("Button")
                }
            }
        }
    }



}
/*##^##
Designer {
    D{i:0;formeditorColor:"#000000";height:1200;width:1920}
}
##^##*/
