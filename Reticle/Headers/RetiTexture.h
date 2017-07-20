#ifndef RETITEXTURE_H_INCLUDED
#define RETITEXTURE_H_INCLUDED

#include <string>
#include <unordered_map>

enum RetiTextureFilterMode
{
    RETI_TEXFLTR_LINEAR,    //Default
    RETI_TEXFLTR_NEAREST
};

enum RetiMipmapFilterMode
{
    RETI_MIPFLTR_LINEAR,
    RETI_MIPFLTR_NEAREST    //Default
};

enum RetiTextureWrapMode
{
    RETI_TEXWRAP_REPEAT,    //Default
    RETI_TEXWRAP_MIRROR,
    RETI_TEXWRAP_CLAMP
};

struct RetiTextureData
{
    unsigned char* data;
    int sSize;
    int tSize;
    int nChannels;
    GLuint gpuTextureId;
    bool isLoaded;
    int nUser;
    int nLoaded;
};

class RetiTexture
{
private:

    static bool is_static_init;
    static RetiTexture* defaultTexture;

    static std::unordered_map<std::string, RetiTextureData*> path_to_data;

    RetiTextureData* texture_struct;

    //unsigned char* texture_data;
    RetiTextureFilterMode filter_mode;
    RetiMipmapFilterMode mipmap_mode;
    RetiTextureWrapMode wrap_s;
    RetiTextureWrapMode wrap_t;
    //int s_size;
    //int t_size;
    //int texture_channels;

    GLuint gpu_mag_filter;
    GLuint gpu_min_filter;
    GLuint gpu_wrap_s;
    GLuint gpu_wrap_t;

    //GLuint texture_id;
    int texture_unit;

    bool is_loaded;

    void translate_params_to_gpu();
    void load_params();
    void load_texture_from_file(std::string);

public:

    RetiTexture();
    RetiTexture(std::string);
    RetiTexture(std::string, RetiTextureFilterMode, RetiMipmapFilterMode, RetiTextureWrapMode, RetiTextureWrapMode, int);
    RetiTexture(const RetiTexture&);
    ~RetiTexture();
    RetiTexture& operator=(const RetiTexture&);

    static void initStatic();
    /*static RetiTexture* getTextureSafe(std::string path = RetiRenderer::getReticleRootDirectory() + "/Resources/Textures/default.jpg",
                                       RetiTextureFilterMode filterMode = RETI_TEXFLTR_LINEAR,
                                       RetiMipmapFilterMode mipMode = RETI_MIPFLTR_NEAREST,
                                       RetiTextureWrapMode wrapS = RETI_TEXWRAP_MIRROR,
                                       RetiTextureWrapMode wrapT = RETI_TEXWRAP_MIRROR,
                                       int texUnit = 0);*/

    void setTextureFilterMode(RetiTextureFilterMode);
    void setMipmapFilterMode(RetiMipmapFilterMode);
    void setWrapModes(RetiTextureWrapMode, RetiTextureWrapMode);
    void setBindLocation(int);

    RetiTextureFilterMode getTextureFilterMode();
    RetiMipmapFilterMode getMipmapFilterMode();
    RetiTextureWrapMode getTextureWrapModeS();
    RetiTextureWrapMode getTextureWrapModeT();
    int getBindLocation();
    int getSizeS();
    int getSizeT();

    void loadTexture();
    void unloadTexture();

    void useTexture();
    void useTextureAt(int);

    static RetiTexture* getDefaultTexture();
};

#endif // RETITEXTURE_H_INCLUDED
