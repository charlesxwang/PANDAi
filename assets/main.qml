import QtQuick 2.0
import QtQuick.Window 2.1

Window {
    id: menulist
    width: 600
    height: 300
    visible: true


    Rectangle {
        id: info
        anchors.centerIn: parent
        visible: false

        width: parent.width/2
        height: 100
        z: 50
        border.width: 2
        border.color: "#363636"
        radius: 10

        Text {
            id: message
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
            wrapMode: Text.WordWrap
            elide: Text.ElideMiddle
            text: blueLinker.message
            color: "#363636"
        }
    }

    Component.onCompleted: pageLoader.source = "Menu.qml"

    Loader {
        id: pageLoader
        anchors.fill: parent
    }
}
