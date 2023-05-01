import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import UpdateChecker

Item {
    property bool showQuestionDialog: false
    signal finished()

    UpdateChecker {
        id: updateChecker
        onCheckFinished: function(hasNewVersion) {
            if(hasNewVersion)
            {
                updateDialog.open()
            }
            else if(showQuestionDialog)
            {
                questionDialog.open()
            }
            finished()
        }
        onDownloadProgress: function(bytesReceived,bytesTotal) {
            downloadProgressDialogProgressBar.to = bytesTotal
            downloadProgressDialogProgressBar.value = bytesReceived
        }
        onDownloadFinished: {
            updateChecker.installNewestVersion()
        }
    }

    // 有更新
    Dialog {
        id: updateDialog
        modal: true
        focus: true

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            downloadProgressDialog.open()
            updateDialog.close()
            updateChecker.start()
        }

        onRejected: {
            updateDialog.close()
            finished()
        }

        contentItem: ScrollView {
            Text {
                id: updateDialogText
            }
        }

        onOpened: {
            updateDialogText.text =
                    "当前版本: " + updateChecker.getCurrentVersion() + "\n"
                    + "最新版本: " + updateChecker.getNewestVersion() + "\n\n"
                    + updateChecker.getChangeLog()
        }
    }
    // 无更新,但询问
    Dialog {
        id: questionDialog
        modal: true
        focus: true

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            updateDialog.open()
            questionDialog.close()
        }

        onRejected: {
            questionDialog.close()
            finished()
        }

        contentItem: Text {
            id: questionDialogText

        }

        onOpened: {
            questionDialogText.text =
                    "当前版本: " + updateChecker.getCurrentVersion() + "\n"
                    + "最新版本: " + updateChecker.getNewestVersion() + "\n"
                    + "已是最新版本\n"
                    + "是否继续更新?"
        }
    }
    // 下载进度
    Dialog {
        id: downloadProgressDialog
        modal: true
        focus: true

        standardButtons: Dialog.NoButton

        contentItem: ProgressBar {
            id: downloadProgressDialogProgressBar
            from: 0
        }
    }

    function checkUpdate() {
        updateChecker.start()
    }
    function checkMinimumVersion() {
        return updateChecker.checkMinimumVersion()
    }
}
