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
        height: parent.height
        width: parent.width
        TabBar {
            id: tabBar
            Layout.fillWidth: true
            //        currentIndex: swipeView.currentIndex

            TabButton {
                text: "答案"
                onClicked: {
                    webView.loadHtml(analysisWebRawData.getAnswerAndAnalysisHtml())
                }
            }
            TabButton {
                text: "原题"
                onClicked: {
                    webView.loadHtml(analysisWebRawData.getQuestionHtml())
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
        if(tabBar.currentIndex==0)
        {
            webView.loadHtml(analysisWebRawData.getAnswerAndAnalysisHtml())
        }
        else if(tabBar.currentIndex==1)
        {
            webView.loadHtml(analysisWebRawData.getQuestionHtml())
        }
    }
}
