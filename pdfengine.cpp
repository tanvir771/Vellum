#include "pdfengine.h"
#include <QDebug>
#include <chrono>

PdfEngine::PdfEngine(QObject* parent) : QObject(parent) {}

bool PdfEngine::load(const QString& path) {
    qDebug() << "Loading PDF from:" << path;

    // beat 0.0295742s
    auto t1 = std::chrono::steady_clock::now();
    auto d = std::unique_ptr<poppler::document>(
        poppler::document::load_from_file(path.toStdString())
        );
    auto t2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> time = t2 - t1;
    qDebug() << "Loading completed in: " << time;

    if (!d) {
        qWarning() << "Failed to create document from" << path;
        return false;
    }
    if (d->pages() <= 0) {
        qWarning() << "Document has no pages:" << path;
        return false;
    }

    doc_ = std::move(d);
    qDebug() << "Successfully loaded PDF with" << doc_->pages() << "pages";
    emit pageCountChanged();
    return true;
}
