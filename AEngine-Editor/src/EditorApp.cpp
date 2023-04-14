/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Provides an entry point for entire project
**/
#include <AEngine/AEngine.h>

AEngine::Application* AEngine::createApplication()
{
	AEngine::ApplicationProperties props;
	props.title = "AEngine Editor";

	return new Application(props);
}
