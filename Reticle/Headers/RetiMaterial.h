#ifndef RETIMATERIAL_H_INCLUDED
#define RETIMATERIAL_H_INCLUDED

#include <vector>
#include <unordered_map>
#include <string>

#include <includes.h>

class RetiTexture;
class RetiShader;

class RetiMaterial
{
private:

    static std::unordered_map<std::string, RetiShader*> path_to_shader;

    RetiShader* shader;
    std::unordered_map<std::string, int> texture_name_unit;
    std::vector<std::string> names;
    std::vector<RetiTexture*> textures;                         //TODO: Spinlock.

    bool is_loaded;
    bool is_shader_bound;

    void initialize_shader(const std::string&, const std::string&);

    void initialize_from_paths(const std::string&, const std::string&, std::vector<std::string>&);

public:

    /** Create a new material using the default Reticle shader interface. */
    RetiMaterial();

    /** Create a new material using the shader interface specified by file at path given by arg1. */
    RetiMaterial(const std::string&);

    /** Create a new material using the vertex shader code in text file at string path arg1, and fragment
    *   shader code at string path arg2, and texture names given by vector of strings arg3.
    */
    RetiMaterial(const std::string&, const std::string&, std::vector<std::string>&);

    /** Create a new material using the vertex shader code in text file at string path arg1, and fragment
    *   shader code at string path arg2, and texture names given by vector of strings arg3. Also assigns the
    *   i'th texture in vector of RetiTexture pointers arg4 to the i'th name in arg3. Produces undefined
    *   behavior if arg3 and arg4 have different lengths.
    */
    RetiMaterial(const std::string&, const std::string&, std::vector<std::string>&, std::vector<RetiTexture*>&);

    /** Copy constructor */
    RetiMaterial(const RetiMaterial&);

    /** Destructor */
    ~RetiMaterial();
    RetiMaterial& operator=(const RetiMaterial&);

    RetiShader* getShader();

    void setTexture(const std::string&, RetiTexture*);

    void useMaterial();

    void loadMaterial();
    void unloadMaterial();

};

#endif // RETIMATERIAL_H_INCLUDED
