import QtQuick 2.4
import QtQuick.Window 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.0
import Client.Component 2.0 as Client

ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 500
    title: qsTr("Hello World")

    Client.Interaction{ id : interaction }

    Row {
        Client.Field {
            id: map
            width: 600
            height: 500
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
            Label {
                id: passed
                x: 0
                y: 270
                width: 200
                height: 50
                text: qsTr("Passed Time")
                font.pointSize: 11
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            Label {
                id: passedTime
                x: 0
                y: 320
                width: 200
                height: 30
                text: "0"
                horizontalAlignment: Text.AlignHCenter
                Timer {
                    id: timer2
                    interval: 100
                    running: true
                    repeat: true
                    onTriggered: {
                        passedTime.text = "" + interaction.currentTime()
                    }
                }
            }
        }
    }

    Button {
        id: button
        x: 600
        y: 460
        width: 100
        height: 40
        text: qsTr("Rain Mode")
    }

    Button {
        id: button1
        x: 700
        y: 460
        width: 100
        height: 40
        text: qsTr("Busy Mode")
    }
}
