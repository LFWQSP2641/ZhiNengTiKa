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
        Rectangle {
            id: flashRectangle
            anchors.fill: parent
            color: "black"
            opacity: 0
            function flash() {
                flashAnimation.start()
            }
            SequentialAnimation {
                id: flashAnimation

                // 使矩形透明度变为1，然后再变回0，从而实现 "闪一下" 的效果
                PropertyAnimation { target: flashRectangle; property: "opacity"; to: 1; duration: 0 }
                PropertyAnimation { target: flashRectangle; property: "opacity"; to: 0; duration: 200 }
            }
        }
    }
    CaptureSession {
        camera: camera
        videoOutput: videoOutput
    }
    ListModel {
        id: videoInputsListModel
    }

    ComboBox {
        id: videoInputsComboBox
        anchors {top: parent.top; left: parent.left; right: parent.right}
        model: videoInputsListModel
        onCurrentIndexChanged: {
            if(videoInputsComboBox.currentIndex >= 0)
            {
                camera.stop()
                camera.cameraDevice = mediaDevices.videoInputs[videoInputsComboBox.currentIndex]
                camera.start()
            }
        }
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
            else
            {
                stateText.text = "未检测到二维码, 请重试"
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
            anchors {top: parent.top; horizontalCenter: parent.horizontalCenter; margins: 10}
            color: "white"
            wrapMode: Text.WordWrap
        }
        Rectangle {
            id: whiteRing
            anchors {top: stateText.bottom; bottom: parent.bottom; horizontalCenter: parent.horizontalCenter; margins: 10}
            width: height
            z: parent.z + 1
            radius: 90
            color: "white"
        }
        Rectangle {
            id: blackRing
            anchors.centerIn: whiteRing
            height: whiteRing.height - 10
            width: height
            z: whiteRing.z + 1
            radius: 90
            color: parent.color
        }
        Rectangle {
            id: blueRoundButton
            anchors.centerIn: whiteRing
            height: blackRing.height - 10
            width: height
            z: blackRing.z + 1
            radius: 90
            color: "#3d94ff"
            Rectangle {
                anchors.fill: parent
                z: parent.z + 1
                radius: parent.radius
                color: "black"
                opacity: 0
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    //按下
                    onPressed: {
                        parent.opacity = 0.4
                    }
                    //释放
                    onReleased: {
                        parent.opacity = 0.2
                    }
                    //指针进入
                    onEntered: {
                        parent.opacity = 0.2
                    }
                    //指针退出
                    onExited: {
                        parent.opacity = 0
                    }
                    onClicked: {
                        qrCodeScanner.scan()
                        flashRectangle.flash()
                    }
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
            z: parent.z + 1
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
        for (var i = 0; i < mediaDevices.videoInputs.length; i++)
        {
            videoInputsListModel.append({"text": mediaDevices.videoInputs[i].description})
        }
        videoInputsComboBox.currentIndex = 0
    }
    Component.onDestruction: {
        camera.stop()
    }
}
