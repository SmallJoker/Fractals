#pragma once

#include "module.h"


class Gui : public Module {
public:
	Gui();

	void drawFrame();
	void glConstruct();
	void glDestruct();
};

