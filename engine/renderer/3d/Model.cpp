/// <file>Model.cpp</file>
/// <summary>
/// モデルに関するクラスの実装。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#include "Model.h"
#include <iostream>
#include "../../core/debug/Log.h"
#include "../../rhi/RHI.h"
#include "../../platform/sdl/MediaLoader.h"

namespace yamaneko_engine {
	void Model::Draw(RHIShader& shader) {
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].Draw(shader);
		}
	}

	void Model::LoadModel(std::string path) {
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			Log::Write(Log::LogLevel::Error, "Model", import.GetErrorString());
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<RHITexture*> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;

			glm::vec3 vector;
			// 座標を取得	
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			// 法線を取得
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;

			// UV座標を取得
			if (mesh->mTextureCoords[0]) {
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.tex_coords = vec;
			}
			else {
				vertex.tex_coords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<RHITexture*> diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
			std::vector<RHITexture*> specular_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	std::vector<RHITexture*> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name)
	{
		RHIDevice* device = RHI::GetDevice();
		if (device == nullptr) {
			Log::Write(Log::LogLevel::Error, "Model", "RHIDevice is not initialized.");
			return {};
		}

		std::vector<RHITexture*> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);

			std::string filename = directory + '/' + std::string(str.C_Str());

			SDL_Surface* surface = MediaLoader::GetSurface(filename);
			SDL_Surface* formated = MediaLoader::ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32);

			TextureType texture_type = (type == aiTextureType_DIFFUSE) ? TextureType::Diffuse : TextureType::Specular;
			
			RHITexture* texture = device->CreateTexture2D(
				formated->w,
				formated->h,
				texture_type,
				formated->pixels
			);

			textures.push_back(texture);
		}
		return textures;
	}
}
