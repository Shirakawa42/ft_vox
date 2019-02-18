#include "ChunkHandler.hpp"
#include "Window.hpp"

Player					g_player;
float					g_deltaTime;

int		main(void)
{
	Window			window;
	MapGeneration	map;

	try
	{
		window.init();
		window.loop();
	}
	catch (VoxException & e)
	{
		std::cout << e.what() << std::endl;
		exit(0);
	}
	catch (std::bad_alloc & e)
	{
		std::cout << e.what() << std::endl;
		exit(0);
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
		exit(0);
	}
	return 0;
}