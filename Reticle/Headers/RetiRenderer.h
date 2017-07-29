#ifndef RETIRENDERER_H_INCLUDED
#define RETIRENDERER_H_INCLUDED

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <atomic>

#include <Core/RetiSpinlock.h>
#include <RetiColor.h>

#include <includes.h>

enum RetiRendererState
{
    RETI_RENDERER_STATE_INACTIVE,
    RETI_RENDERER_STATE_LOADING,
    RETI_RENDERER_STATE_PAUSED,
    RETI_RENDERER_STATE_ACTIVE
};

class RetiShader;
class RetiMesh;
class RetiCamera;
class RetiSceneObject;
class RetiKeyboard;

class RetiRenderer
{
private:

    static constexpr const char* version = "0.0.7a-linux";
    static std::string retiRoot;
    static int nInstances;                  //TODO: Make Atomic
    static std::chrono::duration<long, std::milli> pause;
    static bool is_glfw_init;                  //TODO: Make Atomic

    RetiKeyboard* keyb;

    bool detach_renderer;
    RetiRendererState renderer_state;
    std::atomic<bool> is_renderer_paused;
    std::atomic<bool> do_breakout;
    std::thread* gpu_thread;

    int win_x;                          // TODO: Make Atomic
    int win_y;                          // TODO: Make Atomic
    std::string title;                  //TODO: Switch to atomic string
    GLFWwindow* window;
    bool is_window_init;                //TODO: Decide Atomic
    bool is_glew_init;                  //TODO: Decide Atomic

    RetiCamera* cam;
    bool is_cam_user;

    RetiSpinlock clear_color_lock;
    RetiColor clear_color;
    st::atomic<bool> clear_color_updated;

    RetiShader* common_shader;
    std::vector<RetiSceneObject*> objects;

    void constr_init_flags();
    void init_renderer();
    void init_glfw();
    void init_glew();
    void create_window();
    void init_shader();
    void unload_shader();
    void init_objects();
    void unload_objects();
    void render_internal();
    void gput_post_renderer_cleanup();

public:

    RetiRenderer();
    RetiRenderer(bool detach);
    RetiRenderer(const RetiRenderer& other);
    ~RetiRenderer();
    RetiRenderer& operator=(const RetiRenderer& other);

    static void setReticleRootDirectory(const std::string& path);
    static std::string getReticleRootDirectory();
    static void init();

    const char* getFullVersionString();

    RetiRendererState getRedererState() const;

    void setDetachRenderer(bool detach);
    void setWindowSize(int x, int y);
    void setWindowTitle(const std::string& str);
    void setClearColor(const RetiColor& col);

    void useCamera(RetiCamera* n_cam);

    void addSceneObject(RetiSceneObject* mesh);
    /*void addMesh(const float* vertCoords, const float* texCoords, const int n_verts,
                 const unsigned int* tris, const int n_tris);*/

    RetiKeyboard& getKeyboard();
    RetiCamera& getCamera();

    void startRenderer();
    void stopRenderer();
    void pauseRenderer();
    void resumeRenderer();
};

#endif // RETIRENDERER_H_INCLUDED
