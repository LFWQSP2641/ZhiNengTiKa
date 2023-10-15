import QtQuick

Item {
    id: root
    width: 800
    height: 600
    property int dotCount: 100  // 创建100个星点，可以根据需要调整数量

    Repeater {
        model: dotCount

        delegate: SingleBlinkingDot {
            parentWidth: root.width
            parentHeight: root.height
        }
    }
}
