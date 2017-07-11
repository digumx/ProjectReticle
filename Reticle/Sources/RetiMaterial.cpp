#include <includes.h>

#include <fstream>

#include <RetiMaterial.h>
#include <RetiTexture.h>
#include <RetiShader.h>
#include <RetiRenderer.h>
#include <Core/RetiLog.h>

using namespace std;

unordered_map<string, RetiShader*> RetiMaterial::path_to_shader;

void RetiMaterial::initialize_shader(const string& vertpath, const string& fragpath)
{
    string sid = vertpath + fragpath;
    if(path_to_shader[sid] == nullptr)
    {
        path_to_shader[sid] = new RetiShader(vertpath.c_str(), fragpath.c_str());
        is_shader_bound = true;
    }
    else is_shader_bound = false;
    shader = path_to_shader[sid];
}

void RetiMaterial::initialize_from_paths(const string& vertPath, const string& fragPath, vector<string>& tnames)
{
    initialize_shader(vertPath, fragPath);
    texture_name_unit.reserve(tnames.size());
    names.reserve(tnames.size());
    for(int i = 0; i < tnames.size(); i++)
    {
        texture_name_unit[tnames[i]] = i;
        names.push_back(tnames[i]);
        textures.push_back(RetiTexture::getDefaultTexture());
    }
}

RetiMaterial::RetiMaterial() : RetiMaterial(RetiRenderer::getReticleRootDirectory() + "/Shaders/Default.shif") {}

RetiMaterial::RetiMaterial(const string& filePath)
{
    string dir = filePath.substr(0, filePath.find_last_of({'/', '\\'}) + 1);

    ifstream matFile(filePath.c_str());
    if(!matFile.is_open())
        RetiLog::logln("WARNING: Falied to open file: " + filePath + ", Segfault imminent.");

    string line1, line2, line3;
    getline(matFile, line1);
    getline(matFile, line2);
    getline(matFile, line3);

    int nTex = stoi(line3);

    if(nTex > 15)
        RetiLog::logln("WARNING: Support for more than 16 textures is platform dependent.");

    vector<string> texNames;

    string line;
    for(int i = 0; i < nTex; i++)
    {
        getline(matFile, line);
        texNames.push_back(line);
    }

    initialize_from_paths(dir + line1, dir + line2, texNames);

    is_loaded = false;
}

RetiMaterial::RetiMaterial(const string& vertPath, const string& fragPath, vector<string>& tnames)
{
    initialize_from_paths(vertPath, fragPath, tnames);
    is_loaded = false;
}

RetiMaterial::RetiMaterial(const string& vertPath, const string& fragPath, vector<string>& names, vector<RetiTexture*>& texes)
                            : RetiMaterial(vertPath, fragPath, names)
{
    textures.reserve(texes.size());
    for(int i = 0; i < texes.size(); i++)
        textures.push_back(texes[i]);
}

RetiMaterial::RetiMaterial(const RetiMaterial& other)
{
    shader = other.shader;
    texture_name_unit = other.texture_name_unit;
    textures = other.textures;

    is_loaded = false;
}

RetiMaterial& RetiMaterial::operator=(const RetiMaterial& other)
{
    if(is_loaded)
    {
        RetiLog::logln("WARNING: Copy assignment to loaded shader, ignoring call.");
        return *this;
    }

    shader = other.shader;
    texture_name_unit = other.texture_name_unit;
    textures = other.textures;

    is_loaded = false;

    return *this;
}

RetiMaterial::~RetiMaterial()
{
    if(is_loaded)
    {
        RetiLog::logln("WARNING: Deletine called on loaded material, attempting to unload.");
        unloadMaterial();
    }
}

void RetiMaterial::loadMaterial()
{
    if(!is_shader_bound)
    {
        shader->setActiveShader();
        for(int i = 0; i < names.size(); i++)
            shader->set_uniform_int(shader->get_uniform_location(names[i]), GL_TEXTURE0 + i);
    }

    if(is_loaded)
        return;

    #ifdef VERBOSE_ON
    RetiLog::logln("Loading texture uniforms: ");
    for(int i = 0; i < names.size(); i++)
        RetiLog::logln(":" + names[i] + ":");
    #endif // VERBOSE_ON

    for(int i = 0; i < textures.size(); i++)
    {
        if(textures[i] == nullptr)
            textures[i] = RetiTexture::getDefaultTexture();
        textures[i]->loadTexture();
    }

    is_loaded = true;
}

void RetiMaterial::useMaterial()
{
    if(!is_loaded)
        loadMaterial();
    for(int i = 0; i < textures.size(); i++)
        textures[i]->useTextureAt(i);
}

void RetiMaterial::unloadMaterial()
{
    if(!is_loaded)
        return;
    shader->setActiveShader();
    for(int i = 0; i < names.size(); i++)
        shader->set_uniform_int(shader->get_uniform_location(names[i]), GL_TEXTURE0);
    shader->unloadShader();
    is_loaded = false;
}

RetiShader* RetiMaterial::getShader()
{
    return shader;
}

void RetiMaterial::setTexture(const string& name, RetiTexture* tex)
{
    textures[texture_name_unit[name]] = tex;
}
