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
    signal templateNameClicked(var templateSummary)
    onCurrentItemChanged: {
        templateNameClicked(model.getTemplateSummary(templateListView.currentIndex))
        console.log(model.getTemplateSummary(templateListView.currentIndex))
    }
}
