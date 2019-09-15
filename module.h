#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

#define LOG(str) \
	std::cout << "\e[0;36m" << __PRETTY_FUNCTION__ << "\e[0m: " << str << std::endl

#define ASSERT(expr) \
	if (!(expr)) { \
		std::cout << "\e[0;31m" << #expr << ": Assertion failed\e[0m" << std::endl; \
		throw "Assertion failed"; \
	}

class Module;
extern std::vector<Module *> *g_modules;

enum module_t {
	MODULE_INVALID = 0,
	MODULE_GUI,
	MODULE_FRACTAL
};

class Module {
public:
	Module(module_t type) :
		m_type(type) {}
	virtual ~Module() {};

	inline module_t getType() const
	{ return m_type; }

	virtual void drawFrame() = 0;
	virtual void glConstruct() = 0;
	virtual void glDestruct() = 0;

private:
	module_t m_type;
};

inline Module *getModuleByType(module_t type)
{
	for (auto &module : *g_modules)
		if (module->getType() == type)
			return module;

	return nullptr;
}
