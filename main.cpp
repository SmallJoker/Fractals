#include "imgui.h"
#include "examples/imgui_impl_opengl2.h"
#include "examples/imgui_impl_freeglut.h"
#include "fractal.h"
#include "gui.h"
#include "module.h"
#include <chrono>
#include <thread>
#include <GL/freeglut.h>

static std::vector<Module *> main_modules;
std::vector<Module *> *g_modules = &main_modules;

void drawFrame()
{
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// Start the ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplFreeGLUT_NewFrame();

	// Rendering
	ImGuiIO &io = ImGui::GetIO();
	float sleep_time = 1000.f / 60.f - io.DeltaTime;
	if (sleep_time > 0)
		std::this_thread::sleep_for(std::chrono::milliseconds((int)sleep_time));

	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSolidTeapot(0.3f); // Must have

	for (auto it = g_modules->rbegin(); it != g_modules->rend(); ++it)
		(*it)->drawFrame();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	glutSwapBuffers();
	glutPostRedisplay();
}

void initRender()
{
	// Create GLUT window
	int carg = 0;
	glutInit(&carg, nullptr);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Fractals");
	glutDisplayFunc(drawFrame);
	LOG("Render initialized");
}

int main()
{
	initRender();

	// Init all modules by priority
	g_modules->push_back(new Gui());
	g_modules->push_back(new Fractal(400, 300));

	for (auto it = g_modules->begin(); it != g_modules->end(); ++it)
		(*it)->glConstruct();

	glutMainLoop();

	for (auto it = g_modules->rbegin(); it != g_modules->rend(); ++it) {
		(*it)->glDestruct();
		delete *it;
	}
	g_modules->clear();
	return 0;
}
