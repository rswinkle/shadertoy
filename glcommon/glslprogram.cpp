#include "glslprogram.h"


#include <fstream>
using std::ifstream;
using std::ios;

#include <sstream>
using std::ostringstream;

#include <sys/stat.h>

GLSLProgram::GLSLProgram() : handle(0), linked(false) { }

bool GLSLProgram::compile_shader_file(const char * fileName,
                                      GLSLShader::GLSLShaderType type)
{
	if (!file_exists(fileName)) {
		logString = "File not found.";
		return false;
	}

	if (handle <= 0) {
		handle = glCreateProgram();
		if (handle == 0) {
			logString = "Unable to create shader program.";
			return false;
		}
	}

	ifstream inFile( fileName, ios::in );
	if (!inFile) {
		return false;
	}

	ostringstream code;
	//this right here is not good it reads EOF into the source in linux
	
	int c = inFile.get();
	while (inFile.good()) {
		code << (char) c;
		c = inFile.get();
	}
	inFile.close();


	return compile_shader_string(code.str(), type);
}

bool GLSLProgram::compile_shader_string(const string & source, GLSLShader::GLSLShaderType type)
{
	if (handle <= 0) {
		handle = glCreateProgram();
		if (handle == 0) {
			logString = "Unable to create shader program.";
			return false;
		}
	}

	GLuint shaderHandle = 0;

	switch (type) {
	case GLSLShader::VERTEX:
		shaderHandle = glCreateShader(GL_VERTEX_SHADER);
		vert_shader = shaderHandle;
		break;
	case GLSLShader::FRAGMENT:
		shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
		frag_shader = shaderHandle;
		break;
	case GLSLShader::GEOMETRY:
		shaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
		geometry_shader = shaderHandle;
		break;
	case GLSLShader::TESS_CONTROL:
		shaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER);
		tess_control_shader = shaderHandle;
		break;
	case GLSLShader::TESS_EVALUATION:
		shaderHandle = glCreateShader(GL_TESS_EVALUATION_SHADER);
		tess_eval_shader = shaderHandle;
		break;
	default:
		return false;
	}

	
	const char * c_code = source.c_str();

	//fprintf(stderr, "\nsize = %d\n\n%s\n\n", int(source.size()), c_code);
	glShaderSource(shaderHandle, 1, &c_code, NULL);

	// Compile the shader
	glCompileShader(shaderHandle);

	// Check for errors
	int result;
	glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &result );
	if (GL_FALSE == result) {
		// Compile failed, store log and return false
		int length = 0;
		logString = "";
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length );
		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			glGetShaderInfoLog(shaderHandle, length, &written, c_log);
			logString = c_log;
			delete [] c_log;
		}

		return false;
	} else {
		/*
		if (linked) {
			switch (type) {
			case GLSLShader::VERTEX:
				glDetachShader(handle, vert_shader);
				break;
			case GLSLShader::FRAGMENT:
				glDetachShader(handle, frag_shader);
				break;
			case GLSLShader::GEOMETRY:
				glDetachShader(handle, geometry_shader);
				break;
			case GLSLShader::TESS_CONTROL:
				glDetachShader(handle, tess_control_shader);
				break;
			case GLSLShader::TESS_EVALUATION:
				glDetachShader(handle, tess_eval_shader);
				break;
			}
			linked = false;
		}
		*/
		// Compile succeeded, attach shader and return true
		glAttachShader(handle, shaderHandle);

		//It won't actually be deleted till the shader
		//is detached or the program is deleted
		//It is simply flagged for deletion.
		glDeleteShader(shaderHandle);
		
		return true;
	}
}

bool GLSLProgram::link()
{
	if (linked) return true;
	if (handle <= 0) return false;

	glLinkProgram(handle);

	int status = 0;
	glGetProgramiv( handle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		// Store log and return false
		int length = 0;
		logString = "";

		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length );

		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			glGetProgramInfoLog(handle, length, &written, c_log);
			logString = c_log;
			delete [] c_log;
		}

		return false;
	} else {
		linked = true;
		return linked;
	}
}

void GLSLProgram::use()
{
	if (handle <= 0 || (!linked)) return;
	glUseProgram(handle);
}

string GLSLProgram::log()
{
	return logString;
}

int GLSLProgram::get_handle()
{
	return handle;
}

bool GLSLProgram::isLinked()
{
	return linked;
}

void GLSLProgram::bindAttribLocation(GLuint location, const char * name)
{
	glBindAttribLocation(handle, location, name);
}

void GLSLProgram::bindFragDataLocation(GLuint location, const char * name)
{
	glBindFragDataLocation(handle, location, name);
}

