#ifndef RETITRANSFORM_H_INCLUDED
#define RETITRANSFORM_H_INCLUDED

#include <includes.h>

#include <Core/RetiSpinlock.h>

class RetiTransform
{
private:

    glm::mat4 scale_m;
    glm::mat4 rotate_m;
    glm::mat4 translate_m;
    glm::mat4 transform_m;

    RetiSpinlock lock;

    void reconstruct_transform();

public:

    RetiTransform(){}
    RetiTransform(const RetiTransform& other);
    ~RetiTransform();
    RetiTransform& operator=(const RetiTransform& other);

    void scaleTransform(float x, float y, float z);
    void rotateTransform(float rad, float x, float y, float z);
    void translateTransform(float x, float y, float z);

    glm::mat4 getTransformMatrix();

};

#endif // RETITRANSFORM_H_INCLUDED
