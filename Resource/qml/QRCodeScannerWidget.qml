import QtQuick
import QtMultimedia
import Qt.labs.platform
import QRCodeScannerQML

Item {
    id: qrCodeScannerWidget
    signal scanFinished(string templateCode)
    property string templateCode: ""
    Text {
        id: test
        anchors.centerIn: parent
        z: -1
        text: "如果看到我, 说明相机可能未启动\n" +
              "请重新尝试或检查权限"
    }
    // 扫码线动画
    Rectangle {
        id: scannerRectangle
        width: parent.width / 2
        height: width
        z: videoOutput.z + 1
        anchors.centerIn: parent
        color: Qt.rgba(1, 1, 1, 0)
        Rectangle {
            id: scannerLine
            width: scannerRectangle.width
            height: 2
            color: "#297b6c"
            radius: 1

            NumberAnimation {
                id: scanAnimation
                target: scannerLine
                property: "y"
                from: 0
                to: scannerRectangle.height - scannerLine.height
                duration: 1000
                easing.type: Easing.Linear
                running: true
                onFinished: {
                    pauseTimer.start()
                }
            }
            Timer {
                id: pauseTimer
                interval: 500
                onTriggered: {
                    scanAnimation.start()
                }
            }
        }
    }
    // 遮盖
    // 上
    Rectangle {
        id: topRectangle
        z: videoOutput.z + 1
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.top: videoOutput.top
        anchors.left: videoOutput.left
        anchors.right: videoOutput.right
        anchors.bottom: scannerRectangle.top
    }
    // 下
    Rectangle {
        id: bottomRectangle
        z: videoOutput.z + 1
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.top: scannerRectangle.bottom
        anchors.left: videoOutput.left
        anchors.right: videoOutput.right
        anchors.bottom: videoOutput.bottom
    }
    // 左
    Rectangle {
        id: leftRectangle
        z: videoOutput.z + 1
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.top: topRectangle.bottom
        anchors.left: videoOutput.left
        anchors.right: scannerRectangle.left
        anchors.bottom: bottomRectangle.top
    }
    // 右
    Rectangle {
        id: rightRectangle
        z: videoOutput.z + 1
        color: Qt.rgba(0, 0, 0, 0.5)
        anchors.top: topRectangle.bottom
        anchors.left: scannerRectangle.right
        anchors.right: videoOutput.right
        anchors.bottom: bottomRectangle.top
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
        id: initializeFailedDialog
        buttons: MessageDialog.Ok
        text: "初始化api参数失败"
        onOkClicked: scanFinished("")
    }
    QRCodeScannerQML {
        id: qrCodeScannerQML
        videoSink: videoOutput.videoSink
        onInitializeApiFinished: function(success){
            if(!success)
            {
                camera.stop()
                initializeFailedDialog.open()
            }
        }
        onAnalysisFinishedQML: function(result, success){
            if(success)
            {
                templateCode = result
                waitTimer.start()
            }
        }
    }

    Timer {
        id: waitTimer
        interval: 500
        onTriggered: {
            camera.stop()
            scanFinished(templateCode)
        }
    }

    Component.onCompleted: {
        camera.start()
    }
    Component.onDestruction: {
        camera.stop()
    }
}
