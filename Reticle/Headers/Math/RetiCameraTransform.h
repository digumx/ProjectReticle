#ifndef RETICAMERATRANSFORM_H_INCLUDED
#define RETICAMERATRANSFORM_H_INCLUDED

#include <Math/RetiTransform.h>

class RetiCameraTransform : public RetiTransform
{
protected:

    void reconstruct_transform();

public:

    RetiCameraTransform();
    RetiCameraTransform(const RetiCameraTransform& other);
    ~RetiCameraTransform();
    RetiCameraTransform& operator=(const RetiCameraTransform& other);

    void scaleTransform(float x, float y, float z);
    void rotateTransform(float rad, float x, float y, float z);
    void translateTransform(float x, float y, float z);
};

#endif // RETICAMERATRANSFORM_H_INCLUDED
