// Image renderer
// Author: Max Schwarz <max.schwarz@online.de>

#ifndef IMAGE_RENDERER_H
#define IMAGE_RENDERER_H

#include "../renderer.h"

class ImageRenderer : public Renderer
{
public:
	explicit ImageRenderer(const QString& filename);
	~ImageRenderer();

	void render(int size) override;
private:
	QImage m_image;
	bool m_signaled = false;
};

#endif
