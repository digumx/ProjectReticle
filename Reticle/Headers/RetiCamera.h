#ifndef RETICAMERA_H_INCLUDED
#define RETICAMERA_H_INCLUDED

#include <includes.h>

#include <Core/RetiSpinlock.h>
#include <Math/RetiTransform.h>

class RetiCamera
{
private:

    RetiTransform cam_transf;

    float aspect_ratio;
    float near_plane_view;
    float far_plane_view;
    float fov_y_degrees;

    glm::mat4 projection_t;
    RetiSpinlock p_lock;

    void reconstruct_perspective();

public:

    RetiCamera(float fov = 45, float aspect = (16.0 / 9.0), float np = 0.1, float fp = 1000.0);

    RetiCamera(const RetiCamera& other);
    ~RetiCamera();
    RetiCamera& operator=(const RetiCamera& other);

    RetiTransform& getTransform();

    void setNearPlane(float npv);
    void setFarPlane(float fpv);
    void setAspectRatio(float ar);
    void setFieldOfView(float fov);

    float getNearPlane() {return near_plane_view;}
    float getFarPlane() {return far_plane_view;}
    float getAspectRatio() {return aspect_ratio;}
    float getFieldOfView() {return fov_y_degrees;}

    glm::mat4 getMVP(glm::mat4 m_mat);
};

#endif // RETICAMERA_H_INCLUDED
