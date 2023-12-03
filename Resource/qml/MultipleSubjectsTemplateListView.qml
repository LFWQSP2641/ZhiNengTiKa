import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MultipleSubjectsTemplateListModelList

Item {
    id: multipleSubjectsTemplateListView
    signal multipleSubjectsTemplateNameClicked(var templateSummary)
    FontMetrics {
        id: fm
        font: Qt.application.font
    }
    TabBar {
        id: subjectsTabBar
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        Repeater {
            model: ["用户历史", "语文", "数学", "英语", "物理", "化学", "生物"]

            TabButton {
                text: modelData
                width: Math.max(fm.maximumCharacterWidth * 2, multipleSubjectsTemplateListView.width / 7)
            }
        }
    }
    TemplateListView {
        anchors.top: subjectsTabBar.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        model: MultipleSubjectsTemplateListModelList.at(subjectsTabBar.currentIndex > 0 ? subjectsTabBar.currentIndex : 0)
        onTemplateNameClicked: function(templateSummary){ multipleSubjectsTemplateNameClicked(templateSummary) }
    }

    function addNewTemplate(templateSummary) {
        MultipleSubjectsTemplateListModelList.addNewTemplate(templateSummary)
        subjectsTabBar.currentIndex = 6
    }
}
