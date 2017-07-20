
#include <RetiKeyboard.h>
#include <Core/RetiLog.h>

using namespace std;

RetiKeyboard::RetiKeyboard() : listened_key_pressed(), map_lock() {}

RetiKeyboard::RetiKeyboard(const RetiKeyboard& other) : map_lock()
{
    for(map<int, bool>::const_iterator i = other.listened_key_pressed.begin();
        i != other.listened_key_pressed.end(); i++)
            this->listened_key_pressed[i->first] = i->second;
}

RetiKeyboard::~RetiKeyboard()
{
    map_lock.acquire();
    for(map<int, bool>::iterator i = listened_key_pressed.begin();
        i != listened_key_pressed.end(); i++)
            listened_key_pressed[i->first] = false;
    listened_key_pressed.clear();
    map_lock.release();
}

RetiKeyboard& RetiKeyboard::operator=(const RetiKeyboard& other)
{
    map_lock.acquire();
    listened_key_pressed.clear();
    for(map<int, bool>::const_iterator i = other.listened_key_pressed.begin();
        i != other.listened_key_pressed.end(); i++)
            this->listened_key_pressed[i->first] = i->second;
    map_lock.release();
    return *this;
}

void RetiKeyboard::_glfw_key_callback(GLFWwindow* window, int glfw_code, int scan_code, int action, int mods)
{
    #ifdef DEBUG_CODE
    RetiLog::logln("Key Event: " + to_string(glfw_code) + (action == GLFW_RELEASE ? " Release" : " No-Release"));
    #endif // DEBUG_CODE

    map_lock.acquire();
    for(map<int, bool>::iterator i = listened_key_pressed.begin();
        i != listened_key_pressed.end(); i++)
        {
            #ifdef DEBUG_CODE
            RetiLog::logln("Key State: " + to_string(i->first) + " " + to_string(listened_key_pressed[i->first]));
            #endif // DEBUG_CODE
            if(glfw_code == i->first)
            {
                listened_key_pressed[i->first] = action == GLFW_RELEASE ? false : true;
                #ifndef DEBUG_CODE
                break;
                #endif // DEBUG_CODE
            }
        }
    map_lock.release();
}

void RetiKeyboard::addKeyListener(RetiKey key)
{
    map_lock.acquire();
    listened_key_pressed[key] = false;
    map_lock.release();
}

bool RetiKeyboard::isKeyPressed(RetiKey key)
{
    map_lock.acquire();
    bool ret = listened_key_pressed[key];
    map_lock.release();
    return ret;
}

void RetiKeyboard::removeListener(RetiKey key)
{
    map_lock.acquire();
    listened_key_pressed.erase(key);
    map_lock.release();
}
