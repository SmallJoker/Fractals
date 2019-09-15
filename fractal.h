#pragma once
#include "module.h"
#include <cairo/cairo.h>
#include <cstdint>


struct Complex {
	float r, i;
};

class Fractal : public Module {
public:
	Fractal(int width, int height);
	~Fractal();

	void drawFrame();
	void glConstruct();
	void glDestruct();

	float offset_x;
	float offset_y;
	float zoom = 200.0f;
	void setDirty()
	{
		if (m_cache)
			delete m_cache;
		m_cache = nullptr;
	};

	Complex formula = { .r = 0.4f, .i = -0.2f };

private:
	uint32_t getIterations(Complex c);

	int m_width;
	int m_height;
	cairo_t *m_handle;
	cairo_surface_t *m_surface;

	uint8_t *m_cache = nullptr;
};
