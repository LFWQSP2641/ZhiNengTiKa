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
        onDecodingFinished: function(succeeded, result){
            console.log(succeeded)
            console.log(result.text)
            console.log(result.position.topLeft)
            console.log(result.position.topRight)
            console.log(result.position.bottomRight)
            console.log(result.position.bottomLeft)
            if(succeeded)
            {
                templateCode = result.text
                delayCameraClose.start()
            }
            result.deleteLater()
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

    RoundButton {
        id: selectFileRoundButton
        icon.source: "qrc:/svg/icon/picture.svg"
        anchors {right: parent.right; bottom: parent.bottom; rightMargin: 10; bottomMargin: 10}
        z: parent.z + 1
        onClicked: {
            camera.stop()
            fileDialog.open()
        }
    }
    RoundButton {
        id: switchCameraDevice
        property int currentDeviceIndex: 0
        icon.source: "qrc:/svg/icon/refresh.svg"
        anchors {left: parent.left; bottom: parent.bottom; leftMargin: 10; bottomMargin: 10}
        z: parent.z + 1
        onClicked: {
            switchCameraDevice.currentDeviceIndex += 1
            if(switchCameraDevice.currentDeviceIndex > (mediaDevices.videoInputs.length - 1))
                switchCameraDevice.currentDeviceIndex = 0
            camera.cameraDevice = mediaDevices.videoInputs[switchCameraDevice.currentDeviceIndex]
            camera.stop()
            camera.start()
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
            qrCodeScanner.qrCodeReader.decodeImageByPath(currentFile)
            camera.start()
        }
        onRejected: {
            camera.start()
        }
    }

    // 扫码线动画
    Rectangle {
        id: scannerRectangle
        width: Math.floor(videoOutput.width > videoOutput.height ? videoOutput.height * 4 / 5 : videoOutput.width * 4 / 5)
        height: width
        z: videoOutput.z + 1
        anchors.centerIn: videoOutput
        color: Qt.rgba(1, 1, 1, 0)

        // 左上角
        Rectangle {
            anchors {top: parent.top; left: parent.left}
            width: Math.floor(parent.width / 10)
            height: 2
            color: "white"
        }
        Rectangle {
            anchors {top: parent.top; left: parent.left}
            width: 2
            height: Math.floor(parent.height / 10)
            color: "white"
        }

        // 右上角
        Rectangle {
            anchors {top: parent.top; right: parent.right}
            width: Math.floor(parent.width / 10)
            height: 2
            color: "white"
        }
        Rectangle {
            anchors {top: parent.top; right: parent.right}
            width: 2
            height: Math.floor(parent.height / 10)
            color: "white"
        }

        // 左下角
        Rectangle {
            anchors {bottom: parent.bottom; left: parent.left}
            width: Math.floor(parent.width / 10)
            height: 2
            color: "white"
        }
        Rectangle {
            anchors {bottom: parent.bottom; left: parent.left}
            width: 2
            height: Math.floor(parent.height / 10)
            color: "white"
        }

        // 右下角
        Rectangle {
            anchors {bottom: parent.bottom; right: parent.right}
            width: Math.floor(parent.width / 10)
            height: 2
            color: "white"
        }
        Rectangle {
            anchors {bottom: parent.bottom; right: parent.right}
            width: 2
            height: Math.floor(parent.height / 10)
            color: "white"
        }

        Rectangle {
            id: scannerLine
            width: scannerRectangle.width
            height: 2
            color: "#297b6c"
            ParallelAnimation {
                running: true
                loops: Animation.Infinite
                SequentialAnimation {
                    NumberAnimation {
                        target: scannerLine
                        property: "y"
                        from: 0
                        to: scannerRectangle.height - scannerLine.height
                        duration: 1000
                        easing.type: Easing.Linear
                    }
                    PauseAnimation {
                        duration: 500
                    }
                }
                SequentialAnimation {
                    PropertyAnimation {
                        target: scannerLine
                        property: "opacity"
                        from: 0
                        to: 1 // 设置为1以进行淡入
                        duration: 500
                        easing.type: Easing.Linear
                    }
                    PropertyAnimation {
                        target: scannerLine
                        property: "opacity"
                        from: 1
                        to: 0 // 设置为0以进行淡出
                        duration: 500
                        easing.type: Easing.Linear
                    }
                    PauseAnimation {
                        duration: 500
                    }
                }
            }
        }
    }

    BlinkingDots {
        anchors.fill: videoOutput
    }

    Component.onCompleted: {
        camera.start()
        qrCodeScanner.startThread()
    }
    Component.onDestruction: {
        qrCodeScanner.stopThread()
        camera.stop()
        qrCodeScanner.waitForThreadFinish()
    }
}
