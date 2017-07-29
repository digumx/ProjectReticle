#include <includes.h>

#include <iostream>
#include <ctime>
#include <chrono>
#include <cstring>
#include <fstream>
#include <functional>
#ifdef PLATFORM_LINUX
#include <pthread.h>
#endif // PLATFORM_LINUX

#include <RetiRenderer.h>
#include <RetiShader.h>
#include <RetiMesh.h>
#include <RetiTexture.h>
#include <RetiCamera.h>
#include <RetiKeyboard.h>
#include <RetiSceneObject.h>
#include <Core/RetiLog.h>

using namespace std;

string RetiRenderer::retiRoot;
int RetiRenderer::nInstances;
std::chrono::duration<long, std::milli> RetiRenderer::pause(1);
bool RetiRenderer::is_glfw_init = false;

RetiKeyboard* _callback_keyb;

void _err_callback(int err, const char* desc)
{
    RetiLog::logln("GLFW ERROR: " + string(desc));
}

void _key_callback(GLFWwindow* window, int glfw_code, int scan_code, int action, int mods)
{
    _callback_keyb->_glfw_key_callback(window, glfw_code, scan_code, action, mods);
}

RetiRenderer::RetiRenderer() : clear_color()
{
    /// Set flags
    renderer_state = RETI_RENDERER_STATE_INACTIVE;
    win_x = 800;
    win_y = 600;
    title = "Reticle";
    detach_renderer = true;
    constr_init_flags();

    /// Construct and set up members
    keyb = new RetiKeyboard();
    common_shader = new RetiShader();
    cam = new RetiCamera(45, (win_x * 1.0) / win_y, 0.1, 1000.0);
    is_cam_user = false;

    cam->getTransform().translateTransform(0.0, 0.0, -1.0);

    nInstances++;
}

RetiRenderer::RetiRenderer(bool detach) clear_color()
{
    /// Set Flags
    renderer_state = RETI_RENDERER_STATE_INACTIVE;
    win_x = 800;
    win_y = 600;
    title = "Reticle";
    detach_renderer = detach;
    constr_init_flags();

    /// Construct and set up members
    keyb = new RetiKeyboard();
    common_shader = new RetiShader();
    cam = new RetiCamera(45, (win_x * 1.0) / win_y, 0.1, 1000.0);
    is_cam_user = false;

    cam->getTransform().translateTransform(0.0, 0.0, -1.0);

    nInstances++;
}

RetiRenderer::RetiRenderer(const RetiRenderer& other) clear_color(other.clear_color)
{
    renderer_state = RETI_RENDERER_STATE_INACTIVE;

    if(other.renderer_state != RETI_RENDERER_STATE_INACTIVE)
        RetiLog::logln("WARNING: Copy constructing from non-inactive renderer can cause undefined behavior.");

    /// Copy flags
    win_x = other.win_x;
    win_y = other.win_y;
    title = other.title;
    detach_renderer = other.detach_renderer;
    constr_init_flags();

    /// Copy-construct and set up members
    keyb = new RetiKeyboard(*(other.keyb));
    common_shader = new RetiShader(*(other.common_shader));
    cam = new RetiCamera(*(other.cam));
    is_cam_user = false;

    cam->getTransform().translateTransform(0.0, 0.0, -1.0);

    objects.reserve(other.objects.size());

    for(int i = 0; i < other.objects.size(); i++)
        objects.push_back(other.objects[i]);

    nInstances++;
}

void RetiRenderer::constr_init_flags()
{
    is_renderer_paused = false;
    do_breakout = false;
    is_glew_init = false;
    is_window_init = false;
    clear_color_updated = false;
}

RetiRenderer::~RetiRenderer()
{
    delete common_shader;
    delete keyb;

    if(!is_cam_user)
        delete cam;

    /// Nope. This is ReticleUser's responsibility.
    /*
    for(int i = 0; i < objects.size(); i++)
        delete objects[i];
    */

    objects.clear();

    if(nInstances <= 1)
    {
        glfwTerminate();
        is_glfw_init = false;
    }

    nInstances--;
}

