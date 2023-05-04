import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform as Platform
import SettingOperator

Item {
    required property var builtInStyles

    SettingOperator {
        id: settingOperator
    }

    Dialog {
        id: dialog
        anchors.centerIn: parent
        modal: true
        focus: true

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            if(settingOperator.login(userIdTextField.text, userPwTextField.text))
            {
                settingOperator.save()
                accountComboBox.model = settingOperator.getUserDataListModel()
                logoutButton.enabled = true
                dialog.close()
            }
            else
            {
                messageDialog.show("登录失败,请确认账号密码是否正确")
            }
        }
        onRejected: {
            userIdTextField.clear()
            userPwTextField.clear()
            dialog.close()
        }

        contentItem: ColumnLayout {
            width: parent.width
            height: parent.height
            TextField {
                id: userIdTextField
                Layout.fillWidth: true
                placeholderText: "账号"
            }
            TextField {
                id: userPwTextField
                Layout.fillWidth: true
                placeholderText: "密码"
            }
        }
    }

    UpdateWidget {
        id: updateWidget
        showQuestionDialog: true
    }

    ScrollView {
        width: parent.width
        height: parent.height
        ColumnLayout {
            // ScrollView与其子空间间有两层parent
            // parent: QQuickFlickable
            // parent.parent: QQuickScrollView
            width: parent.parent.width
            height: parent.parent.height

            GroupBox {
                Layout.fillWidth: true
                title: "账号"
                ColumnLayout {
                    width: parent.width
                    height: parent.height
                    ComboBox {
                        id: accountComboBox
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: settingOperator.getUserDataListModel()
                        textRole: "display"

                        onCurrentIndexChanged: {
                            if(accountComboBox.currentIndex > 0)
                            {
                                settingOperator.userDataListToFirst(accountComboBox.currentIndex)
                                accountComboBox.model = settingOperator.getUserDataListModel()
                            }
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Button {
                            id: loginButton
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            text: "登入"
                            onClicked: {
                                dialog.open()
                            }
                        }
                        Button {
                            id:logoutButton
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            text: "登出"
                            enabled: settingOperator.logined()
                            onClicked: {
                                settingOperator.logout()
                                settingOperator.save()
                                logoutButton.enabled = settingOperator.logined()
                                accountComboBox.model = settingOperator.getUserDataListModel()
                            }
                        }
                    }
                }
            }
            GroupBox {
                Layout.fillWidth: true
                title: "外观(重启生效)"
                ColumnLayout {
                    width: parent.width
                    height: parent.height
                    RowLayout {
                        Label {
                            text: "主题:"
                        }
                        ComboBox {
                            id: styleComboBox
                            Layout.fillWidth: true
                            property int styleIndex: -1
                            property bool saving: false
                            model: builtInStyles
                            onActivated: {
                                if(saving)
                                {
                                    settingOperator.setStyle(styleComboBox.currentText)
                                    settingOperator.save()
                                }
                            }

                            Component.onCompleted: {
                                styleIndex = find(settingOperator.getStyle(), Qt.MatchFixedString)
                                if (styleIndex !== -1)
                                    currentIndex = styleIndex
                                saving = true
                            }
                        }
                    }
                    RowLayout {
                        Label {
                            text: "字体:"
                        }
                        ComboBox {
                            id: fontComboBox
                            Layout.fillWidth: true
                            property int fontIndex: -1
                            property bool saving: false
                            model: Qt.fontFamilies()
                            onActivated: {
                                if(saving)
                                {
                                    settingOperator.setFont(currentText)
                                    settingOperator.save()
                                }
                            }

                            Component.onCompleted: {
                                fontIndex = find(settingOperator.getFont(), Qt.MatchFixedString)
                                if (fontIndex !== -1)
                                    currentIndex = fontIndex
                                saving = true
                            }
                        }
                    }
                }
            }
            GroupBox {
                Layout.fillWidth: true
                title: "题卡列表"
                ColumnLayout {
                    width: parent.width
                    height: parent.height
                    RowLayout {
                        ComboBox {
                            Layout.fillWidth: true
                            model: ["金川高级中学"]
                        }
                        ComboBox {
                            Layout.fillWidth: true
                            model: ["2021级"]
                        }
                    }
                    Switch {
                        text: "优先显示最新题卡"
                        checked: settingOperator.getListLatestTemplatePreferentially()
                        onCheckedChanged: {
                            settingOperator.setListLatestTemplatePreferentially(checked)
                            settingOperator.save()
                        }
                    }
                }
            }
            GroupBox {
                Layout.fillWidth: true
                title: "扫码相关"
                Switch {
                    text: "压缩二维码图片"
                    checked: settingOperator.getCompressQRCodeImage()
                    onCheckedChanged: {
                        settingOperator.setCompressQRCodeImage(checked)
                        settingOperator.save()
                    }
                }
            }
            GroupBox {
                Layout.fillWidth: true
                title: "版本"
                RowLayout {
                    width: parent.width
                    height: parent.height
                    Label {
                        text: settingOperator.getVersion()
                    }
                    Button {
                        id: checkUpdate
                        text: "检查更新"
                        Layout.fillWidth: true
                        onClicked: {
                            updateWidget.checkUpdate()
                            checkUpdate.enabled = false
                        }
                        Connections {
                            target: updateWidget
                            function onFinished() {
                                checkUpdate.enabled = true
                            }
                        }
                    }
                }
            }
        }
    }

    Platform.MessageDialog {
        id: messageDialog

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }
}
