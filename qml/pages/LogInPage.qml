import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page

    property string state: "phoneNumber"

    Connections {
        target: Registrator;
        onCodeNeeded: {
            busy.hide()
            page.state = "code"
        }
        onLoggedIn: {
            busy.hide()
            page.state = "done"
        }
        onFailed: {
            busy.hide()
            page.state = "failed"
        }
    }


    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Telepathy-Morse log in")
            }

            // Busy indicator
            Row {
                width: parent.width
                height: busy.height
                spacing: Theme.paddingMedium
                BusyIndicator {
                    id: busy
                    size: BusyIndicatorSize.Medium
                    visible: running
                    running: false
                    function show(data) {
                        actionLabel.text = data
                        page.state = "busy"
                        running = true
                    }
                    function hide() {
                        running = false
                    }
                }
                Label {
                    id: actionLabel
                    wrapMode: Text.Wrap
                    anchors.verticalCenter: parent.verticalCenter
                    visible: busy.visible
                    text: qsTr("Logging in")
                }
            }

            // Phone number field
            TextField {
                id: phoneNumber
                visible: page.state == "phoneNumber"
                width: parent.width
                errorHighlight: text.length === 0
                text: qsTr("")
                validator: RegExpValidator{ regExp: /^\+[0-9]+$/;}
                inputMethodHints: Qt.ImhPreferNumbers

                label: qsTr("Phone number in international format", "Registration phone number text field label")
                EnterKey.enabled: text.length > 0
                EnterKey.onClicked: phoneNumber.send()

                placeholderText: qsTr("Phone number in international format")


                function send()
                {
                    page.state = "checkPhone"
                    busy.show("Checking phone number...")
                    page.forceActiveFocus()
                    Registrator.start( phoneNumber.text.trim())
                }
            }
            Button {
                visible: page.state == "phoneNumber"
                text: qsTr("Go", "Phone number button text")
                enabled: phoneNumber.text.length > 0
                onClicked: phoneNumber.send()
            }

            // Security code field
            TextField {
                id: code
                visible: page.state == "code"
                width: parent.width
                errorHighlight: text.length === 0
                text: qsTr("")
                validator: RegExpValidator{ regExp: /^[0-9]+$/;}
                inputMethodHints: Qt.ImhPreferNumbers

                label: qsTr("Security code", "Enter the security code")
                EnterKey.enabled: text.length > 0
                EnterKey.onClicked: phoneNumber.send()

                placeholderText: qsTr("Security code")
                function send()
                {
                    page.state = "checkCode"
                    busy.show("Logging in...")
                    page.forceActiveFocus()
                    Registrator.logIn( code.text.trim())
                }
            }
            Button {
                visible: page.state == "code"
                text: qsTr("Log in.", "Log in button text")
                enabled: code.text.length > 0
                onClicked: code.send()
            }

            Text {
                id: done
                visible: page.state == "done"
                text: qsTr("Log in done. You can close and remove this app now and start using Morse.")
                color: Theme.primaryColor
                width: parent.width
                wrapMode: Text.Wrap
            }

            Text {
                id: failed
                visible: page.state == "failed"
                text: qsTr("Log in failed. Restart the app to try again.")
                color: Theme.primaryColor
                width: parent.width
                wrapMode: Text.Wrap
            }

        }
    }
}


