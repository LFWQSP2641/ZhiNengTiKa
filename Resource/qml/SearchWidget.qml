import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import TemplateSearcher
import TemplateListModel

Item {
    signal okButtonClicked(string templateCode)
    property string currentTemplateCode: ""
    TemplateSearcher {
        id: templateSearcher

        onSearchResult: function(templateSummary){
            templateListModel.addNewTemplate(templateSummary)
        }
        onSearchFinished: {
            templateListView.model = templateListModel
            // WARNING 第二次搜索之后只显示三个结果
            if (templateListView.count !== templateListModel.rowCount())
            {
                templateListView.model = null
                templateListView.model = templateListModel
            }

            stateText.text = templateListView.count + " 个搜索结果"
            stopSearchButton.visible = false
            startSearchButton.visible = true
            stopSearchButton.enabled = true
        }
    }
    TemplateListModel {
        id: templateListModel
    }

    ColumnLayout {
        width: parent.width
        height: parent.height
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            TextField {
                id: templateNameTextField
                Layout.fillWidth: true
                placeholderText: "题卡名称"
            }
            Button {
                id: startSearchButton
                text: "搜索"
                onClicked: {
                    startSearchButton.visible = false
                    stopSearchButton.visible = true
                    templateListModel.clear()
                    console.log(templateNameTextField.text)
                    templateSearcher.setSearchedTemplateName(templateNameTextField.text)
                    templateSearcher.start()
                }
            }
            Button {
                id: stopSearchButton
                text: "停止"
                visible: false
                onClicked: {
                    stopSearchButton.enabled = false
                }
            }
        }
        Text {
            id: stateText
        }
        TemplateListView {
            id: templateListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            onTemplateNameClicked: function(templateCode){
                currentTemplateCode = templateCode
            }
        }
        Button {
            id: selectOKButton
            Layout.fillWidth: true
            text: "确定"
            onClicked: okButtonClicked(currentTemplateCode)
        }
    }
}
