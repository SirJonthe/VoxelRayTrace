#include <iostream>
//#include <omp.h>

#include "PlatformSDL.h"
#include "Camera.h"
#include "Renderer.h"
#include "RobotModel.h"
#include "Math3d.h"

// Rudimentary OpenMP support is commented out
// see main(...) and Renderer::Render(...)
// see included header files in main.cpp and Renderer.cpp

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		std::cout << "Could not init SDL" << std::endl; 
		return 1;
	}

	//omp_set_num_threads(omp_get_num_procs);

	int w = 800;
	int h = 600;
	bool fs = false;
	if (argc > 1 && (argc-1)%2 == 0) {
		for (int i = 1; i < argc; i+=2) {
			if (strcmp(argv[i], "-w") == 0) {
				w = atoi(argv[i+1]);
				std::cout << "width set to " << w << " from argument " << argv[i+1] << std::endl;
			} else if (strcmp(argv[i], "-h") == 0) {
				h = atoi(argv[i+1]);
				std::cout << "height set to " << h << " from argument " << argv[i+1] << std::endl;
			} else if (strcmp(argv[i], "-fs") == 0) {
				fs = bool( atoi(argv[i+1]) );
				std::cout << "fullscreen set to " << fs << " from argument " << argv[i+1] << std::endl;
			} else {
				std::cout << "Unknown argument: " << argv[i] << std::endl;
			}
		}
	}

	Renderer renderer;
	if (!renderer.Init(w, h, fs)) {
		std::cout << "Could not init video mode" << std::endl;
		SDL_Quit();
		return 1;
	}
	SDL_WM_SetCaption("Voxel Ray Tracing", NULL);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_ShowCursor(SDL_FALSE);

	std::cout << "Total voxel volume: " << sizeof(Robot) << ", in bytes " << sizeof(Robot)*sizeof(Voxel) << std::endl;

	SDL_Event event;
	Camera camera(w, h);
	camera.SetPosition(vec3_t(8.f, 8.f, 8.f));
	bool quit = false;
	float left = 0.f;
	float right = 0.f;
	float forward = 0.f;
	float backward = 0.f;
	float up = 0.f;
	float down = 0.f;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_w:
					forward = 1.f;
					break;
				case SDLK_a:
					left = 1.f;
					break;
				case SDLK_s:
					backward = -1.f;
					break;
				case SDLK_d:
					right = -1.f;
					break;
				case SDLK_q:
					down = 1.f;
					break;
				case SDLK_e:
					up = -1.f;
					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				default: break;
				}
				break;
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_w:
					forward = 0.f;
					break;
				case SDLK_a:
					left = 0.f;
					break;
				case SDLK_s:
					backward = 0.f;
					break;
				case SDLK_d:
					right = 0.f;
					break;
				case SDLK_q:
					down = 0.f;
					break;
				case SDLK_e:
					up = 0.f;
					break;
				default: break;
				}
				break;
			case SDL_MOUSEMOTION:
				camera.Turn(-event.motion.xrel * 0.01f, 0.f);
				break;
			default: break;
			}
		}

		const vec3_t prevCam = camera.GetPosition();
		camera.Move(forward + backward, left + right, down + up);
		for (int i = 0; i < 3; ++i) {
			if (camera.GetPosition()[i] < 0 || camera.GetPosition()[i] >= 16) {
				camera.SetPosition(prevCam);
				break;
			}
		}

		renderer.Render(camera, Robot, RobotDim);
		renderer.Refresh();
	}

	renderer.CleanUp();
	SDL_Quit();
	return 0;
}
