// Abstract renderer interface
// Author: Max Schwarz <max.schwarz@ais.uni-bonn.de>

#ifndef RENDERER_H
#define RENDERER_H

#include <QImage>
#include <QSize>

class Renderer : public QObject
{
Q_OBJECT
public:
	virtual ~Renderer() = default;

	virtual void render(int size) = 0;
Q_SIGNALS:
	void renderFinished(const QImage& image);
};

#endif
