#include <includes.h>

#include <RetiCamera.h>
#include <Math/RetiTransform.h>

using namespace std;

inline void RetiCamera::reconstruct_perspective()
{
    p_lock.acquire();
    projection_t = glm::perspective(fov_y_degrees, aspect_ratio, near_plane_view, far_plane_view);
    p_lock.release();
}

RetiCamera::RetiCamera(float fov, float aspect, float np, float fp)
            :fov_y_degrees(fov), aspect_ratio(aspect), near_plane_view(np), far_plane_view(fp)
{
    reconstruct_perspective();
}

RetiCamera::RetiCamera(const RetiCamera& other)
{
    fov_y_degrees = other.fov_y_degrees;
    aspect_ratio = other.aspect_ratio;
    near_plane_view = other.near_plane_view;
    far_plane_view = other.far_plane_view;

    cam_transf = other.cam_transf;

    reconstruct_perspective();
}

RetiCamera::~RetiCamera()
{
    p_lock.acquire();
    p_lock.release();
}

RetiCamera& RetiCamera::operator=(const RetiCamera& other)
{
    RetiCamera* ncam = new RetiCamera(other);
    return *ncam;
}

RetiTransform& RetiCamera::getTransform()
{
    return cam_transf;
}

glm::mat4 RetiCamera::getMVP(glm::mat4 m_mat)
{
    glm::mat4 v = glm::inverse(cam_transf.getTransformMatrix());
    p_lock.acquire();
    glm::mat4 mvp = projection_t * v * m_mat;
    p_lock.release();
    return mvp;
}

void RetiCamera::setAspectRatio(float ar)
{
    aspect_ratio = ar;
    reconstruct_perspective();
}

void RetiCamera::setFieldOfView(float fov)
{
    fov_y_degrees = fov;
    reconstruct_perspective();
}
void RetiCamera::setNearPlane(float npv)
{
    near_plane_view = npv;
    reconstruct_perspective();
}
void RetiCamera::setFarPlane(float fpv)
{
    far_plane_view = fpv;
    reconstruct_perspective();
}
