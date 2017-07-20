#include <includes.h>

#include <Core/RetiLog.h>
#include <Math/RetiCameraTransform.h>


using namespace std;


inline void RetiCameraTransform::reconstruct_transform()
{
    #ifdef DEBUG_CODE
    RetiLog::logln("Reconstructing Camera Transform");
    #endif // DEBUG_CODE
    transform_m = parent->transform_m * rotate_m * translate_m;
    lock.release();
    for(children_iterator i = children.begin(); i != children.end(); i++)
    {
        i->second->lock.acquire();
        i->second->reconstruct_transform();
    }
}

RetiCameraTransform::RetiCameraTransform() : RetiTransform() {}

RetiCameraTransform::RetiCameraTransform(const RetiCameraTransform& other)
{
    parent = &global_parent;
    setParent(*(other.parent));
    scale_m = other.scale_m;
    rotate_m = other.rotate_m;
    translate_m = other.translate_m;
    transform_m = other.transform_m;
    for(children_const_iterator i = other.children.begin(); i != other.children.end(); i++)
    {
        RetiTransform* child = new RetiTransform(*(i->second));
        child->setParent(*this);
    }
}

RetiCameraTransform::~RetiCameraTransform() {}

RetiCameraTransform& RetiCameraTransform::operator=(const RetiCameraTransform& other)
{
    detach();
    setParent(*(other.parent));
    scale_m = other.scale_m;
    rotate_m = other.rotate_m;
    translate_m = other.translate_m;
    transform_m = other.transform_m;
    for(children_const_iterator i = other.children.begin(); i != other.children.end(); i++)
    {
        RetiTransform* child = new RetiTransform(*(i->second));
        child->setParent(*this);
    }
    return *this;
}

void RetiCameraTransform::translateTransform(float x, float y, float z)
{
    lock.acquire();
    translate_m = glm::translate(translate_m, glm::vec3(-x, -y, -z));
    reconstruct_transform();
}

void RetiCameraTransform::rotateTransform(float rad, float x, float y, float z)
{
    lock.acquire();
    rotate_m = glm::rotate(rotate_m, -rad, glm::vec3(x, y, z));
    reconstruct_transform();
}

void RetiCameraTransform::scaleTransform(float x, float y, float z)
{
    #ifdef VERBOSE_ON
    RetiLog::logln("WARNING: Scale called on RetiCameraTransform, call ignored");
    #endif // VERBOSE_ON
}

void RetiCameraTransform::scaleTransformLocal(float x, float y, float z)
{
    #ifdef VERBOSE_ON
    RetiLog::logln("WARNING: Scale called on RetiCameraTransform, call ignored");
    #endif // VERBOSE_ON
}

void RetiCameraTransform::translateTransformLocal(float x, float y, float z)
{
    lock.acquire();
    glm::mat4 rotm = glm::inverse(rotate_m);
    glm::vec3 vectr = (rotm * glm::vec4(-x, -y, -z, 0)).xyz;
    translate_m = glm::translate(translate_m, vectr);
    reconstruct_transform();
}

void RetiCameraTransform::rotateTransformLocal(float rad, float x, float y, float z)
{
    lock.acquire();
    glm::mat4 rotm = glm::inverse(rotate_m);
    glm::vec3 vectr = (rotm * glm::vec4(x, y, z, 0)).xyz;
    rotate_m = glm::rotate(rotate_m, -rad, vectr);
    reconstruct_transform();
}
