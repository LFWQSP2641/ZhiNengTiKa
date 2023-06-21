import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import UpdateChecker

// TODO Dialog无法显示在控件中心
Dialog {
    id: updateDialog
    anchors.centerIn: parent
    modal: true
    focus: true
    property bool showQuestionDialog: false
    signal finished()

    UpdateChecker {
        id: updateChecker
        onCheckFinished: function(hasNewVersion) {
            if(hasNewVersion||showQuestionDialog)
            {
                updateDialog.open()
            }
            finished()
        }
        onDownloadProgress: function(bytesReceived,bytesTotal) {
            downloadProgressDialogProgressBar.value = bytesReceived
        }
        onDownloadFinished: {
            updateChecker.installNewestVersion()
            downloadProgressDialogProgressBar.indeterminate = true
        }
    }

    contentItem: ColumnLayout {
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentHeight: updateDialogText.height
            // 关闭滚动条
            // QML ScrollBar: Binding loop detected for property "visible"
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AlwaysOff
            clip: true
            Text {
                id: updateDialogText
                wrapMode: Text.WordWrap
                onLinkActivated: function(link){ Qt.openUrlExternally(link) }
            }
        }
        ProgressBar {
            id: downloadProgressDialogProgressBar
            Layout.fillWidth: true
            visible: false
            from: 0
            to: 30000000
        }
        RowLayout {
            Layout.fillWidth: true
            Button {
                id: downloadButton
                text: "下载"
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 3
                onClicked: {
                    parent.visible = false
                    downloadProgressDialogProgressBar.visible = true
                    updateChecker.downloadNewestVersion()
                }
            }
            Button {
                id: rejectButton
                text: "下次一定!"
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
                onClicked: updateDialog.close()
            }
        }
    }

    onOpened: {
        updateDialogText.text =
                "当前版本: " + updateChecker.getCurrentVersion() + "\n"
                + "最新版本: " + updateChecker.getNewestVersion() + "\n\n"
                + updateChecker.getChangeLog()
    }

    function checkUpdate() {
        if(Qt.platform.os === "android")
        {
            updateChecker.start()
        }
    }
    function checkMinimumVersion() {
        return updateChecker.checkMinimumVersion()
    }
}
