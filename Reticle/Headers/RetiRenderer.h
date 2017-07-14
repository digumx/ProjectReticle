#ifndef RETIRENDERER_H_INCLUDED
#define RETIRENDERER_H_INCLUDED

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <atomic>

#include <includes.h>

enum RetiRendererState
{
    RETI_RENDERER_STATE_INACTIVE,
    RETI_RENDERER_STATE_LOADING,
    RETI_RENDERER_STATE_PAUSED,
    RETI_RENDERER_STATE_ACTIVE
};

enum RetiKey : int
{
    RETI_KEY_A = 65,
    RETI_KEY_D = 68,
    RETI_KEY_S = 83,
    RETI_KEY_W = 87
};

class RetiShader;
class RetiMesh;
class RetiCamera;

class RetiRenderer
{
private:

    static constexpr const char* version = "0.0.7a-linux";
    static std::string retiRoot;
    static int nInstances;                  //TODO: Make Atomic
    static std::chrono::duration<long, std::milli> pause;
    static bool is_glfw_init;                  //TODO: Make Atomic

    bool detach_renderer;
    bool sticky_keys;

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

    GLfloat clear_color_r;              // TODO: Spinlock
    GLfloat clear_color_g;
    GLfloat clear_color_b;

    RetiShader* common_shader;
    std::vector<RetiMesh*> meshes;

    void constr_init_flags();
    void init_renderer();
    void init_glfw();
    void init_glew();
    void create_window();
    void init_shader();
    void unload_shader();
    void init_meshes();
    void unload_meshes();
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

    /** Setting this to true forces the GPU thread to run on the thread that calls
    *   startRenderer().
    *
    *   This will make the startRenderer() function call non-latent, and will return only when
    *   rendering is stopped by closing the windows.
    */
    void setDetachRenderer(bool detach);

    /** This will turn on sticky keys for key press calls.
    *
    *   Consider this situation: Your code, somehow, begins to poll key calls every 2s (which
    *   is a very long time for key polling), and the user manages to press and release a key in
    *   those three seconds. Normally, the keypress would have gone unregistered under this
    *   situation. However, with sticky keys on, the next poll will catch the keypress. Note that
    *   if multiple keypresses are generated, this will only catch a single press. Also that repolls
    *   immediately after the sticky polls will not re-catch the same keypress. In general, if you
    *   are writing a real-time and/or an interactive application, polling keypresses quickly is
    *   probably a better solution than this.
    */
    void setStickyKeys(bool sticky);

    /** Returns if a is pressed.
    */
    bool getKey(RetiKey key);

    void setWindowTitle(std::string str);
    void setWindowSize(int x, int y);
    void setClearColor(GLfloat R, GLfloat G, GLfloat B);

    void useCamera(RetiCamera* n_cam);

    void addMesh(RetiMesh* mesh);
    void addMesh(const float* vertCoords, const float* texCoords, const int n_verts,
                 const unsigned int* tris, const int n_tris);

    void startRenderer();
    void stopRenderer();
    void pauseRenderer();
    void resumeRenderer();
};

#endif // RETIRENDERER_H_INCLUDED
