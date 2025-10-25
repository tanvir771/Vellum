#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include "pdfengine.h"
#include "pdfprovider.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qInfo() << "main: creating PdfEngine backend";
    PdfEngine backend;
    qInfo() << "main: creating PdfProvider";
    PdfProvider* provider = new PdfProvider(&backend);

    qInfo() << "main: creating QQmlApplicationEngine";
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    qInfo() << "main: adding image provider";
    engine.addImageProvider("pdf", provider);

    //qInfo() << "main: calling backend.load(...)";
    //bool ok = backend.load("F:/Qt_Projects/PDFBasic/PDFBasic/build/Desktop_Qt_6_9_0_MinGW_64_bit-Debug/sample.pdf");
    //qInfo() << "main: backend.load returned" << ok;

    qInfo() << "main: setting context property PDF";
    engine.rootContext()->setContextProperty("PDF", &backend);

    qInfo() << "main: about to load QML module";
    engine.loadFromModule("PDFBasic", "Main");
    qInfo() << "main: loadFromModule returned";

    return app.exec();
}
