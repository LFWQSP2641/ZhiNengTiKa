import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import ResourceFileFetcher

Item {
    ResourceFileFetcher {
        id: resourceFileFetcher
        onInitFinished: {
            console.log("初始化完成")
            busyIndicator.running = false
            subjectsTabBarModel.model = resourceFileFetcher.getSubjectModel()
            settingItemsLayout.enabled = true
        }

        onResetModelFinished: {
            resourceFileListView.footerItem.visible = resourceFileFetcher.haveNextPage()
            settingItemsLayout.enabled = true
        }

        onContinueLoadModelFinished: {
            resourceFileListView.footerItem.visible = resourceFileFetcher.haveNextPage()
        }

        onError: function(msg) {
            console.log("error 发生:\n" + msg)
        }

        onDownloadResourceFileFinished: function(fileName) {
            busyIndicator.running = false
            settingItemsLayout.enabled = true
            fileDialog.currentFile = fileName
            fileDialog.open()
        }
    }

    BusyIndicator {
        id: busyIndicator
        running: true
        anchors.centerIn: parent
    }

    FileDialog {
        id: fileDialog
        fileMode: FileDialog.SaveFile
        title: "保存文件"
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)

        onAccepted: {
            resourceFileFetcher.saveToFile(file)
        }
    }

    ColumnLayout {
        id: settingItemsLayout
        enabled: false
        anchors.fill: parent
        TabBar {
            id: subjectsTabBar
            Layout.fillWidth: true
            Repeater {
                id: subjectsTabBarModel
                TabButton {
                    text: modelData
                }
            }
            onCurrentItemChanged: {
                if(currentItem == null)
                    return
                console.log(currentItem.text)
                editionTabBarModel.model = null
                editionTabBarModel.model = resourceFileFetcher.getEditionModel(currentItem.text)
            }
        }
        TabBar {
            id: editionTabBar
            Layout.fillWidth: true
            Repeater {
                id: editionTabBarModel
                TabButton {
                    text: modelData
                }
            }
            onCurrentItemChanged: {
                if(currentItem == null)
                    return
                console.log(currentItem.text)
                moduleTabBarModel.model = null
                moduleTabBarModel.model = resourceFileFetcher.getModuleModel(subjectsTabBar.currentItem.text, currentItem.text)
            }
        }
        TabBar {
            id: moduleTabBar
            Layout.fillWidth: true
            Repeater {
                id: moduleTabBarModel
                TabButton {
                    text: modelData
                }
            }
            onCurrentItemChanged: {
                if(currentItem == null)
                    return
                console.log(currentItem.text)
                settingItemsLayout.enabled = false
                resourceFileFetcher.resetModel(subjectsTabBar.currentItem.text, editionTabBar.currentItem.text, moduleTabBar.currentItem.text)
            }
        }
        ListView {
            id: resourceFileListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: resourceFileFetcher.model
            clip: true
            delegate: ItemDelegate {
                id: itemDelegate
                width: resourceFileListView.width
                text: model.display
                highlighted: ListView.isCurrentItem
                onClicked: {
                    if(index === resourceFileListView.currentIndex)
                    {
                        settingItemsLayout.enabled = false
                        busyIndicator.running = true
                        resourceFileFetcher.downloadResourceFile(index)
                    }
                    resourceFileListView.currentIndex = index
                }
            }
            footer: Rectangle {
                id: continueLoadModelRectangle
                width: resourceFileListView.width
                height: 50
                visible: false
                Text {
                    anchors.centerIn: parent
                    text: "继续加载"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        continueLoadModelRectangle.visible = false
                        resourceFileFetcher.continueLoadModel()
                    }
                }
            }
            onCurrentItemChanged: {
                console.log(currentIndex)
            }
        }
    }

    Component.onCompleted: {
        resourceFileFetcher.init()
    }
}
