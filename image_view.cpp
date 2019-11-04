// QML QImage display
// Author: Max Schwarz <max.schwarz@online.de>

#include "image_view.h"

#include <QPainter>
#include <QDebug>

ImageView::ImageView(QQuickItem* parent)
 : QQuickPaintedItem(parent)
{
	setAntialiasing(true);
}

ImageView::~ImageView()
{
}

void ImageView::setImage(const QImage& image)
{
	m_image = image;
	imageChanged();
	recalculateImageRect(size());
	update();
}

void ImageView::paint(QPainter* painter)
{
	if(!m_image.isNull())
		painter->drawImage(m_imageRect, m_image);
}

float ImageView::aspectRatio() const
{
	if(m_image.isNull())
		return 16.0 / 9.0;

	return static_cast<float>(m_image.width()) / m_image.height();
}

void ImageView::recalculateImageRect(const QSizeF& outerSize)
{
	float oW = outerSize.width();
	float oH = outerSize.height();

	if(m_image.isNull())
	{
		m_imageRect = QRectF(0, 0, oW, oH);
		imageRectChanged();
		return;
	}

	float scalingFactor = std::min(
		oW / m_image.width(),
		oH / m_image.height()
	);

	float imgW = scalingFactor * m_image.width();
	float imgH = scalingFactor * m_image.height();

	float offX = (oW - imgW) / 2;
	float offY = (oH - imgH) / 2;

	m_imageRect = QRectF(offX, offY, imgW, imgH);
	imageRectChanged();
}


void ImageView::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
	recalculateImageRect(newGeometry.size());
}

