/// <file>Model.h</file>
/// <summary>
/// モデルに関するクラスの定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "../../rhi/RHIDevice.h"
#include "../../rhi/RHIShader.h"
#include "../../rhi/RHITexture.h"

namespace yamaneko_engine {
	class Model {
	public:
		Model(const char* path) {
			LoadModel(path);
		}
		void Draw(RHIShader& shader);
	private:
		std::vector<Mesh> meshes;
		std::string directory;

		void LoadModel(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<RHITexture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name);
	};
}
