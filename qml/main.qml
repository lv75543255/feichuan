import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.1
import "image"
import "content"
import "content/main.js" as Logic
ApplicationWindow {
    id: mainWindow
    title: qsTr("Qiuge")
    width: 320
    height: 640

    Item{
        id:mainArea
        anchors.fill: parent
    }

    toolBar: ToolBar {
        id:toolbar
        RowLayout{
            anchors.fill: parent

            ToolButton{
                iconSource: "qq.ico"
            }

            Label{
                text:qsTr("Un login")
            }

            ComboBox{

            }

        }
    }

    Component.onCompleted:{
        Logic.login();
    }

}
