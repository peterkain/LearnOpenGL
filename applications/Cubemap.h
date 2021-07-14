#pragma once

#include "Application.h"
#include "SimpleObjLoader.h"

class Cubemap : public Application {
	void init() override;
	void loop() override;
public:
	Cubemap(int width, int height, const std::string& title, bool fullscreen = false) :
		Application(width, height, title, fullscreen) {
	}
};