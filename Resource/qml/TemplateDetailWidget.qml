import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebView
import AnalysisWebRawDataQML

Item {
    AnalysisWebRawData {
        id: analysisWebRawData
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
                    webView.loadHtml(analysisWebRawData.getAnswerAndAnalysisHtml())
                    width: Math.max(50, mainColumnLayout.width / 3)
                }
            }
            TabButton {
                text: "答案"
                onClicked: {
                    webView.loadHtml(analysisWebRawData.getAnswerHtml())
                    width: Math.max(50, mainColumnLayout.width / 3)
                }
            }
            TabButton {
                text: "原题"
                onClicked: {
                    webView.loadHtml(analysisWebRawData.getQuestionHtml())
                    width: Math.max(50, mainColumnLayout.width / 3)
                }
            }
        }
        ListView {
            id: questionsCountsListView
            height: fm.ascent
            Layout.fillWidth: true
            orientation: Qt.Horizontal
            delegate: Item {
                    height: mainColumnLayout.height
                    width: Math.max(30, itemText.width * 5 / 4)
                    Text {
                        id: itemText
                        text: model.display
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: questionsCountsListView.currentIndex = index
                    }
                }
            highlight: Rectangle {
                color: 'steelblue'
            }
            FontMetrics {
                id: fm
                font: Qt.application.font
            }
            onCurrentItemChanged: {
                if(tabBar.currentIndex==0)
                {
                    webView.loadHtml(analysisWebRawData.getAnswerAndAnalysisHtml(questionsCountsListView.currentIndex))
                }
                else if(tabBar.currentIndex==1)
                {
                    webView.loadHtml(analysisWebRawData.getAnswerHtml(questionsCountsListView.currentIndex))
                }
                else if(tabBar.currentIndex==2)
                {
                    webView.loadHtml(analysisWebRawData.getQuestionHtml(questionsCountsListView.currentIndex))
                }
            }
        }

        WebView {
            id: webView
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    function setAnalysisWebRawData(newAnalysisWebRawData){
        analysisWebRawData.setValue(newAnalysisWebRawData)
        questionsCountsListView.model = analysisWebRawData.getQuestionsCountsStrListModel()
//                questionsCountsListView.model = ["ss"]
        if(tabBar.currentIndex==0)
        {
            webView.loadHtml(analysisWebRawData.getAnswerAndAnalysisHtml())
        }
        else if(tabBar.currentIndex==1)
        {
            webView.loadHtml(analysisWebRawData.getAnswerHtml())
        }
        else if(tabBar.currentIndex==2)
        {
            webView.loadHtml(analysisWebRawData.getQuestionHtml())
        }
    }
}
