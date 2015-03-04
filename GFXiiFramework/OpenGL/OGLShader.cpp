#include <stdio.h>
#include <stdlib.h>
#include "GLEW/include/glew.h"

#include "OGLShader.h"

static void TextFileBufferedRead(LPCWSTR filename, char** buffer)
{
	FILE *pfile = NULL;
	*buffer = NULL;
	int count = 0;

	if(filename == NULL) return;

	_wfopen_s(&pfile, filename, L"rt");

	if(!pfile) return;

	fseek(pfile, 0, SEEK_END);
	count = ftell(pfile);	
	rewind(pfile);

	if(count > 0)
	{
		*buffer = new char[count+1];

		count = fread(*buffer, sizeof(char), count, pfile);
		(*buffer)[count] = '\0';
	}
	
	fclose(pfile);

	return;
}

OGLShader::OGLShader()
{
	m_ownerProgram = INVALID_SHADER_PROGRAM_HANDLE;	
}

OGLShader::~OGLShader()
{
	DeleteShader();
}

EShaderError OGLShader::CreateShaderFromSourceFile(LPCWSTR filename, EShaderType type)
{

	m_shaderInternal._type = type;

	if(type == SHADER_UNDEFINED)
		return SHADER_ERROR_CREATE;

	GLhandleARB shaderhandle;
	GLenum shadertype;

	switch(type)
	{
		case SHADER_VERTEX: shadertype = GL_VERTEX_SHADER; break;
		case SHADER_FRAGMENT: shadertype = GL_FRAGMENT_SHADER; break;
		case SHADER_UNDEFINED: break;
	}

	//Create a shader handle from the device.
	shaderhandle = glCreateShader(shadertype);
	
	if(shaderhandle == 0 || shaderhandle == GL_INVALID_ENUM || shaderhandle == GL_INVALID_OPERATION)
	{
		return SHADER_ERROR_CREATE;
	}
	
	m_shaderInternal._syshandle = shaderhandle;	//store the shader handle.

	//TODO: try using AUTO_PTR here?
	char* source;
	TextFileBufferedRead(filename, &source);
	//upload the source code to the shader program
	glShaderSource(shaderhandle, 1, (const char**)&source, NULL);
	delete [] source;

	//TODO: Search documents to find out the way to catch shader compilation errors etc.

	//compile the bugger.
	GLint result = 0;
	glCompileShader(shaderhandle);
	glGetShaderiv(shaderhandle, GL_INFO_LOG_LENGTH, &result);
	
	if(result > 0)
	{
		char* log = new char[result];
		int charlen = 0;
		glGetShaderInfoLog( shaderhandle, result, &charlen, log);

		fprintf(stdout, "Shader log %s\n", log);
		delete [] log;
	}

	return SHADER_ERROR_NONE;
}

void OGLShader::DeleteShader()
{
	ShaderHandle handle = m_shaderInternal._syshandle;

	if(handle != INVALID_SHADER_HANDLE)
	{
		if(m_ownerProgram != INVALID_SHADER_PROGRAM_HANDLE)
		{
			glDetachObjectARB(m_ownerProgram, m_shaderInternal._syshandle);
		}
		glDeleteObjectARB(handle);
		m_shaderInternal._syshandle = INVALID_SHADER_HANDLE;
		m_shaderInternal._handle = INVALID_SHADER_HANDLE;
		m_shaderInternal._type = SHADER_UNDEFINED;
		m_shaderInternal._name[0] = '\0';
		m_ownerProgram = INVALID_SHADER_PROGRAM_HANDLE;
	}
}

void OGLShader::SetShaderOwnerProgram(ShaderProgramHandle handle)
{
	m_ownerProgram = handle;
}
// END_OF ShaderGL
//
// BEGINNING_OF ShaderProgramGL
OGLShaderProgram::~OGLShaderProgram()
{
	DeleteShaderProgram();
}

void OGLShaderProgram::CreateShaderProgram()
{
	if(m_systemHandle != INVALID_SHADER_PROGRAM_HANDLE) //A shader program has already been created.
		return;

	//TODO: Validate the program handle.
	m_systemHandle = glCreateProgram();
}

void OGLShaderProgram::AttachShader(ShaderBase* shader)
{
	if(m_systemHandle != INVALID_SHADER_PROGRAM_HANDLE && shader->GetShaderSysHandle() != INVALID_SHADER_HANDLE)
	{
		m_shaders[m_shaderCount] = shader;
		m_shaderCount += 1;
		shader->SetShaderOwnerProgram(m_systemHandle);

		glAttachShader(m_systemHandle, shader->GetShaderSysHandle());
	}
}

void OGLShaderProgram::BuildShaderProgram()
{
	GLint result = 0;

	glLinkProgram(m_systemHandle);
	
	glGetProgramiv(m_systemHandle, GL_INFO_LOG_LENGTH, &result);
	
	if(result > 0)
	{
#ifdef _DEBUG
		char* log = new char[result];
		int outlength;
		glGetProgramInfoLog(m_systemHandle, result, &outlength, log);

		fprintf(stdout, "Link log: %s\n", log);

		delete [] log;
#endif
	}
}

void OGLShaderProgram::DeleteShaderProgram()
{
	int numAttached = m_shaderCount;

	if(m_systemHandle != INVALID_SHADER_PROGRAM_HANDLE)
	{
		for(int i = 0; i < numAttached; i++)
		{
			delete m_shaders[i];
			m_shaders[i] = NULL;
		}

		glDeleteObjectARB(m_systemHandle);
		m_systemHandle = INVALID_SHADER_PROGRAM_HANDLE;
	}
}

void OGLShaderProgram::AttachAndCompileShaderFromFile(LPCWSTR filename, EShaderType type)
{
	if(m_shaderCount == MAX_NUM_SHADER_PER_PROGRAM)
		return;

	ShaderBase* shader = new OGLShader();
	
	if(shader)
	{
		if(shader->CreateShaderFromSourceFile(filename, type) == SHADER_ERROR_NONE)
		{
			AttachShader(shader);
		}
		else
		{
			delete shader;
		}
	}
}
void OGLShaderProgram::ActivateShaderProgram() 
{ 
	if(m_systemHandle!=INVALID_SHADER_PROGRAM_HANDLE) 
		glUseProgram(m_systemHandle); 
}

void OGLShaderProgram::DeactivateShaderProgram()
{
	if(m_systemHandle != INVALID_SHADER_PROGRAM_HANDLE)
		glUseProgram(0);
}

void OGLShaderProgram::SetUniform1UInt(unsigned int value, const char* param)
{
	int param_loc = glGetUniformLocationARB(m_systemHandle, param);

	glUniform1iARB(param_loc, value);
}

void OGLShaderProgram::SetAttrib4Float(float f1, float f2, float f3, float f4, const char* param)
{
	int param_loc = glGetAttribLocationARB(m_systemHandle, param);

	glVertexAttrib4fARB(param_loc, f1, f2, f3, f4);
}

void OGLShaderProgram::BindAttributeLocation( int index, const char* param_name )
{
	glBindAttribLocation( m_systemHandle, index, param_name );
}
