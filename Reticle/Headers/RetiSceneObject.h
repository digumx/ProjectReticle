#ifndef RETISCENEOBJECT_H_INCLUDED
#define RETISCENEOBJECT_H_INCLUDED

#include <vector>
#include <string>

#include <includes.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Math/RetiTransform.h>

class RetiMesh;
class RetiCamera;
class RetiMaterial;

struct RetiMeshHolder
{
    RetiTransform transf;
    RetiMesh* mesh;
    RetiSpinlock lock;
    bool is_cpy;

    /// This needs to be explicitly specified because we are defining the copy constructor.
    RetiMeshHolder() : transf(), lock(), is_cpy(false) {}

    /// This is necessary because we are using a vector<RetiMeshHolder>, which copy constructs when we call push_back.
    /// Other constructors are unnecessary as all functions for this struct is handled C-style.
    RetiMeshHolder(const RetiMeshHolder& other) : lock()
    {
        transf = other.transf;
        mesh = other.mesh;
        /// is_cpy talks about copies in C-style code. This implies that explicit calls to this
        /// should be avoided, and implicit calls performed with objects in automatic memory.
        is_cpy = false;
    }

    /// Again, this is needed to put RetiMeshHolder into collections properly.
    RetiMeshHolder& operator=(const RetiMeshHolder& other)
    {
        transf = other.transf;
        mesh = other.mesh;
        is_cpy = false;
        return *this;
    }
};

class RetiSceneObject
{
private:

    std::vector<RetiMeshHolder> meshes;
    RetiTransform root_transf;

    bool is_loaded;

    void process_node(aiNode* node, const aiScene* scene, RetiTransform& parent,
                      std::vector<RetiMaterial*>& mats, std::vector<bool>& used_mats);
    void process_mesh(aiMesh* mesh, const aiScene* scene, RetiTransform& parnet,
                      std::vector<RetiMaterial*>& mats, std::vector<bool>& used_mats);
    void process_materials(const aiScene* scene, const std::string& dir,
                           std::vector<RetiMaterial*>& mats, std::vector<bool>& used_mats);

public:

    RetiSceneObject(): is_loaded(false){}
    /** Loads a RetiSceneObject from specified model file. File can be of any model type supported
    *   by Assimp. Note that this joins separate meshes whenever possible, and triangulates all
    *   non-triangular primitives. If no texture coordinates are present, all texture coordinates
    *   are set to 0. Also note that this uses the default Reticle shader interface.
    */
    RetiSceneObject(const std::string& path);
    RetiSceneObject(RetiMesh* msh, RetiTransform& trf = RetiTransform::getIdentity());
    RetiSceneObject(std::vector<RetiMesh*> mshs, std::vector<RetiTransform*> trf);
    RetiSceneObject(const RetiSceneObject& other);
    ~RetiSceneObject();
    RetiSceneObject& operator=(const RetiSceneObject& other);

    RetiTransform& getTransform();
    RetiTransform& getMeshTransform(int id);

    int addMesh(RetiMesh* msh, RetiTransform& trf = RetiTransform::getIdentity());
    void deleteMesh(int id);

    void loadObject();
    void unloadObject();

    void render(RetiCamera* cam);
};

#endif // RETISCENEOBJECT_H_INCLUDED
