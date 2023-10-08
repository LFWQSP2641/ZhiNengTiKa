import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    signal templateCodeObtainFinished(string templateCode)
    signal templateSummaryObtainFinished(var templateSummary)
    property var templateSummaryObject: null
    ColumnLayout {
        width: parent.width
        height: parent.height
        MultipleSubjectsTemplateListView {
            id: multipleSubjectsTemplateListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            onMultipleSubjectsTemplateNameClicked: function(templateSummary) {
                templateSummaryObject = templateSummary
                templateCodeTextField.text = templateSummary.templateCode
            }
        }
        TextField {
            id: templateCodeTextField
            property string previousText: ""
            Layout.fillWidth: true
            placeholderText: "题卡编号"

            onPressed: {
                console.log("templateCodeTextField pressed")
                previousText = templateCodeTextField.text
            }
            onTextEdited: {
                console.log("templateCodeTextField textEdited")
                if(templateCodeTextField.text != previousText)
                {
                    console.log("templateCodeTextField text changed")
                    templateSummaryObject = null
                }
            }
        }
        Button {
            Layout.fillWidth: true
            text: "确认"
            onClicked: {
                if(templateSummaryObject == null)
                    templateCodeObtainFinished(templateCodeTextField.text)
                else
                    templateSummaryObtainFinished(templateSummaryObject)
            }
        }
    }
    function setTemplateCode(templateCode) {
        templateCodeTextField.text = templateCode
    }
    function addNewTemplate(templateSummary) {
        multipleSubjectsTemplateListView.addNewTemplate(templateSummary)
    }
}
