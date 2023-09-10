import QtQuick
import QtQuick.Controls
import QtMultimedia
import Qt.labs.platform
import QRCodeScanner

Item {
    id: qrCodeScannerWidget
    property string templateCode: ""
    signal scanFinished(string templateCode)

    Text {
        id: test
        anchors.centerIn: parent
        z: videoOutput - 1
        text: "如果看到我, 说明相机可能未启动\n" +
              "请重新尝试或检查权限"
    }

    MediaDevices {
        id: mediaDevices
    }
    Camera {
        id: camera
        cameraDevice: mediaDevices.defaultVideoInput
    }
    VideoOutput {
        id: videoOutput
        anchors.fill: parent
    }
    CaptureSession {
        camera: camera
        videoOutput: videoOutput
    }

    MessageDialog {
        id: scanFailedDialog
        buttons: MessageDialog.Ok
        function show(text)
        {
            scanFailedDialog.text = text
            scanFailedDialog.open()
        }
    }
    QRCodeScanner {
        id: qrCodeScanner
        videoSink: videoOutput.videoSink
        onDecodingFinished: function(succeeded, text){
            console.log(succeeded)
            console.log(text)
            stateText.text = "扫描" + (succeeded ? ("成功\n" + text) : "失败")
            if(succeeded)
            {
                templateCode = text
                delayCameraClose.start()
            }
        }
        onError: function(msg){
            console.log(msg)
            scanFailedDialog.show("error 发生:\n" + msg)
        }
    }
    Timer {
        id: delayCameraClose
        interval: 500
        onTriggered: {
            camera.stop()
            scanFinished(templateCode)
        }
    }

    Rectangle {
        anchors {left: parent.left; right: parent.right; bottom: parent.bottom}
        height: Math.max(parent.height / 4, 100)
        color: "black"
        Text {
            id: stateText
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            wrapMode: Text.WordWrap
        }
        Rectangle {
            id: whiteRing
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 10
            height: parent.height / 2 + 20
            width: height
            z: parent.z + 1
            radius: 90
            color: "white"
        }
        Rectangle {
            id: blackRing
            anchors.centerIn: whiteRing
            height: parent.height / 2 + 10
            width: height
            z: parent.z + 2
            radius: 90
            color: "black"
        }
        Rectangle {
            id: blueRoundButton
            anchors.centerIn: whiteRing
            height: parent.height / 2
            width: height
            z: parent.z + 3
            radius: 90
            color: "#3d94ff"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    qrCodeScanner.scan()
                }
            }
        }
        RoundButton {
            id: selectFileRoundButton
            visible: true
            icon.source: "qrc:/svg/icon/picture.svg"
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 10
            anchors.bottomMargin: 10
            onClicked: {
                camera.stop()
                fileDialog.open()
            }
        }
    }
    FileDialog {
        id: fileDialog
        fileMode: FileDialog.OpenFile
        title: "选择图片文件"
        folder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]

        // 限制只显示图片文件
        nameFilters:["Images (*.jpg *.jpeg *.png *.gif)"]

        onAccepted: {
            qrCodeScanner.decodeImageByPath(currentFile)
            camera.start()
        }
        onRejected: {
            camera.start()
        }
    }

    Component.onCompleted: {
        camera.start()
    }
    Component.onDestruction: {
        camera.stop()
    }
}
