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
        anchors.fill: parent
        z: videoOutput.z + 1
        anchors.centerIn: parent
        color: Qt.rgba(1, 1, 1, 0)
        Rectangle {
            id: scannerLine
            width: scannerRectangle.width
            height: 2
            color: "#297b6c"
            radius: 1
            ParallelAnimation {
                running: true
                SequentialAnimation {
                    id: scanLineAnimation
                    loops: Animation.Infinite
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
                    NumberAnimation {
                        target: scannerLine
                        property: "y"
                        to: 0
                        duration: 1000
                        easing.type: Easing.Linear
                    }
                    PauseAnimation {
                        duration: 500
                    }
                }
                SequentialAnimation {
                    id: fadeAnimation
                    loops: Animation.Infinite
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
        anchors.fill: parent
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
