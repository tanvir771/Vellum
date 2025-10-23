#ifndef PDFPROVIDER_H
#define PDFPROVIDER_H

#include <QQuickImageProvider>

class PdfEngine;

class PdfProvider : public QQuickImageProvider {
public:
    explicit PdfProvider(PdfEngine* engine);
    QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override;

private:
    PdfEngine* engine_;
};

#endif // PDFPROVIDER_H
