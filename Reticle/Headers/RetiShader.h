#ifndef RETISHADER_H_INCLUDED
#define RETISHADER_H_INCLUDED


#include <includes.h>

#include <string>

#include <includes.h>

/// A SECTION OF DEFINES THAT SPECIFIES INTERNAL RETICLE STUFF....

#define RETI_UNAME_MVP "mvp_matrix"

class RetiRenderer;

class RetiShader
{

    friend class RetiMaterial;

private:
    char* vertex_shader_code;
    char* fragment_shader_code;

    bool is_loaded;

    GLuint shader_program_id;

    GLuint mvp_pos;

    char* load_shader_code(const char* path);

    void compile_shader(GLuint& shaderId, char* code, GLenum type);

    int get_uniform_location(std::string name);
    void set_uniform_int(int pos, int val);
    void set_uniform_float(int pos, float val);

public:

    RetiShader();
    RetiShader(const char* vertPath, const char* fragPath);
    RetiShader(const RetiShader& other);
    ~RetiShader();
    RetiShader& operator=(const RetiShader& other);

    void useVertexShader(char* vertCode);
    void useFragmentShader(char* fragCode);

    void loadShader();
    void unloadShader();

    void setActiveShader();

    void setMVP(glm::mat4 mvp_mat);
};

#endif // RETISHADER_H_INCLUDED
