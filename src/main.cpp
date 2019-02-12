#include "ChunkHandler.hpp"
#include "Window.hpp"

int		main(void)
{
	Window			window;

	try
	{
		window.init();
		window.loop();
	}
	catch (VoxException e)
	{
		std::cout << e.what() << std::endl;
		exit(0);
	}
	return 0;
}