import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QRCodeScannerQML

Item {
    signal okButtonClicked(string templateCode)
    ColumnLayout {
        width: parent.width
        height: parent.height
        RowLayout {
            Button {
                Layout.fillWidth: true
                text: "扫码"
                onClicked: {
                    templateCodeTextInput.text = QRCodeScanner.scanQRCodeByTakePhoto()
                }
            }
            Button {
                Layout.fillWidth: true
                text: "搜索"
            }
        }
        MultipleSubjectsTemplateListView {
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
}
