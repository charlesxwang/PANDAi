import QtQuick 2.0
import QtQuick.Window 2.1

Rectangle {
    id: board
    width: 600
    height: 300



    property bool deviceMessage: blueLinker.showDialog
    onDeviceMessageChanged: {
        if (blueLinker.showDialog) {
            info.visible = true;
            board.opacity = 0.5;
        } else {
            info.visible = true;
            board.opacity = 0.5;
            //info.visible = false;
            //board.opacity = 1;
        }
    }








    Component.onCompleted: {


    }
}
