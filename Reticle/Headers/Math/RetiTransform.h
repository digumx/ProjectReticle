#ifndef RETITRANSFORM_H_INCLUDED
#define RETITRANSFORM_H_INCLUDED

#include <map>

#include <includes.h>

#include <Core/RetiSpinlock.h>

class RetiTransform
{
private:

    static RetiTransform global_parent;

    RetiTransform* parent;
    unsigned long int i_parent;
    std::map<unsigned long int, RetiTransform*> children;
    unsigned long int i_children;

    glm::mat4 scale_m;
    glm::mat4 rotate_m;
    glm::mat4 translate_m;
    glm::mat4 transform_m;

    RetiSpinlock lock;

    void reconstruct_transform();

public:

    static RetiTransform& getIdentity();

    RetiTransform();
    RetiTransform(const RetiTransform& other);
    ~RetiTransform();
    RetiTransform& operator=(const RetiTransform& other);

    RetiTransform& getParent();

    /// In the following functions, all axes / vectors are in global space
    void scaleTransform(float x, float y, float z);
    void rotateTransform(float rad, float x, float y, float z);
    void translateTransform(float x, float y, float z);

    /** In the following functions, all axes / vectors are locally oriented.
    *   This is distinct from being in local space, and implies that translational
    *   and scaling differences between local space and global space are ignored.
    *   Thus, if the object has been rotated by pi/2 along global Z, and scaled by
    *   a factor of 5 along all axis, calling the following translate function with
    *   (0, 1, 0) would be equivalent to moving -1 (as opposed to -5) along negative X. */
    void scaleTransformLocal(float x, float y, float z);
    void rotateTransformLocal(float rad, float x, float y, float z);
    void translateTransformLocal(float x, float y, float z);

    void setParent(RetiTransform& p);
    void detach();

    glm::mat4 getTransformMatrix();

};

typedef std::map<unsigned long int, RetiTransform*>::iterator children_iterator;
typedef std::map<unsigned long int, RetiTransform*>::const_iterator children_const_iterator;

#endif // RETITRANSFORM_H_INCLUDED
