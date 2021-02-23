#pragma once

#include "MarsHeader.h"
#include "Core/Application.h"

class Martian
{

};

static ME::Application* ME::CreateApp()
{
	new ME::Application;
}
