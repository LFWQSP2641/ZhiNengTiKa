import QtQuick
import QtMultimedia
import QRCodeScannerQML

Item {
    id: qrCodeScannerWidget
    property int captureCount: 0
    Text {
        id: test
        anchors.centerIn: parent
        z: -1
        text: "如果看到我, 说明相机可能未启动\n" +
              "请重新尝试或检查权限"
    }
    Text {
        id: stateText
        anchors.top: videoOutput.top
        anchors.left: videoOutput.left
        z: 1
        text: "于1.5秒后开始第一次解析\n" +
              "点击屏幕可强制解析\n"
        function append(str){
            stateText.text += str + "\n"
            if(stateText.height > qrCodeScannerWidget.height)
            {
                stateText.text = str + "\n"
            }
        }
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
        imageCapture: imageCapturer
        videoOutput: videoOutput
    }
    ImageCapture {
        id: imageCapturer
        onImageCaptured: function(requestId, previewImage){
            qrCodeScannerQML.readImage(previewImage)
            captureCount += 1
            stateText.append("已解析" + captureCount + "次")
        }
        onErrorOccurred: function(requestId, error, message){
            stateText.append("发生错误:" + message)
        }
    }

    QRCodeScannerQML {
        id: qrCodeScannerQML
        onApiInitializing: {
            stateText.append("初始化api参数")
        }
        onInitializeApiFinished: function(success){
            if(success)
            {
                stateText.append("初始化api参数成功")
            }
            else
            {
                stateText.append("初始化api参数失败")
            }
        }
        onAnalysisFinished: function(result,success){
            if(success)
            {
                stateText.append("解析成功:" + result)
            }
            else
            {
                stateText.append("解析失败(可能未检测到二维码)")
            }

            imageCapturer.capture()
        }
    }

    Timer {
        id: timer
        interval: 1500
        running: true
        onTriggered: {
            if(imageCapturer.readyForCapture)
            {
                imageCapturer.capture()
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            stateText.append("尝试强制解析")
            imageCapturer.capture()
        }
    }

    Component.onCompleted: {
        stateText.append("设备列表:")
        var videoInputsList = mediaDevices.videoInputs
        for (var i = 0; i < videoInputsList.length; i++)
        {
            stateText.append(videoInputsList[i])
        }


        camera.start()
    }
    Component.onDestruction: {
        camera.stop()
    }
}
