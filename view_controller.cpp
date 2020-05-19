// Shared control
// Author: Max Schwarz <max.schwarz@online.de>

#include "view_controller.h"

#include "renderers/pdf_renderer.h"
#include "renderers/image_renderer.h"

#include <QDebug>
#include <QMimeDatabase>
#include <QSettings>

ViewController::ViewController(const QString& filename, QObject* parent)
 : QObject{parent}
{
	QMimeDatabase db;
	auto type = db.mimeTypeForFile(filename);
	if(!type.isValid())
	{
		fprintf(stderr, "Could not determine MIME type of file '%s'\n",
			qPrintable(filename)
		);

		throw std::runtime_error("Could not determine MIME type");
	}

	if(type.name().startsWith("video/"))
		m_video = "file://" + filename;
	else if(type.inherits("application/pdf"))
		m_renderer.reset(new PDFRenderer(filename));
	else if(type.name().startsWith("image/"))
		m_renderer.reset(new ImageRenderer(filename));
	else
	{
		fprintf(stderr, "Unknown MIME type: %s\n", qPrintable(type.name()));
		throw std::runtime_error("Could not determine MIME type");
	}

	m_settingsPath = filename + ".dnd";

	QSettings settings(m_settingsPath, QSettings::IniFormat);
	m_rectPos = settings.value("rectPos", m_rectPos).toPointF();
	m_rectWidth = settings.value("rectWidth", m_rectWidth).toDouble();
	m_cellsRect = settings.value("cellsRect", m_cellsRect).toRectF();
	m_cellsX = settings.value("cellsX", m_cellsX).toUInt();
	m_cellsY = settings.value("cellsY", m_cellsY).toUInt();
	m_displayGrid = settings.value("displayGrid", m_displayGrid).toBool();

	connect(this, &ViewController::cellsChanged, [&](){
		m_cellModel.setCells(m_cellsX, m_cellsY);
	});

	m_cellModel.setCells(m_cellsX, m_cellsY);
	m_cellModel.restore(settings);

	if(m_renderer)
	{
		connect(m_renderer.get(), &Renderer::renderFinished, this, &ViewController::setImage);

		m_renderTimer.setSingleShot(true);
		m_renderTimer.setInterval(500);
		connect(&m_renderTimer, &QTimer::timeout, this, &ViewController::render);
		render();
	}
}

ViewController::~ViewController()
{
	QSettings settings(m_settingsPath, QSettings::IniFormat);
	settings.setValue("rectPos", m_rectPos);
	settings.setValue("rectWidth", m_rectWidth);
	settings.setValue("cellsRect", m_cellsRect);
	settings.setValue("cellsX", m_cellsX);
	settings.setValue("cellsY", m_cellsY);
	settings.setValue("displayGrid", m_displayGrid);
	m_cellModel.save(settings);
}

void ViewController::setRectPos(const QPointF& pos)
{
	m_rectPos = pos;
	rectChanged();
}
void ViewController::setRectWidth(double width)
{
	m_rectWidth = width;
	rectWidthChanged();
}

void ViewController::setCellsPerPresenterRow(double cells)
{
	m_cellsPerPresenterRow = cells;
	cellsPerPresenterRowChanged();
}


void ViewController::setPresenterWidth(int width)
{
	m_presenterWidth = width;
	presenterGeometryChanged();
}
void ViewController::setPresenterHeight(int height)
{
	m_presenterHeight = height;
	presenterGeometryChanged();
}

void ViewController::setConsoleResolution(int res)
{
	m_consoleResolution = res;
	resolutionsChanged();

	if(m_renderer)
		m_renderTimer.start();
}

void ViewController::setPresenterResolution(int res)
{
	m_presenterResolution = res;
	resolutionsChanged();

	if(m_renderer)
		m_renderTimer.start();
}

void ViewController::render()
{
	if(m_renderer)
		m_renderer->render(std::max(m_presenterResolution, m_consoleResolution));
}

void ViewController::setImage(const QImage& img)
{
	m_image = img;
	imageChanged();
}

void ViewController::setCellsRect(const QRectF& rect)
{
	m_cellsRect = rect;
	cellsRectChanged();
}

void ViewController::setDisplayGrid(bool display)
{
	m_displayGrid = display;
	displayGridChanged();
}


void ViewController::setCellsX(int cells)
{
	m_cellsX = cells;
	cellsChanged();
}

void ViewController::setCellsY(int cells)
{
	m_cellsY = cells;
	cellsChanged();
}
