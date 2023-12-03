import QtQuick
import QtQuick.Controls
import FileTreeModel
import QMLUtils

TreeView {
    signal fileClicked(string filePath)
    property alias rootPath: fileTreeModel.rootPath
    property string toBeRemoved: ""
    delegate: TreeViewDelegate {
        TapHandler {
            acceptedButtons: Qt.LeftButton
            onTapped: {
                if(!hasChildren)
                    fileClicked(absoluteFilePath)
            }
        }
        TapHandler {
            acceptedButtons: Qt.RightButton
            acceptedDevices: PointerDevice.Mouse
            onTapped: {
                toBeRemoved = absoluteFilePath
                menu.popup()
            }
        }
        TapHandler {
            acceptedDevices: PointerDevice.TouchScreen
            onLongPressed: {
                toBeRemoved = absoluteFilePath
                menu.popup()
            }
        }
    }
    model: fileTreeModel
    Menu {
        id: menu
        MenuItem {
            text: "删除"
            onTriggered: {
                if(toBeRemoved.length != 0)
                    fileTreeModel.removeFile(toBeRemoved)
            }
        }
        onAboutToHide: toBeRemoved = ""
    }

    FileTreeModel {
        id: fileTreeModel
        rootPath: QMLUtils.getResourceFilePath()
    }
}
