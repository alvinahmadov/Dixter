#include "Gui/Application.hpp"
#include "Configuration.hpp"

int main(int argc, char** argv)
{
	print_log("Full App")
	
	auto app = new Dixter::Gui::Application("App");
	
	return app->exec(argc, argv);
}
