#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath), m_RendererID(0)
{
    // gets source code of shader from relative path
    ShaderProgramSource source = ParseShader(filepath);
    // creates the shader
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

// takes a file path as input
// returns vertex and fragment shaders as parsed strings in a ShaderProgramSource data structure {VertexSource, FragmentSource}
ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    // iterates through input file line by line
    while (getline(stream, line))
    {
        // checks if line contains shader syntax
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                // set mode to vertex
                //std::cout << "Vertex Found" << std::endl;
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // set mode to fragment
                //std::cout << "Fragment Found" << std::endl;
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            // cast type to an int, push to string stream for respective type
            ss[(int)type] << line << '\n';
        }
    }
    // returns shader program source struct
    return { ss[0].str(), ss[1].str() };
}

// compiles the shader, taking in the source as a string, and the type as an int
// returns the id of the shader as an unsigned int
unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
    GLCall(unsigned int id = glCreateShader(type));
    // pointer to the characters in the string
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    // compiles the shader
    GLCall(glCompileShader(id));

    // gets the shader, stores compile status int to result
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    // Error handling
    // checks the result against GL_FALSE (int == 0)
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        // alloca lets you alloca on the stack dynamically
        char* message = (char*)_malloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));

        return 0;
    }
    return id;
}

// create shader function, takes a vertex shader and fragment shader as strings
// returns an id of the shader (hence the int initializations)
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    // glCreateProgram returns an unsigned int
    unsigned int program = glCreateProgram(); 
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform1iv(const std::string& name, int count, int value[])
{
    GLCall(glUniform1iv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
    // get location from cache if possible
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist." << std::endl;
    
    // store location in cache
    m_UniformLocationCache[name] = location;
    return location;
}
