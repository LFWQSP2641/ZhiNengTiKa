import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebView
//import ImageProvider
import TemplateAnalysisQML

Item {
    property var templateRawDataQMLPointer: null
    property bool initFinish: false

    TemplateAnalysisQML {
        id: templateAnalysisQML
    }
//    ImageProvider {
//        id: imageProvider
//        cacheMode: true
//    }

    ColumnLayout {
        id: mainColumnLayout
        height: parent.height
        width: parent.width
        TabBar {
            id: tabBar
            Layout.fillWidth: true

            TabButton {
                text: "答案和解析"
            }
            TabButton {
                text: "答案"
            }
            TabButton {
                text: "原题"
            }
            onCurrentIndexChanged: getHtml()
        }
        RowLayout {
            Layout.fillWidth: true
            Text {
                id: switchAllButton
                Layout.preferredWidth: 60
                horizontalAlignment: Text.AlignHCenter
                visible: questionsCountsListView.currentIndex !== -1
                text: "All"
                MouseArea {
                    anchors.fill: parent
                    onClicked: questionsCountsListView.currentIndex = -1
                }
            }

            ListView {
                id: questionsCountsListView
                height: Math.max(fm.ascent, 20)
                Layout.fillWidth: true
                orientation: Qt.Horizontal
                clip: true
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
                onCurrentItemChanged: getHtml()
            }
        }

//        Flickable {
//            id: flick
//            Layout.fillHeight: true
//            Layout.fillWidth: true
//            contentHeight: templateDetailText.height
//            clip: true
//            Text {
//                id: templateDetailText
//                width: parent.width
//                wrapMode: Text.Wrap

//                function setHtml(html) {
//                    console.log("setHtml")
//                    console.log(html)
//                    templateDetailText.text = imageProvider.loadHtml(html)
//                    flick.contentY = 0
//                }
//                // https://stackoverflow.com/questions/5395106/qml-text-scroll
//                // transform不知道是干嘛的
//                // 注释了
//                //transform: Scale { yScale: -1; origin.y: templateDetailText.height/2 }
//            }
//            //transform: Scale { yScale: -1; origin.y: flick.height/2 }
//        }
        WebView {
            id: templateDetailWebView
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
        initFinish = false
        templateAnalysisQML.setValue(newTemplateAnalysisQML)
        questionsCountsListView.model = templateAnalysisQML.getQuestionsCountsStrListModel()
        initFinish = true
        questionsCountsListView.currentIndex = -1
    }
    function getHtml() {
        if(!initFinish)
        {
            return
        }

        if(tabBar.currentIndex === 0)
        {
            templateDetailWebView.loadHtml(templateAnalysisQML.getAnswerAndAnalysisHtml(questionsCountsListView.currentIndex))
        }
        else if(tabBar.currentIndex === 1)
        {
            templateDetailWebView.loadHtml(templateAnalysisQML.getAnswerHtml(questionsCountsListView.currentIndex))
        }
        else if(tabBar.currentIndex === 2)
        {
            templateDetailWebView.loadHtml(templateAnalysisQML.getQuestionHtml(questionsCountsListView.currentIndex))
        }
    }
}
