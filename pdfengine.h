#ifndef PDFENGINE_H
#define PDFENGINE_H

#include <QObject>
#include <QString>
#include <memory>
#include <poppler/cpp/poppler-document.h>

class PdfEngine : public QObject {
    Q_OBJECT
    Q_PROPERTY(int pageCount READ pageCount NOTIFY pageCountChanged)

public:
    explicit PdfEngine(QObject* parent = nullptr);

    Q_INVOKABLE bool load(const QUrl& url);

    poppler::document* doc() const { return doc_.get(); }
    int pageCount() const { return doc_ ? doc_->pages() : 0; }

signals:
    void pageCountChanged();

private:
    std::unique_ptr<poppler::document> doc_;
};

#endif // PDFENGINE_H
