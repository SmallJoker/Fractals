#include "fractal.h"
#include <GL/freeglut.h>
#include <iostream>
#include "imgui.h"


Fractal::Fractal(int width, int height) :
	Module(MODULE_FRACTAL),
	offset_x(0.1f),
	offset_y(0.1f),
	m_width(width),
	m_height(height)
{
	m_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
	m_handle = cairo_create(m_surface);
	cairo_set_source_rgb(m_handle, 1, 1, 1);
	cairo_set_line_width(m_handle, 0.1);

	LOG("Cairo loaded");
}

Fractal::~Fractal()
{
	// Destroy cairo
	cairo_destroy(m_handle);
	cairo_surface_destroy(m_surface);
}

void Fractal::glConstruct()
{
}

void Fractal::glDestruct()
{
	
}

static int MAX_LENGTH = 50;
uint32_t Fractal::getIterations(Complex c)
{
	// c   = c² + z
	// c   = (c.r + c.i)(c.r + c.i) + (z.r + z.i)
	// c   = c.r² + 2 * c.r * c.i - c.i² + (z.r + z.i)
	// c.r = c.r² - c.i² + z.r
	// c.i = 2 * c.r * c.i + z.i

	Complex tmp;
	int n = 0;
	while (true) {
		float r2 = c.r * c.r;
		float i2 = c.i * c.i;
		tmp.r = r2 - i2 + formula.r;
		tmp.i = 2 * c.r * c.i + formula.i;
		c = tmp;
		n++;
		if (n > MAX_LENGTH * MAX_LENGTH)
			break;
		if (r2 + i2 > MAX_LENGTH)
			break;
	}
	return n;
}

static void log_opengl()
{
    auto error = glGetError();
	do {
		switch (error) {
			case GL_NO_ERROR: break;
			case GL_INVALID_OPERATION: std::cout << "GL_INVALID_OPERATION" << std::endl;
				break;
			case GL_INVALID_ENUM:      std::cout << "GL_INVALID_ENUM" << std::endl;
				break;
			case GL_STACK_OVERFLOW:    std::cout << "GL_STACK_OVERFLOW" << std::endl;
				break;
			case GL_STACK_UNDERFLOW:   std::cout << "GL_STACK_UNDERFLOW" << std::endl;
				break;
			default:
				std::cout << std::to_string(error) << std::endl;
		}
	} while ((error = glGetError()) != GL_NO_ERROR);
}

void Fractal::drawFrame()
{
	/*int stride = cairo_image_surface_get_stride(m_surface);
	unsigned char *current_row = cairo_image_surface_get_data(m_surface);
	for (int y = 0; y < m_height; ++y) {
		uint32_t *row = (uint32_t *)current_row;
		for (int x = 0; x < m_width; ++x) {
			row[x] = 0xFF0000FFul;
		}
		current_row += stride;
	}
	cairo_surface_mark_dirty(m_surface);
*/
	uint8_t *cache_pos = nullptr;
	if (!m_cache) {
		uint16_t map[m_width * m_height];
		uint16_t *map_pos = map;

		__uint128_t avg = 0;
		uint32_t avg_cnt = 0;
		for (int y = 0; y < m_height; ++y) {
			for (int x = 0; x < m_width; ++x) {
				Complex c = {
					.r = (x - m_width / 2)  / zoom + offset_x,
					.i = (y - m_height / 2) / zoom + offset_y,
				};
				auto cnt = getIterations(c);
				*map_pos++ = cnt;
				if (cnt > 1) {
					avg += cnt;
					avg_cnt++;
				}
			}
		}
		avg /= avg_cnt;
		m_cache = new uint8_t[m_width * m_height];
		cache_pos = m_cache;
		map_pos = map;

		for (int y = 0; y < m_height; ++y) {
			for (int x = 0; x < m_width; ++x) {
				int16_t what = *map_pos++ - avg;
				*cache_pos++ = what < 0 ? 0 : (what > 0xFF ? 0xFF : what);
			}
		}
	}
// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
// https://learnopengl.com/Advanced-OpenGL/Framebuffers
// http://www.songho.ca/opengl/gl_fbo.html
	glPointSize(3);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ImGuiIO &io = ImGui::GetIO();
	glOrtho(0, io.DisplaySize.x, io.DisplaySize.y, 0, 1, -1);

//glMatrixMode(GL_MODELVIEW);
//glEnable(GL_TEXTURE_2D);

	glBegin(GL_POINTS);
	cache_pos = m_cache;
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			glColor3f(0.0f, 0.0f, *cache_pos++ / 256.0f);
			glVertex2i(x * 2, y * 2);
		}
	}
	glEnd();

	/*unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width / 4, m_height / 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_cache);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
*/
	log_opengl();
}

