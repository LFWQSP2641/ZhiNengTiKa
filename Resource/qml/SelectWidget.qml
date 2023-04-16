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
                templateCodeTextInput.clear()
                templateCodeTextInput.insert(0, templateCode)
            }
        }
        TextInput {
            id: templateCodeTextInput
            Layout.alignment: Qt.AlignHCenter
            text: "输入框"
        }
        Button {
            Layout.fillWidth: true
            text: "确认"
            onClicked: okButtonClicked(templateCodeTextInput.text)
        }
    }
    function setTemplateCode(templateCode) {
        if(templateCode.length === 0)
        {
            console.log("templateCode is empty, return")
            return
        }
        templateCodeTextInput.text = templateCode
        okButtonClicked(templateCode)
    }
    function addNewTemplate(templateRawDataQML) {
        multipleSubjectsTemplateListView.addNewTemplate(templateRawDataQML)
    }
}
