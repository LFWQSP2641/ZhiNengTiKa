import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform as Platform
import SettingOperator

Item {
    SettingOperator {
        id: settingOperator
    }

    Dialog {
        id: dialog
        visible: false
        anchors.centerIn: parent
        ColumnLayout {
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
            RowLayout {
                Layout.fillWidth: true
                Button {
                    id: dialogLoginButton
                    Layout.fillWidth: true
                    text: "登录"
                    onClicked: {
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
                }
                Button {
                    id: dialogCancelButton
                    Layout.fillWidth: true
                    text: "取消"
                    onClicked: {
                        dialog.close()
                    }
                }
            }
        }
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
