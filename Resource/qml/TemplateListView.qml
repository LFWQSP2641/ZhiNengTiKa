import QtQuick

ListView {
    id: multipleSubjectsTemplateListView
    delegate: Item {
            width: multipleSubjectsTemplateListView.width
            height: fm.ascent * 5 / 3
            Text { text: model.display }
            MouseArea {
                anchors.fill: parent
                onClicked: multipleSubjectsTemplateListView.currentIndex = index
            }
        }
    highlight: Rectangle {
        color: 'steelblue'
//        Text {
//            anchors.centerIn: parent
//            text: model.getTemplateCode(multipleSubjectsTemplateListView.currentIndex)
//            color: 'white'
//        }
    }
    FontMetrics {
        id: fm
        font: Qt.application.font
    }
    signal templateNameClicked(string templateCode)
    onCurrentItemChanged: {
        templateNameClicked(model.getTemplateCode(multipleSubjectsTemplateListView.currentIndex))
        console.log(model.getTemplateCode(multipleSubjectsTemplateListView.currentIndex))
    }
}
