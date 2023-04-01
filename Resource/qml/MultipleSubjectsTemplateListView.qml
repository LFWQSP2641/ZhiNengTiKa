import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MultipleSubjectsTemplateListModelList

Item {
    signal multipleSubjectsTemplateNameClicked(string templateCode)
    FontMetrics {
        id: fm
        font: Qt.application.font
    }
    ColumnLayout {
        id: mainColumnLayout
        width: parent.width
        height: parent.height
        TabBar {
            id: subjectsTabBar
            Layout.fillWidth: true
            Repeater {
                model: ["语文", "数学", "英语", "物理", "化学", "生物", "自定义"]



                TabButton {
                    text: modelData
                    width: Math.max(fm.maximumCharacterWidth * 2, mainColumnLayout.width / 7)
                }
            }
        }
        TemplateListView {
            model: MultipleSubjectsTemplateListModelList.at(subjectsTabBar.currentIndex > 0 ? subjectsTabBar.currentIndex : 0)
            Layout.fillWidth: true
            Layout.fillHeight: true
            onTemplateNameClicked: function(templateCode){ multipleSubjectsTemplateNameClicked(templateCode) }
        }
    }
}
