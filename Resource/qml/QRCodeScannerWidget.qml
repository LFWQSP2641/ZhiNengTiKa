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
        width: stateText.contentWidth
        height: stateText.contentHeight
        radius: 10
        color: "#7FFFFFFF"

        Text {
            id: stateText
            text: "点击屏幕扫码"
            wrapMode: Text.WordWrap
        }

        anchors.centerIn: parent
        anchors.verticalCenterOffset: parent.height / 3
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            qrCodeScanner.scan()
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
    }

    Component.onCompleted: {
        camera.start()
    }
    Component.onDestruction: {
        camera.stop()
    }
}
