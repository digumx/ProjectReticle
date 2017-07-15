#ifndef RETIKEYBOARD_H_INCLUDED
#define RETIKEYBOARD_H_INCLUDED

#include <map>

#include <includes.h>

#include <Core/RetiSpinlock.h>

/** Enumeration of all platform independent Keys with their GLFW
*   codes.
*/
enum RetiKey : int
{
    A       = 65,
    D       = 68,
    S       = 83,
    W       = 87,
    ESCAPE  = 256,
    RIGHT   = 262,
    LEFT    = 263,
    DOWN    = 264,
    UP      = 265
};


/** Manages key states and keyboard inputs in a thread safe way
*/
class RetiKeyboard
{
    friend class RetiRenderer;

private:

    std::map<int, bool> listened_key_pressed;
    RetiSpinlock map_lock;

public:

    RetiKeyboard();
    RetiKeyboard(const RetiKeyboard& other);
    ~RetiKeyboard();
    RetiKeyboard& operator=(const RetiKeyboard& other);

    /** Add a new key listener for the passed key. If the listener is already added, this function
    *   resets it's state
    */
    void addKeyListener(RetiKey key);

    /** Returns true if the key passed is in the pressed state, false otherwise. Adds a listener and
    *   misses all events previous to the function call if key passed was not being listened for.
    */
    bool isKeyPressed(RetiKey key);

    /** Removes the listener for the passed key. Does nothing if the listener for the passed key was never
    *   added
    */
    void removeListener(RetiKey key);

    /// INTERNAL - DO NOT CALL
    void _glfw_key_callback(GLFWwindow* window, int glfw_code, int scan_code, int action, int mods);
};

#endif // RETIKEYBOARD_H_INCLUDED
