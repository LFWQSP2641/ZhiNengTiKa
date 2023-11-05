import QtQuick
import QtQuick.Controls

Item {
    id: bouncingMessageDialog
    implicitWidth: Math.max(200, text.contentWidth + 20)
    implicitHeight: Math.max(100, text.contentHeight + okButton.contentHeight + 20)
    visible: false

    Rectangle {
        anchors.fill: parent
        color: "white"

        Text {
            id: text
            text: "bouncingMessageDialog"
            wrapMode: Text.WordWrap
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 5
            }
        }
        Rectangle {
            anchors {
                bottom: okButton.top
                bottomMargin: 5
                right: parent.right
                left: parent.left
            }
            height: 1
            color: "black"
        }

        Text {
            id: okButton
            text: "OK"
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: 5
            }
        }
        MouseArea {
            anchors {
                top: okButton.top
                bottom: okButton.bottom
                right: parent.right
                left: parent.left
            }
            z: parent.z + 1
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                hide() // 点击按钮后隐藏消息提示框
            }
        }
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
        }
    }

    Behavior on y {
        SpringAnimation {
            spring: 2
            damping: 0.2
        }
    }

    NumberAnimation {
        id: popupAnimation
        target: bouncingMessageDialog
        property: "y"
        from: -bouncingMessageDialog.height
        to: (parent.height - bouncingMessageDialog.height) / 2
        duration: 500
        easing.type: Easing.OutBounce
    }

    function show(msgStr) {
        text.text = msgStr
        if(text.contentWidth > bouncingMessageDialog.width)
            text.width = bouncingMessageDialog.width
        bouncingMessageDialog.visible = true
        popupAnimation.start()
    }

    function hide() {
        popupAnimation.stop()
        bouncingMessageDialog.visible = false
        text.text = ""
        text.width = 0
    }
}
