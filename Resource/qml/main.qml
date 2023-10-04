import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtSensors
import Qt.labs.platform
import TemplateRawDataQML
import MultipleSubjectsTemplateListModelList
import QMLUtils
import ZAccelerationToOpacityConverter

ApplicationWindow {
    id: applicationWindow
    width: 480
    height: 640
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
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: Image {
            id: backgroundImage
            asynchronous: true
            cache: false
            mipmap: true
            fillMode: Image.PreserveAspectFit
            source: "image://AnimeImageProvider/image"
            Item {
                id: buttonsItem
                anchors.fill: parent
                opacity: 0.5
                NumberAnimation {
                    id: buttonsItemOpacityAnimation
                    target: buttonsItem
                    property: "opacity"
                    duration: zAccelerationReportTimer.interval - 10
                    easing.type: Easing.Linear
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
                            onClickedLeft: stackView.push(selectWidgetComponent)
                        }

                        IconButton {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            radius: 45
                            widgetSizeRatio: 0.5
                            iconSource: "qrc:/svg/icon/search.svg"
                            buttonText: "搜素"
                            backgroundColor: QMLUtils.generateRandomBrightColor()
                            onClickedLeft: stackView.push(searchWidgetComponent)
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
        }
        onDepthChanged: {
            if(stackView.depth === 1)
            {
                if(imageRefreshTimer.needToRefresh)
                    refreshImage()
                zAccelerationReportTimer.start()
            }
            else
            {
                zAccelerationReportTimer.stop()
            }
        }
    }

    Timer {
        id: imageRefreshTimer
        property bool needToRefresh: false
        running: true
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
            }
        }
    }

    Accelerometer {
        id: accelerometer
        property real x: 0
        property real y: 0
        property real z: 5
        active: true
        onReadingChanged: {
            x = (reading as AccelerometerReading).x
            y = (reading as AccelerometerReading).y
            z = (reading as AccelerometerReading).z
        }
    }

    Timer {
        id: zAccelerationReportTimer
        running: true
        repeat: true
        interval: 100
        onTriggered: {
            if(stackView.depth === 1)
                converter.trySetZAcceleration(accelerometer.z)
        }
    }

    ZAccelerationToOpacityConverter {
        id: converter
        onOpacityChanged: function(newOpacity) {
            buttonsItemOpacityAnimation.to = newOpacity
            buttonsItemOpacityAnimation.restart()
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
        anchors.centerIn: parent
        width: parent.width
        showQuestionDialog: false
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
            Component.onCompleted: {
            }
            Component.onDestruction: {
            }
            onScanFinished: function(templateCode){
                showTemplateDetailWidgetAndPop(templateCode)
            }
        }
    }

    Component.onCompleted: {
        converter.start()
        updateWidget.checkUpdate()
        if(!updateWidget.checkMinimumVersion())
        {
            applicationWindowQuitDialog.open()
        }
    }
    Component.onDestruction: {
        converter.stop()
        converter.wait()
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
            MultipleSubjectsTemplateListModelList.addNewTemplate(templateRawDataQML)
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
            MultipleSubjectsTemplateListModelList.addNewTemplate(templateRawDataQML)
        }

        stackView.pop()
        stackView.push(templateDetailWidgetComponent)
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
    }
}
