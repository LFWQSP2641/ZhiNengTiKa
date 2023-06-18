import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import TemplateAnalysisQML

Item {
    property var templateRawDataQMLPointer: null

    TemplateAnalysisQML {
        id: templateAnalysisQML
    }

    ColumnLayout {
        id: mainColumnLayout
        height: parent.height
        width: parent.width
        TabBar {
            id: tabBar
            Layout.fillWidth: true

            TabButton {
                text: "答案和解析"
                onClicked: {
                    templateDetailText.loadHtml(templateAnalysisQML.getAnswerAndAnalysisHtml())
                    width: Math.max(50, mainColumnLayout.width / 3)
                }
            }
            TabButton {
                text: "答案"
                onClicked: {
                    templateDetailText.loadHtml(templateAnalysisQML.getAnswerHtml())
                    width: Math.max(50, mainColumnLayout.width / 3)
                }
            }
            TabButton {
                text: "原题"
                onClicked: {
                    templateDetailText.loadHtml(templateAnalysisQML.getQuestionHtml())
                    width: Math.max(50, mainColumnLayout.width / 3)
                }
            }
        }
        ListView {
            id: questionsCountsListView
            height: Math.max(fm.ascent, 20)
            Layout.fillWidth: true
            orientation: Qt.Horizontal
            delegate: ItemDelegate {
                height: questionsCountsListView.height
                text: model.display
                highlighted: ListView.isCurrentItem
                onClicked: {
                    questionsCountsListView.currentIndex = index
                }
            }
            FontMetrics {
                id: fm
                font: Qt.application.font
            }
            onCurrentItemChanged: {
                if(tabBar.currentIndex===0)
                {
                    templateDetailText.loadHtml(templateAnalysisQML.getAnswerAndAnalysisHtml(questionsCountsListView.currentIndex))
                }
                else if(tabBar.currentIndex===1)
                {
                    templateDetailText.loadHtml(templateAnalysisQML.getAnswerHtml(questionsCountsListView.currentIndex))
                }
                else if(tabBar.currentIndex===2)
                {
                    templateDetailText.loadHtml(templateAnalysisQML.getQuestionHtml(questionsCountsListView.currentIndex))
                }
            }
        }

        Flickable {
            id: flick
            Layout.fillHeight: true
            Layout.fillWidth: true
            contentHeight: templateDetailText.height
            clip: true
            Text {
                id: templateDetailText
                width: parent.width
                wrapMode: Text.Wrap

                function loadHtml(html) {
                    templateDetailText.text = html
                    flick.contentY = 0
                }
                // https://stackoverflow.com/questions/5395106/qml-text-scroll
                // transform不知道是干嘛的
                // 注释了
                // transform: Scale { yScale: -1; origin.y: templateDetailText.height/2 }
            }
            // transform: Scale { yScale: -1; origin.y: flick.height/2 }
        }
    }

    Component.onCompleted: {
        if(templateRawDataQMLPointer !== null)
        {
            setTemplateRawDataQML(templateRawDataQMLPointer)
        }
    }

    function setTemplateRawDataQML(newTemplateAnalysisQML){
        templateAnalysisQML.setValue(newTemplateAnalysisQML)
        questionsCountsListView.model = templateAnalysisQML.getQuestionsCountsStrListModel()
        if(tabBar.currentIndex===0)
        {
            templateDetailText.loadHtml(templateAnalysisQML.getAnswerAndAnalysisHtml())
        }
        else if(tabBar.currentIndex===1)
        {
            templateDetailText.loadHtml(templateAnalysisQML.getAnswerHtml())
        }
        else if(tabBar.currentIndex===2)
        {
            templateDetailText.loadHtml(templateAnalysisQML.getQuestionHtml())
        }
    }
}
