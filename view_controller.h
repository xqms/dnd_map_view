// Shared control
// Author: Max Schwarz <max.schwarz@online.de>

#ifndef VIEW_CONTROLLER_H
#define VIEW_CONTROLLER_H

#include <QObject>
#include <QImage>
#include <QTimer>

#include <memory>

#include "cell_model.h"

class Renderer;

class ViewController : public QObject
{
Q_OBJECT
Q_PROPERTY(QImage image WRITE setImage READ image NOTIFY imageChanged)
Q_PROPERTY(QString videoURL READ videoURL CONSTANT)

Q_PROPERTY(int presenterWidth READ presenterWidth WRITE setPresenterWidth NOTIFY presenterGeometryChanged)
Q_PROPERTY(int presenterHeight READ presenterHeight WRITE setPresenterHeight NOTIFY presenterGeometryChanged)
Q_PROPERTY(double presenterAspectRatio READ presenterAspectRatio NOTIFY presenterGeometryChanged)

Q_PROPERTY(QPointF rectPos READ rectPos WRITE setRectPos NOTIFY rectChanged)
Q_PROPERTY(double rectWidth READ rectWidth WRITE setRectWidth NOTIFY rectWidthChanged)

Q_PROPERTY(int consoleResolution READ consoleResolution WRITE setConsoleResolution NOTIFY resolutionsChanged)
Q_PROPERTY(int presenterResolution READ presenterResolution WRITE setPresenterResolution NOTIFY resolutionsChanged)

Q_PROPERTY(QRectF cellsRect READ cellsRect WRITE setCellsRect NOTIFY cellsRectChanged)
Q_PROPERTY(int cellsX READ cellsX WRITE setCellsX NOTIFY cellsChanged)
Q_PROPERTY(int cellsY READ cellsY WRITE setCellsY NOTIFY cellsChanged)

Q_PROPERTY(QObject* cellModel READ cellModel CONSTANT)

public:
	explicit ViewController(const QString& filename, QObject* parent = 0);
	~ViewController();

	const QImage& image() const
	{ return m_image; }

	void setImage(const QImage& img);

	const QString& videoURL() const
	{ return m_video; }

	constexpr int presenterWidth() const
	{ return m_presenterWidth; }

	constexpr int presenterHeight() const
	{ return m_presenterHeight; }

	constexpr double presenterAspectRatio() const
	{ return static_cast<double>(m_presenterWidth) / m_presenterHeight; }

	QPointF rectPos() const
	{ return m_rectPos; }

	double rectWidth() const
	{ return m_rectWidth; }

	constexpr int consoleResolution() const
	{ return m_consoleResolution; }

	constexpr int presenterResolution() const
	{ return m_presenterResolution; }

	QRectF cellsRect() const
	{ return m_cellsRect; }

	constexpr int cellsX() const
	{ return m_cellsX; }

	constexpr int cellsY() const
	{ return m_cellsY; }

	QObject* cellModel()
	{
		return &m_cellModel;
	}

public Q_SLOTS:
	void setPresenterWidth(int width);
	void setPresenterHeight(int height);

	void setRectPos(const QPointF& pos);
	void setRectWidth(double width);

	void setPresenterResolution(int res);
	void setConsoleResolution(int res);

	void setCellsRect(const QRectF& rect);
	void setCellsX(int cells);
	void setCellsY(int cells);

Q_SIGNALS:
	void imageChanged();
	void presenterGeometryChanged();

	void rectChanged();
	void rectWidthChanged();

	void resolutionsChanged();

	void cellsChanged();
	void cellsRectChanged();

private Q_SLOTS:
	void render();

private:
	std::unique_ptr<Renderer> m_renderer;
	QImage m_image;

	int m_presenterWidth = 800;
	int m_presenterHeight = 600;

	QPointF m_rectPos{0.1, 0.1};
	double m_rectWidth = 0.5;

	QTimer m_renderTimer;
	int m_presenterResolution = 800;
	int m_consoleResolution = 800;

	QRectF m_cellsRect{0.2, 0.2, 0.7, 0.7};
	int m_cellsX = 10;
	int m_cellsY = 10;

	QString m_settingsPath;

	CellModel m_cellModel;

	QString m_video;
};

#endif

