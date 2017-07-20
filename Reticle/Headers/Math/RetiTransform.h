#ifndef RETITRANSFORM_H_INCLUDED
#define RETITRANSFORM_H_INCLUDED

#include <map>

#include <includes.h>

#include <Core/RetiSpinlock.h>

class RetiTransform
{
protected:

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

    void scaleTransform(float x, float y, float z);
    void rotateTransform(float rad, float x, float y, float z);
    void translateTransform(float x, float y, float z);

    void setParent(RetiTransform& p);
    void detach();

    glm::mat4 getTransformMatrix();

};

typedef std::map<unsigned long int, RetiTransform*>::iterator children_iterator;
typedef std::map<unsigned long int, RetiTransform*>::const_iterator children_const_iterator;

#endif // RETITRANSFORM_H_INCLUDED
