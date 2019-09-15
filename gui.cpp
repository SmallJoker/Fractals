
#include "gui.h"
#include "imgui.h"
#include "examples/imgui_impl_opengl2.h"
#include "examples/imgui_impl_freeglut.h"
#include "fractal.h"

Gui::Gui() :
	Module(MODULE_GUI)
{
}

void Gui::drawFrame()
{
	Fractal *f = (Fractal *)getModuleByType(MODULE_FRACTAL);

	ImGuiIO &io = ImGui::GetIO();
	bool changed = false;
	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Refresh rate: %.0f", io.Framerate);
	changed |= ImGui::InputFloat("Real", &f->formula.r, 0.01f, 0, "%g");
	changed |= ImGui::InputFloat("Imaginary", &f->formula.i, 0.01f, 0, "%g");
	changed |= ImGui::InputFloat("Zoom", &f->zoom, f->zoom * 0.05f, 0, "%g");
	changed |= ImGui::InputFloat("X Offset", &f->offset_x, 5.0f / f->zoom, 0, "%g");
	changed |= ImGui::InputFloat("Y Offset", &f->offset_y, 5.0f / f->zoom, 0, "%g");
	ImGui::End();

	if (changed)
		f->setDirty();
}

void Gui::glConstruct()
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplFreeGLUT_Init();
	ImGui_ImplFreeGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();

	LOG("Dear ImGui initialized");
}

void Gui::glDestruct()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplFreeGLUT_Shutdown();
	ImGui::DestroyContext();
}
