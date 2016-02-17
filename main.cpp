#include "glslprogram.h"

#include <cstdio>
#include <cassert>

#include <SDL2/SDL.h>

#define WIDTH 640
#define HEIGHT 480

#define ATTR_VERTEX 0

using namespace std;




SDL_Window* window;
SDL_GLContext glcontext;


void cleanup();
void setup_context();

int main(int argc, char** argv)
{

	if (argc < 2) {
		printf("usage: %s fragment_shader [refresh_delay=3]\n\n", argv[0]);
		printf("The shader is reloaded every refresh_delay seconds.\n");
		printf("refresh_delay can be fractional ie 3.5)\n");
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



	GLuint triangle;
	glGenBuffers(1, &triangle);
	glBindBuffer(GL_ARRAY_BUFFER, triangle);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(ATTR_VERTEX);
	glVertexAttribPointer(ATTR_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);


	const char vs_text[] = "#version 330\nlayout (location = 0) in vec4 v_vertex;\nvoid main(void) { gl_Position = v_vertex; }\n"

	GLuint vert_shader = compile_shader_str(vs_text, GL_VERTEX_SHADER);
	assert(vert_shader);
	GLuint frag_shader = compile_shader_file(shader_file, GL_FRAGMENT_SHADER);
	if (!frag_shader)
		return 0;

	GLuint prog = glCreateProgram();
	if (!prog)
		return 0;
	glAttachShader(


	GLSLProgram prog;
	compile_link_shaders(prog, 2, GLSLShader::VERTEX, "../shadertoy.vp", GLSLShader::FRAGMENT, shader_file);

	prog.set_uniform("iResolution", vec3(WIDTH, HEIGHT, WIDTH/float(HEIGHT)));


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
					if (compile_link_shaders(prog, 2, GLSLShader::VERTEX, "../shadertoy.vp", GLSLShader::FRAGMENT, shader_file)) {
						prog.set_uniform("iResolution", vec3(xres, yres, xres/yres));
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
			if (compile_link_shaders(prog, 2, GLSLShader::VERTEX, "../shadertoy.vp", GLSLShader::FRAGMENT, shader_file)) {
				prog.set_uniform("iResolution", vec3(xres, yres, xres/yres));
			}
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


int compile_shader_file(const char* filename, GLenum shader_type)
{
	FILE *file = fopen(filename, "r");
	if (!file)
		return 0;

	char* shader_str = NULL;
	if (!file_read(file, &shader_str))
		return 0;

	return compile_shader_str(shader_str, shader_type);
}


int compile_shader_str(const char* shader_str, GLenum shader_type)
{
	GLuint shader = glCreateShader(shader_type);
	if (!shader)
		return 0;

	glShaderSource(shader, 1, &shader_str, NULL);
	glCompileShader(shader);

	int result;
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result) {
		int length = 0;
		glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			char* c_log = (char*)malloc(length);
			int written = 0;
			glGetShaderInfoLog(shaderHandle, length, &written, c_log);
			printf("%s\n", c_log);
			free(c_log);
		}

		return 0;
	}

	return shader;
}



int load_shaders(GLuint* program, char* frag_file)
{

	
	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	if (!vert_shader)
		return 0;


	
	glShaderSource(vert_shader, 1, &vs_text, NULL);

	glCompileShader(vert_shader);







	FILE* file = fopen(frag_file, "r");
	
	char* frag_text = NULL;
	if (!file_read(file, &frag_text)) {
		return 0;
	}

	GLuint tmp_prog;
	tmp_prog = glCreateProgram();
	if (!tmp_prog) {
		free(frag_text);
		return 0;
	}
	


	if (*program = 0) {
		handle = glCreateProgram();
		if (handle == 0) {
			logString = "Unable to create shader program.";
			return false;
		}
	}
	


int file_read(FILE* file, char** str)
{
	char* data;
	size_t size;

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	if (!size) {
		fclose(file);
		return 0;
	}

	data = malloc(size+1);
	if (!data) {
		fclose(file);
		return 0;
	}

	rewind(file);
	if (!fread(data, size, 1, file)) {
		printf("read failure\n");
		fclose(file);
		free(data);
		return 0;
	}

	data[size] = 0; /* null terminate in all cases even if reading binary data */

	out->data = data;
	out->len = size;
	out->elem_size = 1;

	fclose(file);
	return size;
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
