import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import TemplateFetcher
import MultipleSubjectsTemplateListModelList
import QMLUtils
import ZAccelerationToOpacityConverter
import AnimeImageProvider

ApplicationWindow {
    id: applicationWindow
    width: 480
    height: 640
    visible: true
    title: "智能题卡"

    required property var builtInStyles
    property bool waitingForTemplateDetailWidget: false

    Shortcut {
        sequences: ["Esc", "Back"]
        enabled: stackView.depth > 1
        onActivated: navigateBackAction.trigger()
    }

    Action {
        id: navigateBackAction
        icon.source: "qrc:/svg/icon/angle-left.svg"
        onTriggered: {
            if (stackView.depth > 1) {
                stackView.pop()
            }
        }
    }

    header: ToolBar {
        visible: stackView.depth > 1
        ToolButton {
            id: navigateBackButton
            action: navigateBackAction
            anchors.left: parent.left
            anchors {top: parent.top; left: parent.left; bottom: parent.bottom}
        }
        Text {
            id: pageDescription
            anchors {left: navigateBackButton.right; right: parent.right}
            anchors.verticalCenter: parent.verticalCenter
        }
        Component.onCompleted: {
            console.log(background.color.hslLightness)
            if(background.color.hslLightness > 0.5)
                pageDescription.color = "black"
            else
                pageDescription.color = "white"
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: Item {
            Image {
                id: backgroundImage
                property bool rotated: false
                anchors.centerIn: parent
                height: rotated ? parent.width : parent.height
                width: rotated ? parent.height : parent.width
                z: parent.z - 1
                rotation: rotated ? 90 : 0
                asynchronous: true
                cache: false
                mipmap: true
                fillMode: Image.PreserveAspectFit
                source: "image://AnimeImageProvider/image"
                onStatusChanged: {
                    if (backgroundImage.status === Image.Ready)
                    {
                        rotated = (parent.height > parent.width && paintedHeight < paintedWidth) || (parent.height < parent.width && paintedHeight > paintedWidth)
                    }
                }
            }
            Item {
                id: buttonsItem
                anchors.fill: parent
                opacity: 0.5
                NumberAnimation {
                    id: buttonsItemOpacityAnimation
                    target: buttonsItem
                    property: "opacity"
                    duration: converter.interval > 10 ? (converter.interval / 10 * 9) : 250
                    easing.type: Easing.InOutQuad
                }
                ColumnLayout {
                    anchors.centerIn: parent
                    height: parent.height - 10
                    width: parent.width - 10
                    IconButton {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.preferredHeight: parent.height * 0.67
                        radius: 45
                        widgetSizeRatio: 0.5
                        iconSource: "qrc:/svg/icon/qrcode.svg"
                        buttonText: "扫码"
                        backgroundColor: QMLUtils.generateRandomBrightColor()
                        onClickedLeft: stackView.push(qrCodeScannerWidgetComponent)
                    }
                    RowLayout {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.preferredHeight: parent.height * 0.33
                        IconButton {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            radius: 45
                            widgetSizeRatio: 0.5
                            iconSource: "qrc:/svg/icon/list.svg"
                            buttonText: "列表"
                            backgroundColor: QMLUtils.generateRandomBrightColor()
                            onClickedLeft: {
                                stackView.push(selectWidgetComponent)
                            }
                        }
                        IconButton {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            radius: 45
                            widgetSizeRatio: 0.5
                            iconSource: "qrc:/svg/icon/document.svg"
                            buttonText: "资源"
                            backgroundColor: QMLUtils.generateRandomBrightColor()
                            onClickedLeft: stackView.push(resourceFileWidgetComponent)
                        }
                        IconButton {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            radius: 45
                            widgetSizeRatio: 0.5
                            iconSource: "qrc:/svg/icon/search.svg"
                            buttonText: "搜素"
                            backgroundColor: QMLUtils.generateRandomBrightColor()
                            onClickedLeft: {
                                stackView.push(searchWidgetComponent)
                            }
                        }
                        IconButton {
                            Rectangle {
                                id: unreadStateRectangle
                                anchors {right: parent.right; top: parent.top}
                                width: 10
                                height: width
                                radius: 90
                                visible: false
                                color: "red"
                            }
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            radius: 45
                            widgetSizeRatio: 0.5
                            iconSource: "qrc:/svg/icon/megaphone.svg"
                            buttonText: "公告"
                            backgroundColor: QMLUtils.generateRandomBrightColor()
                            onClickedLeft: {
                                stackView.push(announcementListView)
                            }
                        }
                        IconButton {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            radius: 45
                            widgetSizeRatio: 0.5
                            iconSource: "qrc:/svg/icon/settings.svg"
                            buttonText: "设置"
                            backgroundColor: QMLUtils.generateRandomBrightColor()
                            onClickedLeft: {
                                enabled = false
                                stackView.push("qrc:/qml/SettingWidget.qml", {builtInStyles: applicationWindow.builtInStyles})
                                enabled = true
                            }
                        }
                    }
                }
            }
            onHeightChanged: {
                backgroundImage.rotated = (height > width && backgroundImage.paintedHeight < backgroundImage.paintedWidth) || (height < width && backgroundImage.paintedHeight > backgroundImage.paintedWidth)
            }
            onWidthChanged: {
                backgroundImage.rotated = (height > width && backgroundImage.paintedHeight < backgroundImage.paintedWidth) || (height < width && backgroundImage.paintedHeight > backgroundImage.paintedWidth)
            }
        }
        onDepthChanged: {
            if(stackView.depth === 1)
            {
                if((!waitingForTemplateDetailWidget) && imageRefreshTimer.needToRefresh)
                {
                    refreshImage()
                    imageRefreshTimer.start()
                }
                converter.start()
            }
            else
            {
                converter.stop()
            }
        }
    }

    Timer {
        id: imageRefreshTimer
        property bool needToRefresh: false
        repeat: true
        interval: 5000
        onTriggered: {
            if(stackView.depth === 1)
            {
                refreshImage()
            }
            else
            {
                imageRefreshTimer.needToRefresh = true
                imageRefreshTimer.stop()
            }
        }
    }

    Connections {
        target: AnimeImageProvider
        function onFirstImageCached() {
            if(stackView.depth === 1)
            {
                refreshImage()
            }
            else
            {
                imageRefreshTimer.needToRefresh = true
            }
            imageRefreshTimer.start()
        }
    }

    Connections {
        target: AnimeImageProvider
        function onCacheProgress(current, total) {
            console.log("AnimeImageProvider:" + current + "/" + total)
        }
    }

    ZAccelerationToOpacityConverter {
        id: converter
        onOpacityChanged: function(newOpacity) {
            buttonsItemOpacityAnimation.to = newOpacity
            buttonsItemOpacityAnimation.restart()
        }
    }

    NormalMessageDialog {
        id: messageDialog
    }

    NormalMessageDialog {
        id: applicationWindowQuitDialog
        onAboutToHide: Qt.exit(1)
    }

    TemplateFetcher {
        id: templateFetcher
        onError: function(msg) {
            messageDialog.close()
            messageDialog.show(msg)
        }
        onTemplateAnalysisReady: function(object) {
            if(object.network)
            {
                messageDialog.close()
            }
            MultipleSubjectsTemplateListModelList.addNewTemplate(object)
            stackView.push(templateDetailWidgetComponent, {templateAnalysisPointer: object})
        }
        onObtainTemplateFromNetwork: {
            messageDialog.show("获取中...")
        }
    }

    AnnouncementWidget {
        id: announcementListView
        visible: false
        onManagerError: function(msgStr) {
            messageDialog(msgStr)
        }
        onManagerObtainFinished: function(newCount) {
            if(newCount > 0)
            {
                stackView.push(announcementListView)
            }
        }
        onUnreadCountChanged: {
            unreadStateRectangle.visible = (announcementListView.unreadCount != 0)
        }
    }

    // stackView.push在第二个参数连接信号槽好像有点问题
    // TypeError: Cannot assign to read-only property "okButtonClicked"
    // 所以拿个Component包一下
    Component {
        id: selectWidgetComponent
        SelectWidget {
            id: selectWidget
            onTemplateCodeObtainFinished: function(templateCode){
                templateFetcher.handleTemplateRequestByCode(templateCode)
            }
            onTemplateSummaryObtainFinished: function(templateSummary){
                templateFetcher.handleTemplateRequest(templateSummary)
            }
        }
    }

    Component {
        id: searchWidgetComponent
        SearchWidget {
            onTemplateSummaryObtainFinished: function(templateSummary){
                templateFetcher.handleTemplateRequest(templateSummary)
            }
        }
    }

    Component {
        id: resourceFileWidgetComponent
        Item {
            ColumnLayout {
                anchors.fill: parent
                IconButton {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    widgetSizeRatio: 0.5
                    iconSource: "qrc:/svg/icon/disk.svg"
                    buttonText: "本地"
                    onClickedLeft: stackView.push(fileTreeListComponent, {"rootPath": QMLUtils.getResourceFilePath()})
                }
                IconButton {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    widgetSizeRatio: 0.5
                    iconSource: "qrc:/svg/icon/download.svg"
                    buttonText: "下载"
                    onClickedLeft: stackView.push(downloadResourceFileWidget)
                }
            }
        }
    }

    Component {
        id: downloadResourceFileWidget
        DownloadResourceFileWidget {
            onDownloadFinished: function(path) {
                stackView.push(fileTreeListComponent, {"rootPath": path})
            }
        }
    }

    Component {
        id: fileTreeListComponent
        FileTreeList {
            onFileClicked: function(filePath) {
                console.log(filePath)
                if(filePath.endsWith("pdf"))
                {
                    stackView.push(pdfReader, {"source": "file:///" + filePath})
                }
            }
        }
    }

    Component {
        id: pdfReader
        Item {
            property string source: ""
            Text {
                anchors.centerIn: parent
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: "pdf显示暂未实现"
            }
        }
    }

    Component {
        id: templateDetailWidgetComponent
        TemplateDetailWidget {
            Component.onCompleted: {
                if(templateAnalysisPointer !== null)
                    pageDescription.text = templateAnalysisPointer.templateName
                waitingForTemplateDetailWidget = false
            }
            Component.onDestruction: {
                pageDescription.text = ""
            }
        }
    }

    Component {
        id: qrCodeScannerWidgetComponent
        QRCodeScannerWidget {
            id: qrCodeScannerWidget
            Component.onCompleted: {
            }
            Component.onDestruction: {
            }
            onScanFinished: function(templateCode){
                waitingForTemplateDetailWidget = true
                stackView.pop()
                templateFetcher.handleTemplateRequestByCode(templateCode)
            }
        }
    }

    Component.onCompleted: {
        converter.start()
    }
    Component.onDestruction: {
        converter.stop()
        converter.wait()
    }

    function stackViewPopAll() {
        while(stackView.depth > 1)
        {
            stackView.pop()
        }
    }
    function refreshImage() {
        backgroundImage.source = ""
        backgroundImage.source = "image://AnimeImageProvider/image"
        imageRefreshTimer.needToRefresh = false
    }
}
