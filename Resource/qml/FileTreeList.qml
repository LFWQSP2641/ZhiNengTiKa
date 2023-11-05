import QtQuick
import QtQuick.Controls
import FileTreeModel

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
            onTapped: {
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
        rootPath: "C:/Users/TARDIS2641/Documents/Qt/build-ZhiNengTiKaQML-Desktop_Qt_6_5_3_MSVC2019_64bit-Release/release"
    }
}
