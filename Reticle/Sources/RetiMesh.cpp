#include <includes.h>

#include <iostream>


#include <RetiMesh.h>
#include <RetiRenderer.h>
#include <RetiCamera.h>
#include <RetiShader.h>
#include <Core/RetiLog.h>
#include <Core/RetiSpinlock.h>
#include <Math/RetiTransform.h>


using namespace std;

RetiMesh::RetiMesh(const float* vert_coords, const float* tex_coords, const int n_verts,
                   const unsigned int* tris, const int n_tris, RetiMaterial* mat)
                   : vertex_data_size(n_verts * 5) , triangle_size(n_tris * 3)
{
    material = mat;

    vertex_data = new GLfloat[vertex_data_size];
    for(int i = 0; i < n_verts; i++)
    {
        vertex_data[5 * i] = vert_coords[3 * i];
        vertex_data[5 * i + 1] = vert_coords[3 * i + 1];
        vertex_data[5 * i + 2] = vert_coords[3 * i + 2];
        vertex_data[5 * i + 3] = tex_coords[2 * i];
        vertex_data[5 * i + 4] = tex_coords[2 * i + 1];
    }

    triangle = new GLuint[triangle_size];
    for(int i = 0; i < triangle_size; i++)
        triangle[i] = tris[i];

    is_loaded = false;
}

RetiMesh::RetiMesh(const float* vertData, const int vSize, const unsigned int* tris, const int tSize,
                        RetiMaterial* mat)
                        : vertex_data_size(vSize) , triangle_size(tSize)
{
    material = mat;

    vertex_data = new GLfloat[vertex_data_size];
    for(int i = 0; i < vertex_data_size; i++)
        vertex_data[i] = vertData[i];

    triangle = new GLuint[triangle_size];
    for(int i = 0; i < triangle_size; i++)
        triangle[i] = tris[i];

    is_loaded = false;
}

RetiMesh::RetiMesh(const RetiMesh& other)
            : RetiMesh(other.vertex_data, other.vertex_data_size, other.triangle, other.triangle_size) {}

RetiMesh::~RetiMesh()
{
    if(is_loaded)
        RetiLog::logln("WARNING: Deleting Mesh before unloading data from gpu!");
    delete[] vertex_data;
    delete[] triangle;
}

/** Vertex data for this class resides in static-optimized GPU memory. To make sure
*   that rewrites to this memory are avoided, the vertex data residing in this class is
*   treated as immutable. To maintain that, copy assignment is deleted, use copy constructor instead.
*/
/*RetiMesh& RetiMesh::operator=(const RetiMesh& other)
{
    vertex_data_size = other.vertex_data_size;
    triangle_size = other.triangle_size;

    material = mat;

    vertex_data = new GLfloat[vertex_data_size];
    for(int i = 0; i < vertex_data_size; i++)
        vertex_data[i] = other.vertex_data[i];

    triangle = new GLuint[triangle_size];
    for(int i = 0; i < triangle_size; i++)
        triangle[i] = other.triangle[i];

    is_loaded = false;

    return *this;
}*/

void RetiMesh::loadMesh()
{
    if(is_loaded)
        return;

    glGenVertexArrays(1, &vao_id);
    glGenBuffers(1, &vertex_buffer_id);
    glGenBuffers(1, &triangle_buffer_id);

    #ifdef DEBUG_CODE
    GLint maxVerts, maxInds;
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &maxVerts);
    glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &maxInds);
    RetiLog::logln("Max Verts: " + to_string(maxVerts) + " Max Indices: " + to_string(maxInds));
    #endif // DEBUG_CODE

    glBindVertexArray(vao_id);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
    #ifdef DEBUG_CODE
    RetiLog::logln("Vertex data buffer size: " + to_string(vertex_data_size));
    #endif
    glBufferData(GL_ARRAY_BUFFER, vertex_data_size * sizeof(GLfloat), vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer_id);
    #ifdef DEBUG_CODE
    RetiLog::logln("Triangle buffer size: " + to_string(triangle_size));
    #endif // DEBUG_CODE
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangle_size * sizeof(GLuint), triangle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    material->loadMaterial();

    is_loaded = true;
}

void RetiMesh::unloadMesh()
{
    glBindVertexArray(0);
    glDeleteBuffers(1, &vertex_buffer_id);
    glDeleteBuffers(1, &triangle_buffer_id);
    glDeleteVertexArrays(1, &vao_id);
    vertex_buffer_id = 0;
    triangle_buffer_id = 0;
    vao_id = 0;

    material->unloadMaterial();

    is_loaded = false;
}

void RetiMesh::render(RetiCamera* cam, RetiTransform& transf)
{
    if(!is_loaded)
        loadMesh();

    material->useMaterial();
    material->getShader()->setMVP(cam->getMVP(transf.getTransformMatrix()));

    glBindVertexArray(vao_id);
    glDrawElements(GL_TRIANGLES, triangle_size, GL_UNSIGNED_INT, 0);
}