RetiRenderer& RetiRenderer::operator=(const RetiRenderer& other)
{
    if(this->renderer_state != RETI_RENDERER_STATE_INACTIVE ||
       other.renderer_state != RETI_RENDERER_STATE_INACTIVE)
        RetiLog::logln("WARNING: Copy constructing from or to non-inactive renderer can cause undefined behavior.");

    renderer_state = RETI_RENDERER_STATE_INACTIVE;

    //Copy flags
    win_x = other.win_x;
    win_y = other.win_y;
    title = other.title;
    clear_color_r = other.clear_color_r;
    clear_color_g = other.clear_color_g;
    clear_color_b = other.clear_color_b;
    detach_renderer = other.detach_renderer;
    constr_init_flags();

    //Delete existing members
    delete keyb;
    delete common_shader;
    delete cam;

    // Copy-construct to set up new members.
    keyb = new RetiKeyboard(*(other.keyb));
    common_shader = new RetiShader(*(other.common_shader));
    cam = new RetiCamera(*(other.cam));
    is_cam_user = false;
    cam->getTransform().translateTransform(0.0, 0.0, -1.0);

    objects.clear();

    objects.reserve(other.objects.size());

    for(int i = 0; i < other.objects.size(); i++)
        objects.push_back(other.objects[i]);

    return *this;
}

const char* RetiRenderer::getFullVersionString()
{
    return version;
}

RetiRendererState RetiRenderer::getRedererState() const
{
    RetiRendererState cpy(renderer_state);
    return cpy;
}

void RetiRenderer::setDetachRenderer(bool detach)
{
    if(renderer_state == RETI_RENDERER_STATE_INACTIVE)
        detach_renderer = detach;
    else
        RetiLog::logln("WARNING: Cannot set detach renderer on non inactive Renderer, call ignored.");
}

void RetiRenderer::setReticleRootDirectory(const string& path)
{
    retiRoot = path;
}

string RetiRenderer::getReticleRootDirectory()
{
    return retiRoot;
}

void RetiRenderer::init()
{
    RetiLog::initLog();

    RetiTexture::initStatic();
}

void RetiRenderer::setWindowSize(int x, int y)
{
    if(renderer_state != RETI_RENDERER_STATE_INACTIVE)
    {
        RetiLog::logln("WARNING: Cannot set window size on non-inactive renderer, call ignored.");
        return;
    }
    RetiRenderer::win_x = x;
    RetiRenderer::win_y = y;
}

void RetiRenderer::setWindowTitle(const string& str)
{
    if(renderer_state != RETI_RENDERER_STATE_INACTIVE)
    {
        RetiLog::logln("WARNING: Cannot set window title on non-inactive renderer, call ignored.");
        return;
    }
    title = str;
}

void RetiRenderer::setClearColor(const RetiColor& col)
{
    /*if(renderer_state != RETI_RENDERER_STATE_INACTIVE)
    {
        RetiLog::logln("WARNING: Cannot set clear clear_color on non-inactive renderer, call ignored.");
        return;
    }*/
    /// From now on, you can change clear color while rendering.
    clear_color_lock.acquire();
    clear_color = col;
    clear_color_updated = true;
    clear_color_lock.release();
}

void RetiRenderer::init_glfw()
{
    if(is_glfw_init)
        return;
    RetiLog::logln("Initializing GLFW.");
    glfwInit();
    glfwSetErrorCallback(_err_callback);


    is_glfw_init = true;

    RetiLog::logln("GLFW initialization complete.");
}

void RetiRenderer::create_window()
{
    if(is_window_init)
        return;
    RetiLog::logln("Creating window");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, 0);
    window = glfwCreateWindow(win_x, win_y, title.c_str(), nullptr, nullptr);
    if(window==nullptr)
        RetiLog::logln("FAyILED TO CREATE WINDOW " + to_string(win_x) + " " + to_string(win_y));
    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(0);
    _callback_keyb = keyb;
    glfwSetKeyCallback(this->window, _key_callback);
    is_window_init = true;
    RetiLog::logln("Created window");
}

