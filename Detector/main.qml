import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("App with C++ Connect Detector")

    Component.onCompleted: {
        iray.connectDetector()
    }

    Column {
        anchors.centerIn: parent
        spacing: 20

        Button {
            text: "MODE"
            onClicked: {
                iray.contribution_In_Mode_Selection_And_other_Operations(false, 1)
            }
        }

        Button {
            text: "Acquire"
            onClicked: {
                iray.imageGrabbing("D:\\tempfile\\proj_", false)
            }
        }
    }
}
