import QtQuick
import QtQuick.Layouts

Rectangle {
    id: rec

    property alias iconSource: icon.source
    property alias buttonText: button.text
    property color backgroundColor: "#ffffff"

    //自定义点击信号
    signal clickedLeft()
    signal clickedRight()
    signal release()

    color: backgroundColor

    ColumnLayout {
        anchors.fill: parent
        Image {
            id: icon
            fillMode: Image.PreserveAspectFit
            clip: true
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Text {
            id: button
            Layout.alignment: Qt.AlignHCenter
            Layout.fillHeight: true
        }
    }

    Rectangle {
        id: foreground
        color: "#000000"
        z: parent.z + 1
        anchors.fill: parent
        opacity: 0
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        //接受左键和右键输入
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: function(mouse){
            //左键点击
            if (mouse.button === Qt.LeftButton)
            {
                parent.clickedLeft()
            }
            else if(mouse.button === Qt.RightButton)
            {
                parent.clickedRight()
            }
        }

        //按下
        onPressed: {
            foreground.opacity = 0.4
        }

        //释放
        onReleased: {
            foreground.opacity = 0.2
            parent.release()
        }

        //指针进入
        onEntered: {
            foreground.opacity = 0.2
        }

        //指针退出
        onExited: {
            foreground.opacity = 0
        }
    }
}


