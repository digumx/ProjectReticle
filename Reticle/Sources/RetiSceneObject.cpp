#include <RetiSceneObject.h>
#include <RetiMesh.h>
#include <RetiMaterial.h>
#include <RetiTexture.h>
#include <Core/RetiLog.h>

using namespace std;

void RetiSceneObject::process_materials(const aiScene* scene, const string& dir,
                                         vector<RetiMaterial*>& mats, vector<bool>& used_mats)
{
    mats.reserve(scene->mNumMaterials);
    used_mats.reserve(scene->mNumMaterials);
    for(int i = 0; i < scene->mNumMaterials; i++) used_mats.push_back(false);
    for(int i = 0; i < scene->mNumMaterials; i++)
    {
        RetiMaterial* mat = new RetiMaterial();
        if(scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString diffPath_ai;
            scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &diffPath_ai);
            RetiTexture* diff = new RetiTexture(dir + FILEPATH_SEPARATOR + string(diffPath_ai.C_Str()));
            mat->setTexture("colorTexture", diff);
        }
        mats.push_back(mat);
    }
}

void RetiSceneObject::process_mesh(aiMesh* mesh, const aiScene* scene, RetiTransform& parent,
                                   vector<RetiMaterial*>& mats, vector<bool>& used_mats)
{
    #ifdef DEBUG_CODE
    RetiLog::logln("Processing mesh: " +    to_string(mesh->mNumVertices) + " " + to_string(mesh->mNumFaces));
    #endif // DEBUG_CODE

    float* vertCoords =     new float[mesh->mNumVertices * 3];
    float* texCoords =      new float[mesh->mNumVertices * 2];
    unsigned int* tris =    new unsigned int[mesh->mNumFaces * 3];

    for(int i = 0; i < mesh->mNumVertices; i++)
    {
        vertCoords[i*3]     = mesh->mVertices[i].x;
        vertCoords[i*3 + 1] = mesh->mVertices[i].y;
        vertCoords[i*3 + 2] = mesh->mVertices[i].z;
    }

    if(mesh->mTextureCoords[0])
        for(int i = 0; i < mesh->mNumVertices; i++)
        {
            texCoords[i*2]      = mesh->mTextureCoords[0][i].x;
            texCoords[i*2 + 1]  = mesh->mTextureCoords[0][i].y;
        }
    else for(int i = 0; i < (mesh->mNumVertices * 2); i++) texCoords[i] = 0;

    for(int i = 0; i < mesh->mNumFaces; i++)
    {
        tris[i*3]       = mesh->mFaces[i].mIndices[0];
        tris[i*3 + 1]   = mesh->mFaces[i].mIndices[1];
        tris[i*3 + 2]   = mesh->mFaces[i].mIndices[2];
    }

    RetiMesh* rmesh;
    if(mesh->mMaterialIndex >= 0)
    {
        rmesh = new RetiMesh(vertCoords, texCoords, mesh->mNumVertices, tris, mesh->mNumFaces, mats[mesh->mMaterialIndex]);
        used_mats[mesh->mMaterialIndex] = true;
    }
    else rmesh = new RetiMesh(vertCoords, texCoords, mesh->mNumVertices, tris, mesh->mNumFaces);

    RetiMeshHolder hldr;
    int j = meshes.size();
    meshes.push_back(hldr);
    meshes[j].mesh = rmesh;
    meshes[j].transf.setParent(parent);
}

void RetiSceneObject::process_node(aiNode* node, const aiScene* scene, RetiTransform& parent,
                                   vector<RetiMaterial*>& mats, vector<bool>& used_mats)
{
    RetiTransform* node_trf = new RetiTransform();
    node_trf->setParent(parent);
    for(int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* msh = scene->mMeshes[node->mMeshes[i]];
        process_mesh(msh, scene, *node_trf, mats, used_mats);
    }
    for(int i = 0; i < node->mNumChildren; i++)
        this->process_node(node->mChildren[i], scene, *node_trf, mats, used_mats);
}

RetiSceneObject::RetiSceneObject(const string& path)
{
    is_loaded = false;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_SortByPType);
    #ifdef DEBUG_CODE
    RetiLog::logln("GLuint size:" + to_string(sizeof(GLuint)));
    #endif // DEBUG_CODE
    string err(importer.GetErrorString());
    //#ifndef DEBUG_CODE
    if(!scene || !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
    //#endif // DEBUG_CODE
        RetiLog::logln("ASSIMP ERROR: " + string(importer.GetErrorString()) + ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ? "t" : "f"));  ///DEBUG_CODE

    string directory = path.substr(0, path.find_last_of(FILEPATH_SEPARATOR));

    vector<RetiMaterial*> mats;
    vector<bool> used_mats;
    process_materials(scene, directory, mats, used_mats);
    process_node(scene->mRootNode, scene, root_transf, mats, used_mats);
}

