#include <includes.h>

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>


#include <RetiShader.h>
#include <RetiRenderer.h>
#include <Core/RetiLog.h>

using namespace std;

char* RetiShader::load_shader_code(const char* path)
{
    ifstream sourceFile(path);
    if(!sourceFile.is_open())
        RetiLog::logln("FAILED TO OPEN FILE: " + string(path));

    string sourceStr = "";
    string line;
    while(getline(sourceFile, line))
        sourceStr += line + "\n";

    char* ret = new char[sourceStr.size() + 1];
    size_t lst = sourceStr.copy(ret, sourceStr.size()+1);
    ret[lst] = '\0';

    return ret;
}

void RetiShader::compile_shader(GLuint& shaderId, char* code, GLenum type)
{
    shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, &code, NULL);
    glCompileShader(shaderId);
    GLint compileStatus;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
    if(compileStatus != GL_TRUE)
    {
        char* log = new char[65536];
        glGetShaderInfoLog(shaderId, 65536, NULL, log);
        RetiLog::logln("SHADER COMPILE ERROR: " + string(code));
        RetiLog::logln(log);
    }
}

RetiShader::RetiShader()
{
    string vertRelPath= "/Shaders/Default_v.shdr";
    string vertPath = RetiRenderer::getReticleRootDirectory() + vertRelPath;
    vertex_shader_code = load_shader_code(vertPath.c_str());

    string fragRelPath = "/Shaders/Default_f.shdr";
    string fragPath = RetiRenderer::getReticleRootDirectory() + fragRelPath;
    fragment_shader_code = load_shader_code(fragPath.c_str());

    is_loaded = false;
}

RetiShader::RetiShader(const char* vertPath, const char* fragPath)
{
    vertex_shader_code = load_shader_code(vertPath);
    fragment_shader_code = load_shader_code(fragPath);

    is_loaded = false;
}

RetiShader::RetiShader(const RetiShader& other)
{
    vertex_shader_code = new char[sizeof(other.vertex_shader_code)];
    strcpy(vertex_shader_code, other.vertex_shader_code);
    fragment_shader_code = new char[sizeof(other.fragment_shader_code)];
    strcpy(fragment_shader_code, other.fragment_shader_code);

    is_loaded = false;
}

RetiShader::~RetiShader()
{
    delete[] vertex_shader_code;
    delete[] fragment_shader_code;
}

RetiShader& RetiShader::operator=(const RetiShader& other)
{
    RetiShader* pCpy = new RetiShader(other);
    return *pCpy;
}

void RetiShader::useVertexShader(char* vertCode)
{
    vertex_shader_code = vertCode;
}

void RetiShader::useFragmentShader(char* fragCode)
{
    fragment_shader_code = fragCode;
}

void RetiShader::loadShader()
{
    if(is_loaded)
        return;

    GLuint vertex_shader_id = 0;
    GLuint fragment_shader_id = 0;

    compile_shader(vertex_shader_id, vertex_shader_code, GL_VERTEX_SHADER);
    compile_shader(fragment_shader_id, fragment_shader_code, GL_FRAGMENT_SHADER);

    GLint vertexCompileStatus;
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &vertexCompileStatus);
    if(vertexCompileStatus != GL_TRUE)
    {
        char log[65536];
        glGetShaderInfoLog(vertex_shader_id, 65536, NULL, log);
        RetiLog::logln("VERTEX SHADER COMPILE ERROR: \n" + string(log));
    }

    GLint fragmentCompileStatus;
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &fragmentCompileStatus);
    if(fragmentCompileStatus != GL_TRUE)
    {
        char log[65536];
        glGetShaderInfoLog(fragment_shader_id, 65536, NULL, log);
        RetiLog::logln("FRAGMENT SHADER COMPILE ERROR: \n" + string(log));
    }

    shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id, vertex_shader_id);
    glAttachShader(shader_program_id, fragment_shader_id);
    glLinkProgram(shader_program_id);

    GLint linkStatus;
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &linkStatus);
    if(linkStatus != GL_TRUE)
    {
        char log[65536];
        glGetProgramInfoLog(shader_program_id, 65536, NULL, log);
        RetiLog::logln("SHADER LINK ERROR: " + string(log));
    }

    mvp_pos = glGetUniformLocation(shader_program_id, "mvp_matrix");
    if(mvp_pos <= 0)
        RetiLog::logln("WARNING: MVP position matrix position is not strictly positive.");

    glDetachShader(shader_program_id, vertex_shader_id);
    glDetachShader(shader_program_id, fragment_shader_id);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    is_loaded = true;
}

void RetiShader::unloadShader()
{
    glUseProgram(0);
    glDeleteProgram(shader_program_id);
    is_loaded = false;
}

void RetiShader::setActiveShader()
{
    if(!is_loaded)
        loadShader();

    glUseProgram(shader_program_id);
}

int RetiShader::get_uniform_location(string name)
{
    if(!is_loaded)
    {
        RetiLog::logln("Uniform access call on unloaded shader, attempting to load.");
        loadShader();
    }
    return glGetUniformLocation(shader_program_id, name.c_str());
}

void RetiShader::set_uniform_int(int pos, int val)
{
    if(!is_loaded)
    {
        RetiLog::logln("Uniform access call on unloaded shader, attempting to load.");
        loadShader();
    }
    glUniform1i(pos, val);
}

void RetiShader::set_uniform_float(int pos, float val)
{
    if(!is_loaded)
    {
        RetiLog::logln("Uniform access call on unloaded shader, attempting to load.");
        loadShader();
    }
    glUniform1f(pos, val);
}

void RetiShader::setMVP(glm::mat4 mvp_mat)
{
    setActiveShader();
    glUniformMatrix4fv(mvp_pos, 1, GL_FALSE, glm::value_ptr(mvp_mat));
}
