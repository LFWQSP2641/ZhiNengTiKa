import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SettingOperator
import Settings
import AccountManager

Item {
    required property var builtInStyles

    function displayCurrentUserData() {
        if(AccountManager.isLoggedin())
        {
            currentUserName.text = "账号:" + AccountManager.getCurrentUserData().username
            currentUserPassword.text = "密码:****************"
            currentUserAccessToken.text = "AccessToken:" + AccountManager.getCurrentUserData().accessToken
            currentUserAuthorization.text = "Authorization:" + AccountManager.getCurrentUserData().authorization
            currentUserSchoolId.text = "SchoolId:" + AccountManager.getCurrentUserData().schoolId
            currentUserStudentId.text = "StudentId:" + AccountManager.getCurrentUserData().studentId
            currentUserClientSession.text = "ClientSession:" + AccountManager.getCurrentUserData().clientSession
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

    function accountControlsEnabled(enabled) {
        accountComboBox.enabled = enabled
        loginButton.enabled = enabled
        logoutButton.enabled = enabled && AccountManager.isLoggedin()
        currentUserRelogin.enabled = enabled
    }

    SettingOperator {
        id: settingOperator
    }
    Connections {
        target: AccountManager
        function onLoginFinished(success, object) {
            messageDialog.close()
            if(success)
            {
                accountComboBox.model = AccountManager.getDescriptionOfUserDatas()
                logoutButton.enabled = true
                AccountManager.checkCurrentAccountValid()
                accountControlsEnabled(false)
            }
            else
            {
                messageDialog.show("登录失败,请确认账号密码是否正确")
                accountControlsEnabled(true)
            }
        }
        function onReloginFinished(success, object) {
            if(success)
                messageDialog.show("重新登陆成功")
            else
                messageDialog.show("重新登陆失败")
            AccountManager.checkCurrentAccountValid()
        }
        function onCheckCurrentAccountValidFinished(valid) {
            currentUserValidRectangle.color = valid ? "green" : "red"
            accountComboBox.enabled = true
            currentUserRelogin.visible = (!valid) && AccountManager.isLoggedin()
            displayCurrentUserData()
            accountControlsEnabled(true)
        }
        function onError(msg) {
            messageDialog.show(msg)
            accountControlsEnabled(true)
        }
    }

    Dialog {
        id: loginDialog
        width: parent.width
        anchors.centerIn: parent
        focus: true

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            AccountManager.login(userIdTextField.text, userPwTextField.text)
            accountControlsEnabled(false)
            loginDialog.close()
        }
        onRejected: {
            userIdTextField.clear()
            userPwTextField.clear()
            loginDialog.close()
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

    Dialog {
        id: setImageUrlDialog
        width: parent.width
        anchors.centerIn: parent
        focus: true

        standardButtons: Dialog.Ok | Dialog.Cancel
        contentItem: ColumnLayout {
            width: parent.width
            height: parent.height
            ComboBox {
                id: imageNameComboBox
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: settingOperator.getAnimeImageNameList()
                currentIndex: settingOperator.getCurrentAnimeImageNameIndex()
                onActivated: {
                    imageUrlTextField.text = settingOperator.getAnimeImageUrl(currentIndex)
                }
            }
            TextField {
                id: imageUrlTextField
                Layout.fillWidth: true
                placeholderText: "图片url"
                text: Settings.animeImageUrl
            }
        }
        onAccepted: {
            Settings.animeImageUrl = imageUrlTextField.text
            imageNameComboBox.currentIndex = settingOperator.getCurrentAnimeImageNameIndex()
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
                                model: AccountManager.getDescriptionOfUserDatas()
                                enabled: false

                                onActivated: {
                                    if(accountComboBox.currentIndex > 0)
                                    {
                                        currentUserValidRectangle.color = "blue"
                                        AccountManager.toFirst(accountComboBox.currentIndex)
                                        accountComboBox.model = AccountManager.getDescriptionOfUserDatas()
                                        AccountManager.checkCurrentAccountValid()
                                        accountControlsEnabled(false)
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
                                        loginDialog.open()
                                    }
                                }
                                Button {
                                    id:logoutButton
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    text: "登出"
                                    enabled: AccountManager.isLoggedin()
                                    onClicked: {
                                        AccountManager.logout()
                                        logoutButton.enabled = AccountManager.isLoggedin()
                                        accountComboBox.model = AccountManager.getDescriptionOfUserDatas()
                                        AccountManager.checkCurrentAccountValid()
                                        accountControlsEnabled(false)
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
                                    color: "blue"
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
                                visible: false
                                Layout.fillWidth: true
                                text: "重新登陆"
                                onClicked: {
                                    AccountManager.relogin()
                                    accountControlsEnabled(false)
                                }
                            }
                        }
                    }
                }
            }
            GroupBox {
                Layout.fillWidth: true
                title: "外观"
                ColumnLayout {
                    width: parent.width
                    height: parent.height
                    GroupBox {
                        Layout.fillWidth: true
                        title: "重启生效"
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
                                    font: currentText.length == 0 ? undefined : currentText
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
                        title: "即时生效"
                        ColumnLayout {
                            width: parent.width
                            height: parent.height
                            Button {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                text: "设置图片源"
                                onClicked: setImageUrlDialog.open()
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

    Component.onCompleted: {
        AccountManager.checkCurrentAccountValid()
        accountControlsEnabled(false)
    }

    Component.onDestruction: {
        Settings.saveToFile()
    }
}
