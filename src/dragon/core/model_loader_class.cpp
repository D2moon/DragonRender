#include "dragon/core/model_loader_class.h"
#include <spdlog/spdlog.h>
#include <spdlog/spdlog.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


namespace Dragon {


    ModelLoader::ModelLoader() {

    }

    shared_ptr<Model> ModelLoader::Load(const string &filename) {
        auto file_path = path_tool_.GetFilePath(filename);
        if (!exists(file_path)) {
            spdlog::error("no such model file: {}", file_path.string());
            exit(0);
        }
        path_tool_.SetCurrentPath(file_path.parent_path());

        Assimp::Importer importer;
        auto scene = importer.ReadFile(file_path.string(),
                                       aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FixInfacingNormals |
                                       aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            spdlog::error("load model file failed {}", file_path.string());
            exit(0);
        }
        spdlog::warn("load model : {}", file_path.string());
        return LoadModel(scene);

    }

    shared_ptr<Model> ModelLoader::LoadModel(const aiScene *scene) {
        auto model = make_shared<Model>();
        ProcessNode(scene->mRootNode, scene, model);
        model->PostProcess();
        return model;

    }

    void ModelLoader::ProcessNode(const aiNode *node, const aiScene *scene, shared_ptr<Model> model) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            auto mesh_res = ProcessMesh(mesh, scene);
            model->AddMesh(mesh_res);
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene, model);
        }
    }

    shared_ptr<Mesh> ModelLoader::ProcessMesh(const aiMesh *mesh, const aiScene *scene) {

        std::vector<Vertex> vertices;
        std::vector<uint> indices;
        for (uint i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.normal = Normal3f(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            vertex.position = Vector3f(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
//            vertex.tangent = Vector3f(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
//            vertex.b_tangent = Vector3f(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                vertex.texcoord = Vector2f(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            } else
                vertex.texcoord = Vector2f(0.0f, 0.0f);
            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        return make_shared<Mesh>(vertices, indices);

    }

}