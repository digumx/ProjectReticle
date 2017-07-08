#include <includes.h>

#include <Math/RetiTransform.h>

using namespace std;

RetiTransform::RetiTransform(const RetiTransform& other) : lock()
{
    scale_m = other.scale_m;
    rotate_m = other.rotate_m;
    translate_m = other.translate_m;
    transform_m = other.transform_m;
}

RetiTransform::~RetiTransform()
{
    lock.acquire();
    lock.release();
}

RetiTransform& RetiTransform::operator=(const RetiTransform& other)
{
    RetiTransform* nt = new RetiTransform(other);
    return *nt;
}

inline void RetiTransform::reconstruct_transform()
{
    transform_m = translate_m * rotate_m * scale_m;
}

void RetiTransform::scaleTransform(float x, float y, float z)
{
    scale_m = glm::scale(scale_m, glm::vec3(x, y, z));
    reconstruct_transform();
}

void RetiTransform::translateTransform(float x, float y, float z)
{
    translate_m = glm::translate(translate_m, glm::vec3(x, y, z));
    reconstruct_transform();
}


void RetiTransform::rotateTransform(float rad, float x, float y, float z)
{
    rotate_m = glm::rotate(rotate_m, rad, glm::vec3(x, y, z));
    reconstruct_transform();
}

glm::mat4 RetiTransform::getTransformMatrix()
{
    lock.acquire();
    glm::mat4 cpy = transform_m;
    lock.release();
    return cpy;
}
