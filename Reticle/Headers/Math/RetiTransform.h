#ifndef RETITRANSFORM_H_INCLUDED
#define RETITRANSFORM_H_INCLUDED

#include <map>

#include <includes.h>

#include <Core/RetiSpinlock.h>

class RetiTransform
{
private:

    friend class RetiCameraTransform;

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

    /** Scale, translate and rotate operations. All vector axes are in World space.
    *   Note that rotation axis in rotateTransform() is NOT normalized.
    */
    void scaleTransform(float x, float y, float z);
    void rotateTransform(float rad, float x, float y, float z);
    void translateTransform(float x, float y, float z);
    /** Same as before, except now vector axes are oriented with respect to local space.
    *   Note that this does not mean the vectors are in local space, only that the rotation
    *   component is with respect to local coordinate axes. This is basically as if only rotation
    *   has been applied to the vectors, and not translation or scaling.
    */
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
