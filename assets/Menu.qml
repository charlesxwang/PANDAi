import QtQuick 2.0

Rectangle {
    width: 600
    height: 300


    Rectangle {
        width: parent.width
        height: 70
        border.width: 1
        border.color: "#363636"
        radius: 5

        Text {
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
            text: "Welcome to PANDAi \n Please select an option"
            font.pixelSize: 20
            elide: Text.ElideMiddle
            color: "#363636"
        }
    }

    Rectangle {
        id: startServer
        anchors.centerIn: parent
        width: parent.width/2
        height: parent.height/5
        color: "#363636"

        Text {
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
            font.bold: true
            text: "Start PANDAi server"
            color: "#E3E3E3"
            wrapMode: Text.WordWrap
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                pageLoader.source = "Board.qml";
                blueLinker.startServer();
                //handdetector.run();
            }
        }
    }

    Rectangle {
        id: startClient
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: startServer.bottom
        anchors.topMargin: 10
        width: parent.width/2
        height: parent.height/5
        color: "#363636"

        Text {
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
            font.bold: true
            text: "Start PANDAi client"
            color: "#E3E3E3"
            wrapMode: Text.WordWrap
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                pageLoader.source = "Board.qml";
                blueLinker.startClient()
            }
        }
    }
}
