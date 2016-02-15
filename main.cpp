#include "rsw_math.h"
#include "GLObjects.h"
#include "glslprogram.h"

#include <cstdio>

#include <iostream>

#include <SDL2/SDL.h>

#define WIDTH 640
#define HEIGHT 480

using namespace std;

using rsw::vec4;
using rsw::mat4;

vec4 Red(1.0f, 0.0f, 0.0f, 0.0f);
vec4 Green(0.0f, 1.0f, 0.0f, 0.0f);
vec4 Blue(0.0f, 0.0f, 1.0f, 0.0f);

SDL_Window* window;
SDL_GLContext glcontext;


void cleanup();
void setup_context();

int main(int argc, char** argv)
{

	if (argc < 2) {
		printf("usage: %s fragment_shader [refresh_delay]\n", argv[0]);
		printf("The shader is reloaded every refresh_delay seconds.\n");
		printf("refresh delay defaults to 3 and it can be fractional ie 3.5)\n");
		return 0;
	}

	char* shader_file = argv[1];
	int refresh_delay = 3000;

	if (argc >= 3) {
		float tmp = atof(argv[2]);
		printf("tmp = %f\n", tmp);
		if (tmp != 0.0 && tmp < 30)
			refresh_delay = tmp * 1000;
	}


	setup_context();


	float points[] = { -1.0,  1.0, 0,
	                   -1.0, -1.0, 0,
	                    1.0, -1.0, 0,

	                   -1.0,  1.0, 0,
	                    1.0, -1.0, 0,
	                    1.0,  1.0, 0 };
	mat4 identity;

	GLSLProgram prog, tmp_prog;
	printf("sizeof(points) = %lu\n", sizeof(points));

	Buffer triangle(1);
	triangle.bind(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(GLSLShader::ATTR_VERTEX);
	glVertexAttribPointer(GLSLShader::ATTR_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);


	compile_link_shaders(prog, 2, GLSLShader::VERTEX, "../shadertoy.vp", GLSLShader::FRAGMENT, shader_file);

	prog.set_uniform("iResolution", vec3(WIDTH, HEIGHT, WIDTH/float(HEIGHT)));

	//glClearColor(0, 0, 0, 1);

	

	SDL_Event e;
	int quit = 0;
	float xres = WIDTH, yres = HEIGHT;

	unsigned int old_time = 0, new_time=0, counter = 0, tmp;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = 1;
				break;

			case SDL_KEYDOWN:
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE:
					quit = 1;
					break;
				case SDL_SCANCODE_RETURN:
					if (compile_link_shaders(tmp_prog, 2, GLSLShader::VERTEX, "../shadertoy.vp", GLSLShader::FRAGMENT, shader_file)) {
						prog.delete_program();
						prog = tmp_prog;
					}
				}

			case SDL_WINDOWEVENT:
				switch (e.window.event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					xres = e.window.data1;
					yres = e.window.data2;
					//don't think that z value is what shadertoy.com uses but oh well
					prog.set_uniform("iResolution", vec3(xres, yres, xres/yres));
					glViewport(0, 0, xres, yres);
					break;
				}
				break;
			}
		}

		new_time = SDL_GetTicks();
		tmp = new_time - old_time;
		if (tmp > refresh_delay) {
			printf("%.3f FPS\n", (float)counter*1000/(float)tmp);
			old_time = new_time;
			counter = 0;
			if (compile_link_shaders(tmp_prog, 2, GLSLShader::VERTEX, "../shadertoy.vp", GLSLShader::FRAGMENT, shader_file)) {
				prog.delete_program();
				prog = tmp_prog;
				prog.set_uniform("iResolution", vec3(xres, yres, xres/yres));
			}
			tmp_prog.delete_program();
		}

		prog.set_uniform("iGlobalTime", new_time/1000.0f);
		
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		SDL_GL_SwapWindow(window);

		++counter;
	}

	cleanup();

	return 0;
}


void setup_context()
{
	if (SDL_Init(SDL_INIT_VIDEO)) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		exit(0);
	}

	window = SDL_CreateWindow("Ex 1", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	if (!window) {
		cleanup();
		exit(0);
	}

	glcontext = SDL_GL_CreateContext(window);
	
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("Error: %s\n", glewGetErrorString(err));
		cleanup();
		exit(0);
	}
}

void cleanup()
{
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);

	SDL_Quit();
}
