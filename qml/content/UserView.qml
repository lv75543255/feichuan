import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
ListView{

    delegate: Item {

    }

    model:ListModel {
        ListElement {
            icon: "Buttons"
            name: "content/ButtonPage.qml"
        }
        ListElement {
            icon: "Sliders"
            name: "content/SliderPage.qml"
        }
        ListElement {
            icon: "ProgressBar"
            name: "content/ProgressBarPage.qml"
        }
    }
}
