import QtQuick

Rectangle {
    id: dot
    width: 2
    height: 2
    color: "white"
    radius: width / 2

    property int parentWidth: 0
    property int parentHeight: 0

    SequentialAnimation on opacity {
        running: true
        NumberAnimation {
            duration: Math.random() * 1000
            from: 0.0
            to: 1.0
        }
        NumberAnimation {
            duration: Math.random() * 1000
            from: 1.0
            to: 0.0
        }
        onFinished: {
            setRandomCoordinate()
            start()
        }
    }

    Component.onCompleted: {
        setRandomCoordinate()
    }

    function setRandomCoordinate() {
        x = Math.random() * parentWidth
        y = Math.random() * parentHeight
    }
}
