import QtQuick

Rectangle {
    id: rec

    property alias iconSource: icon.source
    property alias buttonText: button.text

    property color clr_enter: "#dcdcdc"
    property color clr_exit: "#ffffff"
    property color clr_click: "#aba9b2"
    property color clr_release: "#ffffff"

    //自定义点击信号
    signal clickedLeft()
    signal clickedRight()
    signal release()

    Image {
        id: icon
        fillMode: Image.PreserveAspectFit
        clip: true
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
    }

    Text {
        id: button
        anchors.top: icon.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: icon.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        //接受左键和右键输入
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
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
            color = clr_click
        }

        //释放
        onReleased: {
            color = clr_enter
            parent.release()
        }

        //指针进入
        onEntered: {
            color = clr_enter
        }

        //指针退出
        onExited: {
            color = clr_exit
        }
    }
}


