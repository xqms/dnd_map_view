// PDF renderer
// Author: Max Schwarz <max.schwarz@online.de>

#include "pdf_renderer.h"

#include <poppler-qt5.h>

#include <QDebug>
#include <QThreadPool>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>

class PDFRenderer::Private
{
public:
	std::shared_ptr<Poppler::Document> document;
	Poppler::Page* page{};

	QThreadPool pool;
	QFuture<void> future;
};

PDFRenderer::PDFRenderer(const QString& filename)
 : m_d{new Private}
{
	m_d->document.reset(
		Poppler::Document::load(filename)
	);

	if(!m_d->document || m_d->document->isLocked())
	{
		throw std::runtime_error("Could not open PDF");
	}

	m_d->document->setRenderHint(Poppler::Document::Antialiasing, true);
	m_d->document->setRenderHint(Poppler::Document::TextAntialiasing, true);

	m_d->page = m_d->document->page(0);
	if(!m_d->page)
		throw std::runtime_error("PDF is empty");
}

PDFRenderer::~PDFRenderer()
{
}

void PDFRenderer::render(int size)
{
	m_d->future = QtConcurrent::run(&m_d->pool, this, &PDFRenderer::doRender, size);
}

void PDFRenderer::doRender(int size)
{
	// Don't do something stupid
	if(size > 10000)
	{
		qDebug() << "Limiting excessize PDF rendering size";
		size = 10000;
	}

	QSizeF pageSize = m_d->page->pageSizeF() / 72.0;
	float dpi = size / std::max(pageSize.width(), pageSize.height());

	QImage image = m_d->page->renderToImage(dpi, dpi);
	if(image.isNull())
		throw std::runtime_error("Could not render image");

	renderFinished(image);
}


