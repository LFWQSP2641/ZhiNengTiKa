import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Dialog {
    anchors.centerIn: parent
    focus: true

    standardButtons: Dialog.Ok

    contentItem: Text {
        id: messageDialogText
        wrapMode: Text.Wrap
    }

    function show(caption) {
        messageDialogText.text = caption;
        messageDialog.open();
    }
}
