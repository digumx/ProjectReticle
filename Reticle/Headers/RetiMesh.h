#ifndef RETIMESH_H_INCLUDED
#define RETIMESH_H_INCLUDED

#include <includes.h>

#include <vector>

#include <RetiMaterial.h>
#include <Core/RetiSpinlock.h>
#include <Math/RetiTransform.h>

class RetiCamera;

class RetiMesh
{
private:

    // Internal OpenGL handles
    GLfloat* vertex_data;               //Treat as const after constructor call
    const GLuint vertex_data_size;
    GLuint* triangle;                   //Treat as const after constructor call
    const GLuint triangle_size;

    RetiMaterial* material;

    bool is_loaded;

    GLuint vertex_buffer_id;
    GLuint triangle_buffer_id;
    GLuint vao_id;

public:

    /** \brief Constructs a RetiMesh copying from the given vertex positions, texture coordinates,
     *          triangulation data and material.
     *
     * \param   vert_coords     const float*        An array of vertex positions in the format
     *                                              {v1.x, v1.y, v1.z, v2.x, v2.y, ...}
     * \param   tex_coords      const float*        An array of texture coordinates in the format
     *                                              {v1.u, v1.v, v2.u, v2.v, v3.u, ...}
     * \param   n_verts         const int           Number of vertices in the data passed.
     * \param   tris            const unsigned int* The triangulation array.
     * \param   n_tris          const int           Number of triangles in the triangulation array
     * \param   mat             RetiMaterial*       Pointer to the material to be used. Creates a
     *                                              new material using defaults if unspecified.
     *
     */
    RetiMesh(const float* vert_coords, const float* tex_coords, const int n_verts,
             const unsigned int* tris, const int n_tris, RetiMaterial* mat = new RetiMaterial());
    RetiMesh(const float* vertData, const int vSize, const unsigned int* tris, const int tSize, RetiMaterial* mat = new RetiMaterial());
    RetiMesh(const RetiMesh& other);
    ~RetiMesh();

/** Vertex data for this class resides in static-optimized GPU memory. To make sure
*   that rewrites to this memory are avoided, the vertex data residing in this class is
*   treated as immutable. To maintain that, copy assignment is deleted, use copy constructor instead.
*/
    RetiMesh& operator=(const RetiMesh &) = delete;

    // Initialize OpenGL
    void loadMesh();
    void unloadMesh();


    //Render with OpenGL
    void render(RetiCamera* cam, RetiTransform& transf);
};

#endif // RETIMESH_H_INCLUDED
