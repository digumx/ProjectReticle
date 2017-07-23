#include <string>

#include <includes.h>

#include <Math/RetiTransform.h>
#include <Core/RetiLog.h>

using namespace std;

RetiTransform RetiTransform::global_parent;

RetiTransform& RetiTransform::getIdentity()
{
    RetiTransform* transf = new RetiTransform();
    return *transf;
}

RetiTransform::RetiTransform()
{
    i_children = 0;
    parent = &global_parent;
    scale_m = glm::mat4(1.0);
    rotate_m = glm::mat4(1.0);
    translate_m = glm::mat4(1.0);
    transform_m = glm::mat4(1.0);
}

RetiTransform::RetiTransform(const RetiTransform& other) : lock(), i_children(0), i_parent(0)
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

RetiTransform::~RetiTransform()
{
    lock.acquire();
    if(parent != &global_parent)
        detach();
    lock.release();
}

RetiTransform& RetiTransform::operator=(const RetiTransform& other)
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

inline void RetiTransform::reconstruct_transform()
{
    transform_m = parent->transform_m * translate_m * rotate_m * scale_m;
    lock.release();
    for(children_iterator i = children.begin(); i != children.end(); i++)
    {
        i->second->lock.acquire();
        i->second->reconstruct_transform();
    }
}

void RetiTransform::scaleTransform(float x, float y, float z)
{
    lock.acquire();
    glm::vec3 vectr(rotate_m * glm::vec4(x, y, z, 0));
    scale_m = glm::scale(scale_m, vectr);
    reconstruct_transform();
}

void RetiTransform::translateTransform(float x, float y, float z)
{
    lock.acquire();
    translate_m = glm::translate(translate_m, glm::vec3(x, y, z));
    reconstruct_transform();
}

void RetiTransform::rotateTransform(float rad, float x, float y, float z)
{
    lock.acquire();
    glm::mat4 rotm = glm::inverse(rotate_m);
    glm::vec3 vectr(rotm * glm::vec4(x, y, z, 0));
    rotate_m = glm::rotate(rotate_m, rad, vectr);
    reconstruct_transform();
}

void RetiTransform::scaleTransformLocal(float x, float y, float z)
{
    lock.acquire();
    scale_m = glm::scale(scale_m, glm::vec3(x, y, z));
    reconstruct_transform();
}

void RetiTransform::translateTransformLocal(float x, float y, float z)
{
    lock.acquire();
    glm::vec3 vectr(rotate_m * glm::vec4(x, y, z, 0));
    translate_m = glm::translate(translate_m, vectr);
    reconstruct_transform();
}

void RetiTransform::rotateTransformLocal(float rad, float x, float y, float z)
{
    lock.acquire();
    rotate_m = glm::rotate(rotate_m, rad, glm::vec3(x, y, z));
    reconstruct_transform();
}

RetiTransform& RetiTransform::getParent()
{
    return *parent;
}

void RetiTransform::setParent(RetiTransform& p)
{
    if(&p == &global_parent) return;
    parent->children.erase(i_parent);
    parent = &p;
    i_parent = p.i_children;
    p.i_children++;
    p.children[i_parent] = this;
    reconstruct_transform();
}

void RetiTransform::detach()
{
    parent->children.erase(i_parent);
    parent = &global_parent;
    reconstruct_transform();
}

glm::mat4 RetiTransform::getTransformMatrix()
{
    lock.acquire();
    glm::mat4 cpy = transform_m;
    lock.release();
    return cpy;
}
