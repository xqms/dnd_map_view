// QML QImage display
// Author: Max Schwarz <max.schwarz@online.de>

#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H

#include <QQuickPaintedItem>
#include <QImage>

class ImageView : public QQuickPaintedItem
{
Q_OBJECT
Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
Q_PROPERTY(float aspectRatio READ aspectRatio NOTIFY imageChanged)
Q_PROPERTY(QImage nullImage CONSTANT READ nullImage)
Q_PROPERTY(QRectF imageRect READ imageRect NOTIFY imageRectChanged)
Q_PROPERTY(int imageWidth READ imageWidth NOTIFY imageChanged)
Q_PROPERTY(int imageHeight READ imageHeight NOTIFY imageChanged)
public:
	explicit ImageView(QQuickItem* parent = 0);
	virtual ~ImageView();

	void paint(QPainter* painter) override;

	QImage image() const
	{ return m_image; }

	inline QImage nullImage() const
	{ return QImage(); }

	float aspectRatio() const;

	QRectF imageRect() const
	{ return m_imageRect; }

	int imageWidth() const
	{ return m_image.width(); }

	int imageHeight() const
	{ return m_image.height(); }
public Q_SLOTS:
	void setImage(const QImage& image);
Q_SIGNALS:
	void imageChanged();
	void imageRectChanged();
protected:
	void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;
	void recalculateImageRect(const QSizeF& outerSize);
private:
	QImage m_image;
	QRectF m_imageRect;
};

#endif
