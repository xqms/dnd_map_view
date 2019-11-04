// Image renderer
// Author: Max Schwarz <max.schwarz@online.de>

#include "image_renderer.h"

ImageRenderer::ImageRenderer(const QString& filename)
 : m_image{filename}
{
	if(m_image.isNull())
		throw std::runtime_error("Could not open input image");
}

ImageRenderer::~ImageRenderer()
{
}

void ImageRenderer::render(int size)
{
	if(!m_signaled)
	{
		renderFinished(m_image);
		m_signaled = true;
	}
}