void RetiRenderer::init_glew()
{
    if(is_glew_init)
        return;
    RetiLog::logln("Initializing GLEW.");
    glewExperimental = GL_TRUE;
    int res = glewInit();
    if(res != GLEW_OK)
    {
        RetiLog::logln(string((const char*) glewGetErrorString(res)));
    }

    int w, h;
    glfwGetFramebufferSize(this->window, &w, &h);
    glViewport(0, 0, w, h);

    is_glew_init = true;

    RetiLog::logln("GLEW initialization complete.");
}

void RetiRenderer::init_renderer()
{
    this->init_glfw();
    this->create_window();
    this->init_glew();
    this->init_shader();
    this->init_objects();

    clear_color_lock.acquire();
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    clear_color_updated = true;
    clear_color_lock.release();

    glClearColor(clear_color_r, clear_color_g, clear_color_b, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void RetiRenderer::init_shader()
{
    common_shader->loadShader();
}

void RetiRenderer::unload_shader()
{
    common_shader->unloadShader();
}

void RetiRenderer::init_objects()
{
    for(int i = 0; i < objects.size(); i++)
        objects[i]->loadObject();
}

void RetiRenderer::unload_objects()
{
    for(int i = 0; i < objects.size(); i++)
        objects[i]->unloadObject();
}

void RetiRenderer::render_internal()
{
    renderer_state = RETI_RENDERER_STATE_LOADING;
    this->init_renderer();

    renderer_state = RETI_RENDERER_STATE_ACTIVE;
    while(!glfwWindowShouldClose(this->window))
    {
        /// Check for pause and break at top.
        if(is_renderer_paused)
        {
            this_thread::sleep_for(pause);
            continue;
        }

        if(do_breakout)
        {
            RetiLog::logln("Breaking Out.");
            do_breakout = false;
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        /// Poll GLFW-based input
        glfwPollEvents();

        ///Update Clear color
        if(!clear_color_updated)
        {
            clear_color_lock.acquire();
            glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
            clear_color_updated = true;
            clear_color_lock.release();
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        common_shader->setActiveShader();
        for(int i = 0; i < objects.size(); i++)
            objects[i]->render(cam);

        glfwSwapBuffers(RetiRenderer::window);
    }
    gput_post_renderer_cleanup();
}

void RetiRenderer::gput_post_renderer_cleanup()
{
    unload_objects();
    unload_shader();
    //delete window;
    glfwDestroyWindow(window);

    #ifdef VERBOSE_ON
    RetiLog::logln("GPUT Cleanup complete.");
    #endif // VERBOSE_ON
}

void RetiRenderer::addSceneObject(RetiSceneObject* obj)
{
    objects.push_back(obj);
}

/*void RetiRenderer::addMesh(const float* vertCoords, const float* texCoords, const int n_verts,
                           const unsigned int* tris, const int n_tris)
{
    RetiMesh* mesh = new RetiMesh(vertCoords, texCoords, n_verts, tris, n_tris);
    meshes.push_back(mesh);
}*/

void RetiRenderer::startRenderer()
{
    if(detach_renderer)
        gpu_thread = new thread(&RetiRenderer::render_internal, this);
    else
        render_internal();
}

void RetiRenderer::stopRenderer()
{
    RetiLog::logln("Stopping Renderer");
    do_breakout = true;
    gpu_thread->join();
    delete gpu_thread;
    is_window_init = false;
    is_glew_init = false;
    RetiLog::logln("Renderer stopped");
}

void RetiRenderer::pauseRenderer()
{
    is_renderer_paused = true;
}

void RetiRenderer::resumeRenderer()
{
    is_renderer_paused = false;
}

void RetiRenderer::useCamera(RetiCamera* n_cam)
{
    if(!is_cam_user)
        delete cam;
    cam = n_cam;
    is_cam_user = true;
}

RetiKeyboard& RetiRenderer::getKeyboard()
{
    return *keyb;
}

RetiCamera& RetiRenderer::getCamera()
{
    return *cam;
}
