import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    signal okButtonClicked(string templateCode)
    ColumnLayout {
        width: parent.width
        height: parent.height
        MultipleSubjectsTemplateListView {
            id: multipleSubjectsTemplateListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            onMultipleSubjectsTemplateNameClicked: function(templateCode) {
                templateCodeTextField.clear()
                templateCodeTextField.insert(0, templateCode)
            }
        }
        TextField {
            id: templateCodeTextField
            Layout.fillWidth: true
            placeholderText: "题卡编号"
        }
        Button {
            Layout.fillWidth: true
            text: "确认"
            onClicked: okButtonClicked(templateCodeTextField.text)
        }
    }
    function setTemplateCode(templateCode) {
        templateCodeTextField.text = templateCode
        if(templateCodeTextField.text.length === 0)
        {
            console.log("templateCode is empty, return")
            return
        }
        okButtonClicked(templateCode)
    }
    function addNewTemplate(templateRawDataQML) {
        multipleSubjectsTemplateListView.addNewTemplate(templateRawDataQML)
    }
}
