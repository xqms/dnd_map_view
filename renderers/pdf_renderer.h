// PDF renderer
// Author: Max Schwarz <max.schwarz@online.de>

#ifndef PDF_RENDERER_H
#define PDF_RENDERER_H

#include "../renderer.h"

#include <memory>

class PDFRenderer : public Renderer
{
public:
	explicit PDFRenderer(const QString& filename);
	~PDFRenderer();

	void render(int size) override;
private:
	class Private;

	void doRender(int size);

	std::unique_ptr<Private> m_d;
};

#endif
