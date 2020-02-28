#include "Macros.hpp"
#include "Exception.hpp"
#include "Constants.hpp"
#include "Configuration.hpp"
#include "Gui/Application.hpp"

using namespace Dixter;
using TConfMan = TConfigurationManager;
using EConf = EConfiguration;

int main(int argc, char **argv)
{
	printl_log("GUI Module")
	auto app = new Gui::TApplication(argc, argv, "Dixter");
	int width(1200), height(600);
	try
	{
		auto __confManIni = getIniManager({g_guiConfigPath});
		width = __confManIni->accessor()->getValue("window_width");
		height = __confManIni->accessor()->getValue("window_height");
	} catch (TException& e)
	{
		printerr(e.getMessage())
	}

	auto window = new Gui::TWindow("Dixter", width, height);
	
	return app->exec(window);
}