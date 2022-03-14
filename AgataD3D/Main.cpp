#include "pch.h"
#include "Scene3D.h"
#include <stb/stb_image.cpp>

int main(int argc, char** argv) {

	Agata::WindowParams windowParams;
	windowParams.Width = 1280u;
	windowParams.Height = 720u;
	windowParams.Title = "Proyecto de Graficas 2";

	Agata::Scene* scene = new Scene3D(windowParams);
	scene->OnRun();
	delete scene;

}