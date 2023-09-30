import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SettingOperator
import Settings

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
                accountComboBox.model = settingOperator.getUserDataList()
                logoutButton.enabled = true
                refleshUserState()
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
        anchors.centerIn: parent
        width: parent.width
        showQuestionDialog: true
        onFinished: {
            checkUpdate.enabled = true
        }
    }

    Flickable {
        height: parent.height
        width: parent.width
        contentHeight: settingItemsLayout.height
        ColumnLayout {
            id: settingItemsLayout
            width: parent.width
            GroupBox {
                Layout.fillWidth: true
                title: "账号"
                ColumnLayout {
                    width: parent.width
                    height: parent.height
                    GroupBox {
                        Layout.fillWidth: true
                        title: "账号管理"
                        ColumnLayout {
                            width: parent.width
                            height: parent.height
                            ComboBox {
                                id: accountComboBox
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                model: settingOperator.getUserDataList()

                                onActivated: {
                                    if(accountComboBox.currentIndex > 0)
                                    {
                                        settingOperator.userDataListToFirst(accountComboBox.currentIndex)
                                        accountComboBox.model = settingOperator.getUserDataList()
                                        refleshUserState()
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
                                    enabled: Settings.isLogin()
                                    onClicked: {
                                        Settings.logout()
                                        logoutButton.enabled = Settings.isLogin()
                                        refleshUserState()
                                        accountComboBox.model = settingOperator.getUserDataList()
                                    }
                                }
                            }
                        }
                    }
                    GroupBox {
                        Layout.fillWidth: true
                        title: "账号状态"
                        ColumnLayout {
                            width: parent.width
                            height: parent.height
                            Text {
                                id: currentUserValidText
                                text: "可用性:"
                                Rectangle {
                                    id: currentUserValidRectangle
                                    anchors.left: parent.right
                                    anchors.margins: 10
                                    height: parent.height
                                    width: height
                                    radius: 90
                                    color: settingOperator.isCurrentUserEmpty() ? "red" : (settingOperator.isCurrentUserValid() ? "green" : "yellow")
                                }
                            }
                            RowLayout {
                                Layout.fillWidth: true
                                Text {
                                    id: currentUserName
                                    Layout.fillWidth: true
                                    clip: true
                                }
                                Text {
                                    id: currentUserPassword
                                    Layout.fillWidth: true
                                    clip: true
                                }
                            }
                            RowLayout {
                                Layout.fillWidth: true
                                Text {
                                    id: currentUserSchoolId
                                    Layout.fillWidth: true
                                    clip: true
                                }
                                Text {
                                    id: currentUserStudentId
                                    Layout.fillWidth: true
                                    clip: true
                                }
                            }
                            Text {
                                id: currentUserAccessToken
                                Layout.fillWidth: true
                                clip: true
                            }
                            Text {
                                id: currentUserAuthorization
                                Layout.fillWidth: true
                                clip: true
                            }
                            Text {
                                id: currentUserClientSession
                                Layout.fillWidth: true
                                clip: true
                            }
                            Button {
                                id: currentUserRelogin
                                Layout.fillWidth: true
                                text: "重新登陆"
                                onClicked: {
                                    if(settingOperator.currentUserRelogin())
                                        messageDialog.show("重新登陆成功")
                                    else
                                        messageDialog.show("重新登陆失败")
                                    refleshUserState()
                                }
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
                            model: builtInStyles
                            onActivated: {
                                Settings.qmlStyle = styleComboBox.currentText
                            }

                            Component.onCompleted: {
                                styleIndex = find(Settings.qmlStyle, Qt.MatchFixedString)
                                if (styleIndex !== -1)
                                    currentIndex = styleIndex
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
                            model: Qt.fontFamilies()
                            delegate: ItemDelegate {
                                text: modelData
                                font: modelData
                                width: fontComboBox.width
                                onClicked: {
                                    fontComboBox.popup.close()
                                }
                            }

                            onActivated: {
                                Settings.font = currentText
                            }

                            Component.onCompleted: {
                                fontIndex = find(Settings.font, Qt.MatchFixedString)
                                if (fontIndex !== -1)
                                    currentIndex = fontIndex
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
                        text: "优先显示最新题卡(重启生效)"
                        checked: Settings.listLatestTemplatePreferentially
                        onCheckedChanged: {
                            Settings.listLatestTemplatePreferentially = checked
                        }
                    }
                }
            }
            GroupBox {
                Layout.fillWidth: true
                title: "扫码相关"
                Switch {
                    text: "压缩二维码图片"
                    checked: Settings.compressQRCodeImage
                    onCheckedChanged: {
                        Settings.compressQRCodeImage = checked
                    }
                }
            }
            GroupBox {
                Layout.fillWidth: true
                title: "文件"
                ColumnLayout {
                    width: parent.width
                    height: parent.height
                    Button {
                        Layout.fillWidth: true
                        text: "删除用户已导入题卡"
                        onClicked: {
                            settingOperator.deleteTemplateFile()
                            messageDialog.show("已删除用户已导入题卡\n" +
                                               "建议重启程序")
                        }
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
                            if(Qt.platform.os === "android")
                            {
                                checkUpdate.enabled = false
                                updateWidget.checkUpdate()
                            }
                            else
                            {
                                messageDialog.show("不支持当前系统自动更新")
                            }
                        }
                    }
                }
            }

            GroupBox {
                Layout.fillWidth: true
                title: "关于"
                RowLayout {
                    width: parent.width
                    height: parent.height
                    Button {
                        Layout.fillWidth: true
                        text: "软件信息"
                        onClicked: messageDialog.show("<p>使用 Qt 框架的跨平台软件,支持 Windows, Android, Linux</p>" +
                                                      "<p>本软件使用<strong>GNU Affero General Public License v3.0</strong>协议开源</p>" +
                                                      "<p>作者:LFWQSP2641( <a href=\"https://github.com/LFWQSP2641\">https://github.com/LFWQSP2641</a> )</p>" +
                                                      "<p>Copyright © 2022 - 2023 LFWQSP2641.All Rights Reserved.</p>" +
                                                      "<p>项目地址: <a href=\"https://github.com/LFWQSP2641/ZhiNengTiKa\">https://github.com/LFWQSP2641/ZhiNengTiKa</a></p>")
                    }
                    Button {
                        Layout.fillWidth: true
                        text: "使用的开源库"
                        onClicked: messageDialog.show("<p>Qt ( <a href=\"https://www.qt.io\">https://www.qt.io</a> )</p>" +
                                                      "<p><small>GNU (Lesser) General Public License v3.0</small></p>" +
                                                      "<br/>" +
                                                      "<p>ZXing-C++ ( <a href=\"https://github.com/zxing-cpp/zxing-cpp\">https://github.com/zxing-cpp/zxing-cpp</a> )</p>" +
                                                      "<p><small>Apache License 2.0</small></p>" +
                                                      "<br/>" +
                                                      "<p>android_openssl ( <a href=\"https://github.com/KDAB/android_openssl\">https://github.com/KDAB/android_openssl</a> )</p>" +
                                                      "<p><small>Apache License 2.0</small></p>" +
                                                      "<br/>" +
                                                      "<p>Qt-AES ( <a href=\"https://github.com/bricke/Qt-AES\">https://github.com/bricke/Qt-AES</a> )</p>" +
                                                      "<p><small>The Unlicense</small></p>")
                    }
                }
            }
        }
    }

    Dialog {
        id: messageDialog
        anchors.centerIn: parent
        modal: true
        focus: true

        contentItem: Text {
            id: messageDialogText
            wrapMode: Text.WordWrap
            onLinkActivated: function(link){ Qt.openUrlExternally(link) }
        }


        function show(caption) {
            messageDialogText.text = caption;
            messageDialog.open();
        }
    }

    function refleshUserState() {
        var userValid = settingOperator.isCurrentUserValid()
        currentUserValidRectangle.color = userValid ? "green" : "red"
        currentUserRelogin.visible = (!userValid) && Settings.isLogin()
        if(Settings.isLogin())
        {
            currentUserName.text = "账号:" + settingOperator.getCurrentUserUsername()
            currentUserPassword.text = "密码:****************"
            currentUserAccessToken.text = "AccessToken:" + settingOperator.getCurrentUserAccessToken()
            currentUserAuthorization.text = "Authorization:" + settingOperator.getCurrentUserAuthorization()
            currentUserSchoolId.text = "SchoolId:" + settingOperator.getCurrentUserSchoolId()
            currentUserStudentId.text = "StudentId:" + settingOperator.getCurrentUserStudentId()
            currentUserClientSession.text = "ClientSession:" + settingOperator.getCurrentUserClientSession()
        }
        else
        {
            currentUserName.text = "账号:****************"
            currentUserPassword.text = "密码:****************"
            currentUserAccessToken.text = "AccessToken:****************"
            currentUserAuthorization.text = "Authorization:****************"
            currentUserSchoolId.text = "SchoolId:****************"
            currentUserStudentId.text = "StudentId:****************"
            currentUserClientSession.text = "ClientSession:****************"
        }
    }

    Component.onCompleted: {
        refleshUserState()
    }

    Component.onDestruction: {
        Settings.saveToFile()
    }
}
