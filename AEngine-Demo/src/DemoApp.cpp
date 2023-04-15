/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Provides an entry point for entire project
**/
#include <AEngine.h>

AEngine::Application* AEngine::CreateApplication()
{
	AEngine::ApplicationProperties props;
	props.title = "AEngine Demo";

	return new Application(props);
}
