import QtQuick 2.4
import QtQuick.Window 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.0
import Client.Component 2.0 as Client

ApplicationWindow {
    id: root
    visible: true
    width: 840
    height: 480
    title: qsTr("Hello World")

    Client.Interaction{ id : interaction }

    Row {
        Client.Field {
            id: map
            width: 640
            height: 480
        }
        Column {
            Button {
                id: start
                width: 200
                height: 200
                Label {
                    id: buttonLabel
                    width: 200
                    height: 200
                    text: "start"
                    font.pointSize: 15
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                onClicked: {
                    interaction.setVision();
                }
            }
            Label {
                id: finished
                x: 0
                y: 200
                text: "finished case number"
                font.pointSize: 11
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                width: 200
                height: 50
            }
            Label {
                id: finishedCaseNum
                width: 200
                height: 20
                text: "0"
                horizontalAlignment: Text.AlignHCenter
                Timer {
                    id: timer1
                    interval: 50
                    running: true
                    repeat: true
                    onTriggered: {
                        finishedCaseNum.text = "" + interaction.finishedNum()
                    }
                }
            }
        }
    }
}
