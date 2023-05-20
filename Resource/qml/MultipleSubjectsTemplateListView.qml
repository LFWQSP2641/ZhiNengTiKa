import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MultipleSubjectsTemplateListModelList

Item {
    id: multipleSubjectsTemplateListView
    signal multipleSubjectsTemplateNameClicked(string templateCode)
    FontMetrics {
        id: fm
        font: Qt.application.font
    }
//    ColumnLayout {
//        id: mainColumnLayout
//        width: parent.width
//        height: parent.height
//        spacing: 0
//        TabBar {
//            id: subjectsTabBar
//            Layout.fillWidth: true
//            Repeater {
//                model: ["语文", "数学", "英语", "物理", "化学", "生物", "用户"]

//                TabButton {
//                    text: modelData
//                    width: Math.max(fm.maximumCharacterWidth * 2, mainColumnLayout.width / 7)
//                }
//            }
//        }
//        TemplateListView {
//            model: MultipleSubjectsTemplateListModelList.at(subjectsTabBar.currentIndex > 0 ? subjectsTabBar.currentIndex : 0)
//            Layout.fillWidth: true
//            Layout.fillHeight: true
//            onTemplateNameClicked: function(templateCode){ multipleSubjectsTemplateNameClicked(templateCode) }
//        }
//    }

    TabBar {
        id: subjectsTabBar
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        Repeater {
            model: ["语文", "数学", "英语", "物理", "化学", "生物", "用户"]

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
        onTemplateNameClicked: function(templateCode){ multipleSubjectsTemplateNameClicked(templateCode) }
    }

    function addNewTemplate(templateRawDataQML) {
        MultipleSubjectsTemplateListModelList.addNewTemplate(templateRawDataQML)
        subjectsTabBar.currentIndex = 6
    }
}
