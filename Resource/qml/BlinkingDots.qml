import QtQuick

Item {
    id: root
    width: 800
    height: 600
    property real dotDensity: 0.0001

    property int dotCount: Math.floor(width * height * dotDensity)

    Repeater {
        id: repeater

        delegate: SingleBlinkingDot {
            parentWidth: root.width
            parentHeight: root.height
        }
    }

    onWidthChanged: delayAdjustDotCount.start()
    onHeightChanged: delayAdjustDotCount.start()

    Timer {
        id: delayAdjustDotCount
        interval: 1000
        onTriggered: {
            adjustDotCount()
        }
    }

    function adjustDotCount() {
        dotCount = Math.floor(width * height * dotDensity)
        repeater.model = dotCount
    }
}
