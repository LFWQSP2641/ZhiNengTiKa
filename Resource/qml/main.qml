import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import WebRawDataQML

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: "智能题卡"

    WebRawData {
        id: webRawData
    }

    SwipeView {
        id: swipeView
        anchors.top: tabBar.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        currentIndex: tabBar.currentIndex

        SelectWidget {
            onOkButtonClicked: function(templateCode){
                webRawData.setValue(templateCode)
                templateDetailWidget.setAnalysisWebRawData(webRawData.toAnalysisWebRawDataQML())
                tabBar.setCurrentIndex(1)
            }
        }
        TemplateDetailWidget {
            id: templateDetailWidget
        }

        SettingWidget {
        }
    }

    TabBar {
        anchors.left: parent.left
        anchors.right: parent.right

        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: "题卡"
        }
        TabButton {
            text: "解析"
        }
        TabButton {
            text: "设置"
        }
    }
}
