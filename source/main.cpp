#include <iostream>
#include <stb_image.h>
#include "Application/Application.h"

int main()
{
	auto app = std::make_unique<Papyrus::Application>(); 
	app->run(); 
	return 0;
}