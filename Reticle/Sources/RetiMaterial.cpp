#include <includes.h>

#include <fstream>
#include <map>
#include <cctype>

#include <RetiMaterial.h>
#include <RetiTexture.h>
#include <RetiShader.h>
#include <RetiRenderer.h>
#include <Core/RetiLog.h>

using namespace std;

unordered_map<string, RetiShader*> RetiMaterial::path_to_shader;

/// These are some utility functions unrelated to the RetiMaterial, but related to .shif files.

void load_file_to_string(string& str, ifstream& file)
{
    str = "";
    string ln;
    if(!file.is_open()) return;
    getline(file, ln);
    while(getline(file, ln)) str = str + ln;
    return;
}

string substr_by_char(char bch, char ech, size_t& start, const string& str)
{
    size_t bpos;
    if((bpos = str.find_first_of(bch, start)) == string::npos)
    {
        RetiLog::logln("WARNING: Expected " + string(1, bch) + "After" + to_string(start));
        return "";
    }
    size_t epos;
    if((epos = str.find_first_of(ech, bpos)) == string::npos)
    {
        RetiLog::logln("WARNING: Unmatched " + string(1, bch) + "At" + string(1, bpos));
        return "";
    }
    start = epos;
    return str.substr(bpos+1, epos-bpos-1);
}

void break_list(const string& data, char ch, vector<string>& out)
{
    #ifdef DEBUG_CODE
    RetiLog::logln("Breaking string " + data);
    #endif // DEBUG_CODE
    size_t pos = 0;
    size_t prev = 0;
    while((pos = data.find_first_of(ch, prev)) != string::npos)
    {
        out.push_back(data.substr(prev, pos-prev));
        prev = pos + 1;
    }
}

string trim_string(const string& str, const string& charset)
{
    size_t bg = str.find_first_not_of(charset);
    size_t nd = str.find_last_not_of(charset);
    return str.substr(bg, nd-bg+1);
}

void load_shif_sections(const string& fdata, map<string, string>& secs)
{
    size_t pos = 0;
    while((pos = fdata.find_first_of('(', pos)) != string::npos)
    {
        string key = trim_string(substr_by_char('(', ')', pos, fdata), RETI_SHIFSYNTX_WHITECHARS);
        string val = trim_string(substr_by_char('{', '}', pos, fdata), RETI_SHIFSYNTX_WHITECHARS);
        secs[key] = val;
        pos++;
    }
}

void load_keyval_section(const string& data, map<string, string>& keyval)
{
    vector<string> keyvalVectr;
    break_list(data, ';', keyvalVectr);
    size_t p;
    for(unsigned int i = 0; i < keyvalVectr.size(); i++)
    {
        if((p = keyvalVectr[i].find_first_of('=')) == string::npos)
            RetiLog::logln("WARNING: Malformed line: " + keyvalVectr[i]);
        string key = trim_string(keyvalVectr[i].substr(0, p), RETI_SHIFSYNTX_WHITECHARS);
        string val = trim_string(keyvalVectr[i].substr(p+1), RETI_SHIFSYNTX_WHITECHARS);
        keyval[key] = val;
    }
}

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

RetiMaterial::RetiMaterial() : RetiMaterial(RetiRenderer::getReticleRootDirectory() + RETI_DEFAULT_SHIF) {}

RetiMaterial::RetiMaterial(const string& filePath)
{
    #ifdef VERBOSE_ON
    RetiLog::logln("Loading shif: " + filePath);
    #endif // VERBOSE_ON

    string dir = filePath.substr(0, filePath.find_last_of({'/', '\\'}) + 1);

    ifstream matFile(filePath.c_str());
    if(!matFile.is_open())
        RetiLog::logln("WARNING: Falied to open file: " + filePath + ", Segfault imminent.");

    string fdata;
    load_file_to_string(fdata, matFile);
    map<string, string> name_to_data;
    load_shif_sections(fdata, name_to_data);

    /// Load texnames
    vector<string> texNames;
    break_list(name_to_data[RETI_SHIFSYNTX_TEXNAMES], ';', texNames);
    #ifdef DEBUG_CODE
    RetiLog::logln("Number of textures: " + to_string(texNames.size()));
    #endif // DEBUG_CODE
    for(int i = 0; i < texNames.size(); i++)
        texNames[i] = trim_string(texNames[i], "\"");

    const string& cdata = name_to_data[RETI_SHIFSYNTX_CODE];
    map<string, string> codeKeyval;
    load_keyval_section(cdata, codeKeyval);
    string vCode = trim_string(codeKeyval[RETI_SHIFSYNTX_VERTEXCODE], "\"");
    string fCode = trim_string(codeKeyval[RETI_SHIFSYNTX_FRAGMENTCODE], "\"");

    if(vCode == "")
        RetiLog::logln("WARNING: Vertex code reference is empty!");
    if(fCode == "")
        RetiLog::logln("WARNING: Fragment code reference is empty!");

    initialize_from_paths(dir + vCode, dir + fCode, texNames);

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
        RetiLog::logln("WARNING: Delete called on loaded material, attempting to unload.");
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
