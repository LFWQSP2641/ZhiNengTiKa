import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import TemplateRawDataQML

ApplicationWindow {
    id: applicationWindow
    width: 640
    height: 480
    visible: true
    title: "智能题卡"

    required property var builtInStyles

    Shortcut {
        sequences: ["Esc", "Back"]
        enabled: stackView.depth > 1
        onActivated: navigateBackAction.trigger()
    }

    Action {
        id: navigateBackAction
        text: "返回上一页"
        onTriggered: {
            if (stackView.depth > 1) {
                stackView.pop()
            }
        }
    }

    header: TabButton{
        action: navigateBackAction
        visible: stackView.depth > 1 &&
                 Qt.platform.os !== "android" &&
                 Qt.platform.os !== "ios"
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: Item {
            ColumnLayout {
                height: parent.height
                width: parent.width
                spacing: 0
                RowLayout{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.verticalStretchFactor: 3
                    spacing: 0
                    Button {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        text: "扫码"
                        onClicked: stackView.push(qrCodeScannerWidgetComponent)
                    }
                    Button {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        text: "列表"
                        onClicked: stackView.push(selectWidgetComponent)
                    }
                }
                RowLayout{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.verticalStretchFactor: 1
                    spacing: 0
                    Button {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        text: "搜索"
                        onClicked: stackView.push(searchWidgetComponent)
                    }
                    Button {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        text: "设置"
                        onClicked: stackView.push("qrc:/qml/SettingWidget.qml",
                                                  {builtInStyles: applicationWindow.builtInStyles})
                    }
                }
            }
        }
    }

    MessageDialog {
        id: messageDialog

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }

    MessageDialog {
        id: versionRetiredQuitDialog
        buttons: MessageDialog.Ok
        text: "请更新版本或检查网络连接"
        onOkClicked: Qt.exit(1)
    }

    TemplateRawDataQML {
        id: templateRawDataQML
    }

    UpdateWidget {
        id: updateWidget
        showQuestionDialog: false
    }

    // stackView.push在第二个参数连接信号槽好像有点问题
    // TypeError: Cannot assign to read-only property "okButtonClicked"
    // 所以拿个Component包一下

    Component {
        id: selectWidgetComponent
        SelectWidget {
            id: selectWidget
            onOkButtonClicked: function(templateCode){
                showTemplateDetailWidget(templateCode)
            }
        }
    }

    Component {
        id: searchWidgetComponent
        SearchWidget {
            onOkButtonClicked: function(templateCode){
                showTemplateDetailWidget(templateCode)
            }
        }
    }

    Component {
        id: templateDetailWidgetComponent
        TemplateDetailWidget {
            templateRawDataQMLPointer: templateRawDataQML
        }
    }

    Component {
        id: qrCodeScannerWidgetComponent
        QRCodeScannerWidget {
            id: qrCodeScannerWidget
            onScanFinished: function(templateCode){
                selectWidget.setTemplateCode(templateCode)
                showTemplateDetailWidgetAndPop(templateCode)
            }
        }
    }

    Component.onCompleted: {
        updateWidget.checkUpdate()
        if(!updateWidget.checkMinimumVersion())
        {
            versionRetiredQuitDialog.open()
        }
    }

    function showTemplateDetailWidget(templateCode) {
        templateRawDataQML.setValue(templateCode)
        if(!templateRawDataQML.isValid())
        {
            messageDialog.show(templateRawDataQML.getErrorStr())
            return
        }
        if(templateRawDataQML.isNetwork())
        {
            selectWidget.addNewTemplate(templateRawDataQML)
        }

        stackView.push(templateDetailWidgetComponent)
    }
    function showTemplateDetailWidgetAndPop(templateCode) {
        templateRawDataQML.setValue(templateCode)
        if(!templateRawDataQML.isValid())
        {
            messageDialog.show(templateRawDataQML.getErrorStr())
            return
        }
        if(templateRawDataQML.isNetwork())
        {
            selectWidget.addNewTemplate(templateRawDataQML)
        }

        stackView.pop()
        stackView.push(templateDetailWidgetComponent)
    }
}
