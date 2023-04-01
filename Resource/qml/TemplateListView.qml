import QtQuick

ListView {
    id: list
    delegate: Item {
            width: list.width
            height: 20
            Text { text: model.display }
            MouseArea {
                anchors.fill: parent
                onClicked: list.currentIndex = index
            }
        }
    highlight: Rectangle {
        color: 'steelblue'
//        Text {
//            anchors.centerIn: parent
//            text: model.getTemplateCode(list.currentIndex)
//            color: 'white'
//        }
    }
    signal templateNameClicked(string templateCode)
    onCurrentItemChanged: {
        templateNameClicked(model.getTemplateCode(list.currentIndex))
        console.log(model.getTemplateCode(list.currentIndex))
    }
}
