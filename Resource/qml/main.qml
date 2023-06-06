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
        icon.source: stackView.depth > 1 ? "qrc:/svg/icon/angle-left.svg" : "qrc:/svg/icon/menu-burger.svg"
        onTriggered: {
            if (stackView.depth > 1) {
                stackView.pop()
                listView.currentIndex = 0
            } else {
                drawer.open()
            }
        }
    }

    Shortcut {
        sequence: "Menu"
        onActivated: optionsMenuAction.trigger()
    }

    Action {
        id: optionsMenuAction
        icon.name: "menu"
        onTriggered: optionsMenu.open()
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                action: navigateBackAction
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            Label {
                id: titleLabel
                Binding {
                    target: titleLabel
                    property: "text"
                    value: listView.currentItem.text
                }
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            ToolButton {
                id: scanQRCodeButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                icon.source: "qrc:/svg/icon/qrcode.svg"
                onClicked: {
                    while(stackView.depth > 1)
                    {
                        stackView.pop()
                    }
                    stackView.push(qrCodeScannerWidgetComponent)
                }
            }
        }
    }

    Drawer {
        id: drawer
        width: Math.min(applicationWindow.width, applicationWindow.height) / 3 * 2
        height: applicationWindow.height
        interactive: stackView.depth === 1

        ListView {
            id: listView

            focus: true
            currentIndex: 0
            anchors.fill: parent

            delegate: ItemDelegate {
                width: listView.width
                text: model.title
                highlighted: ListView.isCurrentItem
                onClicked: {
                    listView.currentIndex = index
                    switch (index)
                    {
                    case 0:
                        while(stackView.depth > 1)
                        {
                            stackView.pop()
                        }
                        break
                    case 1:
                        stackView.push(searchWidgetComponent)
                        break
                    case 2:
                        stackView.push("qrc:/qml/SettingWidget.qml", {builtInStyles: applicationWindow.builtInStyles})
                        break
                    }
                    drawer.close()
                }
            }

            model: ListModel {
                ListElement { title: "题卡列表" }
                ListElement { title: "搜索" }
                ListElement { title: "设置" }
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: SelectWidget {
            id: selectWidget
            onOkButtonClicked: function(templateCode){
                showTemplateDetailWidget(templateCode)
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
        id: applicationWindowQuitDialog
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
            Component.onDestruction: {
                titleLabel.text = listView.currentItem.text
            }
        }
    }

    Component {
        id: qrCodeScannerWidgetComponent
        QRCodeScannerWidget {
            id: qrCodeScannerWidget
            Component.onCompleted: {
                scanQRCodeButton.enabled = false
                titleLabel.text = "扫码界面"
            }
            Component.onDestruction: {
                scanQRCodeButton.enabled = true
                titleLabel.text = listView.currentItem.text
            }
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
            applicationWindowQuitDialog.open()
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
        titleLabel.text = templateRawDataQML.getTemplateName()
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
        titleLabel.text = templateRawDataQML.getTemplateName()
    }
}
