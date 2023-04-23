import QtQuick
import QtQuick.Controls

ListView {
    id: templateListView
    clip: true
    delegate: ItemDelegate {
        width: templateListView.width
        text: model.display
        highlighted: ListView.isCurrentItem
        onClicked: {
            templateListView.currentIndex = index
        }
    }
    signal templateNameClicked(string templateCode)
    onCurrentItemChanged: {
        templateNameClicked(model.getTemplateCode(templateListView.currentIndex))
        console.log(model.getTemplateCode(templateListView.currentIndex))
    }
}
