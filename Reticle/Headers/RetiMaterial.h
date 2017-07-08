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

    RetiMaterial();
    RetiMaterial(const std::string&);
    RetiMaterial(const std::string&, const std::string&, std::vector<std::string>&);
    RetiMaterial(const std::string&, const std::string&, std::vector<std::string>&, std::vector<RetiTexture*>&);
    RetiMaterial(const RetiMaterial&);
    ~RetiMaterial();
    RetiMaterial& operator=(const RetiMaterial&);

    RetiShader* getShader();

    void setTexture(std::string, RetiTexture*);

    void useMaterial();

    void loadMaterial();
    void unloadMaterial();

};

#endif // RETIMATERIAL_H_INCLUDED
