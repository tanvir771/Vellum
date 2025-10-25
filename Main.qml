import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: "PDF Viewer - Multiple Pages"


    Column {
        anchors.fill: parent
        spacing: 10

        Row {
            width: parent.width
            height: 50
            spacing: 10
            padding: 10

            Button {
                text: "Previous"
                enabled: currentPage > 0
                onClicked: currentPage--
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "Page " + (currentPage + 1) + " of " + totalPages
                font.pixelSize: 16
            }

            Button {
                text: "Next"
                enabled: currentPage < totalPages - 1
                onClicked: currentPage++
            }

            FileDialog {
                id: fileDialog
                title: "Open PDF"
                nameFilters: ["PDF files (*.pdf)"]
                onAccepted: {
                    PDF.load(Qt.resolvedUrl(fileDialog.selectedFile))
                    currentPage = -1 // force a redraw
                    currentPage = 0
                }
            }

            Button {
                text: "Open PDF"
                onClicked: fileDialog.open()
            }
        }

        Rectangle {
            width: parent.width
            height: parent.height - 60
            color: "lightgray"

            Image {
                id: pdfImage
                anchors.centerIn: parent
                width: parent.width - 40
                height: parent.height - 40
                source: "image://pdf/page" + currentPage
                fillMode: Image.PreserveAspectFit
                cache: false
                smooth: true

                onStatusChanged: {
                    console.log("Image status:", status, "for page", currentPage)
                }
            }

            BusyIndicator {
                anchors.centerIn: parent
                running: pdfImage.status === Image.Loading
            }
        }
    }

    // Side panel for thumbnails
    Rectangle {
        width: 120
        color: "#eeeeee"
        ListView {
            id: thumbnailList
            anchors.fill: parent
            model: totalPages
            delegate: Item {
                width: parent.width
                height: 80
                Image {
                    source: "image://pdf/page" + index
                    width: parent.width - 10
                    height: parent.height - 10
                    fillMode: Image.PreserveAspectFit
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: currentPage = index
                }
            }
        }
    }


    property int currentPage: 0
    property int totalPages: PDF && PDF.pageCount > 0 ? PDF.pageCount : 1
}
