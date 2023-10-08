import QtQuick
import QtQuick.Layouts

Rectangle {
    id: rec

    property alias iconSource: icon.source
    property alias buttonText: button.text
    property color backgroundColor: "#ffffff"
    property real widgetSizeRatio: 1
    property bool acceptedRightButtons: false

    //自定义点击信号
    signal clickedLeft()
    signal clickedRight()
    signal release()

    color: backgroundColor

    ColumnLayout {
        anchors.centerIn: parent
        height: parent.height * widgetSizeRatio
        width: parent.width * widgetSizeRatio
        spacing: 10
        Image {
            id: icon
            fillMode: Image.PreserveAspectFit
            clip: true
            mipmap: true
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
        radius: parent.radius
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        //接受左键和右键输入
        acceptedButtons: acceptedRightButtons ? (Qt.LeftButton | Qt.RightButton) : Qt.LeftButton
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
        onReleased: function(mouse){
            if(Math.abs(mouse.x - parent.x) > parent.width ||
                    Math.abs(mouse.y - parent.y) > parent.height)
                return
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


