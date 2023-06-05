import QtQuick
import QtMultimedia
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
    Text {
        id: stateText
        anchors.top: videoOutput.top
        anchors.left: videoOutput.left
        z: 1
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
        videoOutput: videoOutput
    }

    QRCodeScannerQML {
        id: qrCodeScannerQML
        videoSink: videoOutput.videoSink
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
        onAnalysisFinishedQML: function(result, success){
            if(success)
            {
                stateText.append("解析成功:" + result)
                templateCode = result
                waitTimer.start()
            }
            else
            {
                stateText.append("解析失败(可能未检测到二维码)")
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
