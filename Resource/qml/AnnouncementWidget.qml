import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import AnnouncementManager

ListView {
    id: announcementListView

    signal managerObtainFinished(bool hasNew)
    signal managerError(string msgStr)

    clip: true
    model: manager.announcementModel
    delegate: ItemDelegate {
        id: itemDelegate
        width: announcementListView.width
        height: titleRectangle.height
        ColumnLayout {
            width: itemDelegate.width
            Rectangle {
                id: titleRectangle
                Layout.fillWidth: true
                color: "grey"
                height: titleText.contentHeight + 10
                Text {
                    id: titleText
                    anchors.centerIn: parent
                    text: title
                    wrapMode: Text.WordWrap
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if(detailText.visible)
                        {
                            itemDelegate.height = Qt.binding(function() { return titleRectangle.height })
                            detailText.visible = false
                        }
                        else
                        {
                            itemDelegate.height = Qt.binding(function() { return titleRectangle.height + detailText.height + 10 })
                            detailText.visible = true
                        }
                        if(!read)
                        {
                            unreadStateRectangle.visible = false
                            manager.markAnnouncementAsRead(index)
                        }
                    }
                }
                Rectangle {
                    id: unreadStateRectangle
                    anchors {right: parent.right; top: parent.top}
                    width: 10
                    height: width
                    radius: 90
                    visible: !read
                    color: "red"
                }
            }
            Text {
                id: detailText
                Layout.fillWidth: true
                text: detail
                wrapMode: Text.WordWrap
                visible: false
            }
        }
    }

    AnnouncementManager {
        id: manager
        onError: function(msgStr) {
            managerError(msgStr)
        }
        onObtainFinished: function(hasNew) {
            managerObtainFinished(hasNew)
        }
    }
    Component.onCompleted: {
        manager.obtainAnnouncement()
    }
}
