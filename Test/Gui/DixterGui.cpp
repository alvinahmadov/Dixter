#include "Gui/Application.hpp"

int main(int argc, char **argv)
{
	printl_log("GUI Module")
	auto app = new Dixter::Gui::Application(argc, argv, "Dixter");

	auto window = new Dixter::Gui::DixterWindow("Window", 1280, 768);
	return app->exec(window);
}