/// <file>Mesh.h</file>
/// <summary>
/// メッシュに関するクラスの定義。
/// </summary>
/// <remarks>
/// Copyright (c) 2026 junya005
///
/// This software is released under the MIT License.
/// https://github.com/junya005/yamaneko-engine
/// </remarks>

#pragma once

#include <vector>
#include <string>
#include <memory>
#include "../../rhi/RHITexture.h"
#include "../../rhi/RHIShader.h"
#include "../../rhi/RHIBuffer.h"
#include "../../rhi/RHIDevice.h"
#include "Vertex.h"

namespace yamaneko_engine {
	class Mesh {
	public:
		const std::vector<Vertex>& GetVertices() const { return vertices_; }
		const std::vector<unsigned int>& GetIndices() const { return indices_; }
		const std::vector<RHITexture*>& GetTextures() const { return textures_; }

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<RHITexture*> textures);
		void Draw(RHIShader& shader);
	private:
		std::vector<Vertex> vertices_;
		std::vector<unsigned int> indices_;
		std::vector<RHITexture*> textures_;

		std::unique_ptr<RHIBuffer> vertex_buffer_;
		std::unique_ptr<RHIBuffer> elements_buffer_;
		void SetUpMesh();
	};
}
