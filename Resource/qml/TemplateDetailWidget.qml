import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebView
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
                    webView.loadHtml(templateAnalysisQML.getAnswerAndAnalysisHtml())
                    width: Math.max(50, mainColumnLayout.width / 3)
                }
            }
            TabButton {
                text: "答案"
                onClicked: {
                    webView.loadHtml(templateAnalysisQML.getAnswerHtml())
                    width: Math.max(50, mainColumnLayout.width / 3)
                }
            }
            TabButton {
                text: "原题"
                onClicked: {
                    webView.loadHtml(templateAnalysisQML.getQuestionHtml())
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
                    webView.loadHtml(templateAnalysisQML.getAnswerAndAnalysisHtml(questionsCountsListView.currentIndex))
                }
                else if(tabBar.currentIndex===1)
                {
                    webView.loadHtml(templateAnalysisQML.getAnswerHtml(questionsCountsListView.currentIndex))
                }
                else if(tabBar.currentIndex===2)
                {
                    webView.loadHtml(templateAnalysisQML.getQuestionHtml(questionsCountsListView.currentIndex))
                }
            }
        }

        WebView {
            id: webView
            Layout.fillHeight: true
            Layout.fillWidth: true
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
            webView.loadHtml(templateAnalysisQML.getAnswerAndAnalysisHtml())
        }
        else if(tabBar.currentIndex===1)
        {
            webView.loadHtml(templateAnalysisQML.getAnswerHtml())
        }
        else if(tabBar.currentIndex===2)
        {
            webView.loadHtml(templateAnalysisQML.getQuestionHtml())
        }
    }
}
