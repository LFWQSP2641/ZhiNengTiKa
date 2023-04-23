import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import TemplateRawDataQML
import QRCodeScannerQML

ApplicationWindow {
    id: applicationWindow
    width: 640
    height: 480
    visible: true
    title: "智能题卡"

    Shortcut {
        sequences: ["Esc", "Back"]
        enabled: stackView.depth > 1
        onActivated: navigateBackAction.trigger()
    }

    Action {
        id: navigateBackAction
        text: stackView.depth > 1 ? "返回" : "菜单"
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
                text: listView.currentItem ? listView.currentItem.text : "智能题卡"
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            ToolButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: "扫码"
                onClicked: {
                    while(stackView.depth > 1)
                    {
                        stackView.pop()
                    }
                    selectWidget.setTemplateCode(QRCodeScanner.scanQRCodeByTakePhoto())
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
            currentIndex: -1
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
                        stackView.push("qrc:/qml/SettingWidget.qml")
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

    TemplateRawDataQML {
        id: templateRawDataQML
    }

    TemplateDetailWidget {
        id: templateDetailWidget
        width: applicationWindow.width
        height: applicationWindow.height
        visible: false
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

        stackView.push("qrc:/qml/TemplateDetailWidget.qml", {templateRawDataQMLPointer: templateRawDataQML})
    }
}