RetiSceneObject::RetiSceneObject(RetiMesh* msh, RetiTransform& trf) : root_transf()
{
    is_loaded = false;
    RetiMeshHolder hldr;
    meshes.push_back(hldr);
    int i = meshes.size() - 1;
    meshes[i].transf = trf;
    meshes[i].transf.setParent(root_transf);
    meshes[i].mesh = msh;
    meshes[i].is_cpy = false;
}

RetiSceneObject::RetiSceneObject(vector<RetiMesh*> mshs, vector<RetiTransform*> trf)  : root_transf()
{
    is_loaded = false;
    meshes.reserve(mshs.size());
    for(int i = 0; i < mshs.size(); i++)
    {
        RetiMeshHolder hldr;
        meshes.push_back(hldr);
        int j = meshes.size() - 1;
        if(trf[i]) meshes[j].transf = *(trf[i]);
        meshes[j].transf.setParent(root_transf);
        meshes[j].mesh = mshs[i];
        meshes[j].is_cpy = false;
    }
}

RetiSceneObject::RetiSceneObject(const RetiSceneObject& other)
{
    is_loaded = false;
    root_transf = other.root_transf;
    meshes.reserve(other.meshes.size());
    for(int i = 0; i < other.meshes.size(); i++)
    {
        RetiMeshHolder hldr;
        meshes.push_back(hldr);
        int j = meshes.size() - 1;
        meshes[j].transf = other.meshes[i].transf;
        meshes[j].transf.setParent(root_transf);
        meshes[j].is_cpy = true;
        meshes[j].mesh = other.meshes[i].mesh;
    }
}

RetiSceneObject::~RetiSceneObject()
{
    if(is_loaded)
    {
        RetiLog::logln("WARNING: Delete called on loaded RetiSceneObject, attempting to unload.");
        unloadObject();
    }
    for(int i = 0; i < meshes.size(); i++)
    {
        meshes[i].lock.acquire();
        if(!meshes[i].is_cpy) delete meshes[i].mesh;
        meshes[i].lock.release();
    }
}

RetiSceneObject& RetiSceneObject::operator=(const RetiSceneObject& other)
{
    #ifdef DEBUG_CODE
    RetiLog::logln("Assignment called, is loaded: " + to_string(is_loaded));
    #endif // DEBUG_CODE
    is_loaded = false;
    root_transf = other.root_transf;
    meshes.clear();
    meshes.reserve(other.meshes.size());
    for(int i = 0; i < other.meshes.size(); i++)
    {
        RetiMeshHolder hldr;
        meshes.push_back(hldr);
        int j = meshes.size() - 1;
        meshes[j].transf = other.meshes[i].transf;
        meshes[j].transf.setParent(root_transf);
        meshes[j].is_cpy = true;
        meshes[j].mesh = other.meshes[i].mesh;
    }
    return *this;
}

RetiTransform& RetiSceneObject::getTransform()
{
    return root_transf;
}

RetiTransform& RetiSceneObject::getMeshTransform(int id)
{
    return meshes[id].transf;
}

int RetiSceneObject::addMesh(RetiMesh* msh, RetiTransform& trf)
{
    RetiMeshHolder hldr;
    meshes.push_back(hldr);
    int j = meshes.size() - 1;
    meshes[j].transf = trf;
    meshes[j].transf.setParent(root_transf);
    meshes[j].mesh = msh;
    meshes[j].is_cpy = false;
    return j;
}

void RetiSceneObject::deleteMesh(int id)
{
    meshes[id].lock.acquire();
    if(!meshes[id].is_cpy) delete meshes[id].mesh;
    meshes[id].lock.release();
    meshes.erase(meshes.begin() + id);
}

void RetiSceneObject::loadObject()
{
    if(is_loaded) return;
    for(int i = 0; i < meshes.size(); i++)
        meshes[i].mesh->loadMesh();
    is_loaded = true;
    #ifdef DEBUG_CODE
    RetiLog::logln("Object Loaded.");
    #endif // DEBUG_CODE
}

void RetiSceneObject::unloadObject()
{
    #ifdef DEBUG_CODE
    RetiLog::logln("Unload called on object" + to_string(is_loaded));
    #endif // DEBUG_CODE
    if(!is_loaded) return;
    #ifdef DEBUG_CODE
    RetiLog::logln("Unloading object");
    #endif // DEBUG_CODE
    for(int i = 0; i < meshes.size(); i++)
        meshes[i].mesh->unloadMesh();
    is_loaded = false;
}

void RetiSceneObject::render(RetiCamera* cam)
{
    if(!is_loaded) loadObject();
    for(int i = 0; i < meshes.size(); i++)
        meshes[i].mesh->render(cam, meshes[i].transf);
}
