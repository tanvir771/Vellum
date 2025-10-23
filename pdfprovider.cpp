#include "pdfprovider.h"
#include "pdfengine.h"
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-page-renderer.h>
#include <cstring>

PdfProvider::PdfProvider(PdfEngine* engine)
    : QQuickImageProvider(QQuickImageProvider::Image), engine_(engine) {
    qDebug() << "PdfProvider constructed";
}

QImage PdfProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize) {
    qDebug() << "requestImage called for:" << id;
    qDebug() << "requestedSize:" << requestedSize;

    if (!engine_ || !engine_->doc()) {
        qWarning() << "No engine or document";
        return QImage();
    }

    // Parse page number from id (e.g., "page0", "page1", etc.)
    int pageNum = 0;
    if (id.startsWith("page")) {
        pageNum = id.mid(4).toInt();
    }

    if (pageNum < 0 || pageNum >= engine_->doc()->pages()) {
        qWarning() << "Invalid page number:" << pageNum;
        return QImage();
    }

    qDebug() << "Rendering page:" << pageNum;
    std::unique_ptr<poppler::page> page(engine_->doc()->create_page(pageNum));
    if (!page) {
        qWarning() << "Failed to create page";
        return QImage();
    }

    poppler::page_renderer renderer;
    renderer.set_image_format(poppler::image::format_argb32);
    renderer.set_render_hint(poppler::page_renderer::antialiasing, true);
    renderer.set_render_hint(poppler::page_renderer::text_antialiasing, true);

    // Render
    poppler::image popplerImg = renderer.render_page(page.get(), 300, 300);

    if (!popplerImg.is_valid()) {
        qWarning() << "Poppler image invalid";
        return QImage();
    }

    qDebug() << "Poppler image size:" << popplerImg.width() << "x" << popplerImg.height();

    // Create QImage and copy data
    QImage result(popplerImg.width(), popplerImg.height(), QImage::Format_ARGB32);
    std::memcpy(result.bits(), popplerImg.const_data(),
                popplerImg.height() * popplerImg.bytes_per_row());

    if (size) {
        *size = result.size();
    }

    qDebug() << "Returning QImage:" << result.size() << "isNull:" << result.isNull();

    return result;
}
