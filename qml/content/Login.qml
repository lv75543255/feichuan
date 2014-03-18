import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {
    id:root
    width: 320
    height: 640

    signal login(string nickname,string groupname)
    signal cancle()

    RowLayout {
        id: columnLayout1
        x: 54
        y: 177
        width: 213
        height: 41

        Label {
            id: label1

            text: qsTr("Ｎickname")
        }

        TextField {
            id: textField1
            anchors.left: parent.left
            anchors.leftMargin: 80

            placeholderText: qsTr("nickname")
        }
    }

    RowLayout {
        id: rowLayout1
        x: 54
        y: 243
        width: 213
        height: 34

        Label {
            id: label2
            text: qsTr("GroupName")
        }


        TextField {
            id: textField2
            anchors.left: parent.left
            anchors.leftMargin: 80
            placeholderText: qsTr("groupname")
        }
    }

    RowLayout {
        id: rowLayout2
        x: 54
        y: 372
        width: 213
        height: 57

        Button {
            id: button1
            text: qsTr("Login")
            anchors.left: parent.left
            anchors.leftMargin: 20
            onClicked: login(textField1,textField2)
        }

        Button {
            id: button2
            text: qsTr("Cancle")
            onClicked: cancle()
        }
    }

}