void GLSLProgram::set_uniform( const char *name, float x, float y)
{
	int loc = get_uniform_location(name);
	if (loc >= 0) {
		glUniform2f(loc,x,y);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

void GLSLProgram::set_uniform( const char *name, float x, float y, float z)
{
	int loc = get_uniform_location(name);
	if (loc >= 0) {
		glUniform3f(loc,x,y,z);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

void GLSLProgram::set_uniform(const char *name, const vec2 & v)
{
	this->set_uniform(name,v.x,v.y);
}

void GLSLProgram::set_uniform(const char *name, const vec3 & v)
{
	this->set_uniform(name,v.x,v.y,v.z);
}

void GLSLProgram::set_uniform(const char *name, const vec4 & v)
{
	int loc = get_uniform_location(name);
	if (loc >= 0) {
		glUniform4f(loc,v.x,v.y,v.z,v.w);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

void GLSLProgram::set_uniform(const char *name, const mat4 & m)
{
	int loc = get_uniform_location(name);
	if (loc >= 0) {
		glUniformMatrix4fv(loc, 1, GL_TRUE, (GLfloat*)m.matrix);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

void GLSLProgram::set_uniform(const char *name, const mat3 & m)
{
	int loc = get_uniform_location(name);
	if (loc >= 0) {
		glUniformMatrix3fv(loc, 1, GL_TRUE, (GLfloat*)m.matrix);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

void GLSLProgram::set_uniform(const char *name, float val)
{
	int loc = get_uniform_location(name);
	if (loc >= 0) {
		glUniform1f(loc, val);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

void GLSLProgram::set_uniform(const char *name, int val)
{
	int loc = get_uniform_location(name);
	if (loc >= 0) {
		glUniform1i(loc, val);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

void GLSLProgram::set_uniform(const char *name, bool val)
{
	int loc = get_uniform_location(name);
	if (loc >= 0) {
		glUniform1i(loc, val);
	} else {
		printf("Uniform: %s not found.\n",name);
	}
}

void GLSLProgram::print_active_uniforms()
{

	GLint nUniforms, size, location, maxLen;
	GLchar * name;
	GLsizei written;
	GLenum type;

	glGetProgramiv( handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
	glGetProgramiv( handle, GL_ACTIVE_UNIFORMS, &nUniforms);

	name = (GLchar *) malloc( maxLen );

	printf(" Location | Name\n");
	printf("------------------------------------------------\n");
	for (int i = 0; i < nUniforms; ++i) {
		glGetActiveUniform( handle, i, maxLen, &written, &size, &type, name );
		location = glGetUniformLocation(handle, name);
		printf(" %-8d | %s\n",location, name);
	}

	free(name);
}

void GLSLProgram::print_active_attribs()
{
	GLint written, size, location, maxLength, nAttribs;
	GLenum type;
	GLchar * name;

	glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
	glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &nAttribs);

	name = (GLchar *) malloc( maxLength );

	printf(" Index | Name\n");
	printf("------------------------------------------------\n");
	for (int i = 0; i < nAttribs; i++) {
		glGetActiveAttrib( handle, i, maxLength, &written, &size, &type, name );
		location = glGetAttribLocation(handle, name);
		printf(" %-5d | %s\n",location, name);
	}

	free(name);
}

bool GLSLProgram::validate()
{
	if (!isLinked()) return false;

	GLint status;
	glValidateProgram( handle );
	glGetProgramiv( handle, GL_VALIDATE_STATUS, &status );

	if (GL_FALSE == status) {
		// Store log and return false
		int length = 0;
		logString = "";

		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length );

		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			glGetProgramInfoLog(handle, length, &written, c_log);
			logString = c_log;
			delete [] c_log;
		}

		return false;
	} else {
	   return true;
	}
}

int GLSLProgram::get_uniform_location(const char * name)
{
	return glGetUniformLocation(handle, name);
}

bool GLSLProgram::file_exists(const string & fileName)
{
	struct stat info;
	int ret = -1;

	ret = stat(fileName.c_str(), &info);
	return 0 == ret;
}



int GLSLProgram::get_uniform_block_info(unsigned int block_index, GLenum info)
{
	GLint temp;
	glGetActiveUniformBlockiv(handle, block_index, info, &temp);
	return temp;
	
}




void GLSLProgram::delete_program()
{
	glDeleteProgram(handle);
	handle = 0;
	linked = false;
}



int compile_link_shaders(GLSLProgram& prog, int num_shaders, ...)
{
	va_list shader_list;
	va_start(shader_list, num_shaders);

	int type;
	char *file= NULL;
	// Iterate over this argument list
	for (int i=0; i<num_shaders; ++i) {
	
		type = va_arg(shader_list, int);	// Number of attributes
		file = va_arg(shader_list, char*);
		if (!prog.compile_shader_file(file, GLSLShader::GLSLShaderType(type)))
		{
			printf("%s shader failed to compile!\n%s", file, prog.log().c_str());
			return 0;
		}
	}
	va_end(shader_list);

	if (!prog.link()) {
		printf("Shader program failed to link!\n%s",
			   prog.log().c_str());
		return 0;
	}

	prog.use();
	return 1;
}
