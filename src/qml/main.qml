import QtQuick 2.4
import QtQuick.Window 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.0
import Client.Component 1.0 as Client

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
        Button {
            id: start
            text: "start"
            width: 200
            height: 200
            onClicked: {
                interaction.setVision();
            }
        }
    }
}
