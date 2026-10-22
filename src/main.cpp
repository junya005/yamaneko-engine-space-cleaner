#define SDL_MAIN_HANDLED
#include "../engine/Engine.h"
#include <imgui.h>
#include <algorithm>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Engine {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex_coords;
	};

	struct Texture {
		unsigned int id;
		std::string type;
		std::string path;
	};

	struct Transform {
	public:
		glm::vec3 position;
		glm::vec3 rotation; // ラジアン単位
		glm::vec3 scale;
		Transform() : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f) {}

		glm::mat4 GetRotationMatrix() const {
			glm::mat4 rot = glm::mat4(1.0f);
			rot = glm::rotate(rot, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			rot = glm::rotate(rot, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			rot = glm::rotate(rot, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
			return rot;
		}

		glm::vec3 GetForwardVector() const {
			return glm::vec3(GetRotationMatrix() * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
		}

		glm::vec3 GetUpVector() const {
			return glm::vec3(GetRotationMatrix() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
		}

		glm::vec3 GetRightVector() const {
			return glm::vec3(GetRotationMatrix() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
		}

		glm::mat4 GetModelMatrix() const {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, position);
			model = model * GetRotationMatrix();
			model = glm::scale(model, scale);
			return model;
		}

	};

	// ファイルを文字列でロードする関数
	inline std::string LoadFileToString(const std::string& filePath) {
		std::ifstream file(filePath);
		if (!file.is_open()) {
			return "";
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	inline std::string LoadFileToString(const char* filePath) {
		return LoadFileToString(std::string(filePath));
	}

	// 毎回 / 1000.0f するのは面倒なので、秒単位で取得する関数を用意
	inline float GetTime() {
		return SDL_GetTicks() / 1000.0f;
	}

	inline glm::vec2 GetMouseMotion() {
		int x, y;
		SDL_GetRelativeMouseState(&x, &y);
		return glm::vec2(x, y);
	}

	class Shader {
	public:
		unsigned int ID;

		Shader(const char* vertexPath, const char* fragmentPath);
		void use();
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec2(const std::string& name, float x, float y) const;
		void setVec3(const std::string& name, float x, float y, float z) const;
		void setVec4(const std::string& name, float x, float y, float z, float w) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;
		void setSampler2D(const std::string& name, int textureUnit) const;
	};

	Shader::Shader(const char* vertexPath, const char* fragmentPath) {
		// 頂点シェーダーのロード
		std::string vertexShaderSource = Engine::LoadFileToString(vertexPath);
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* vertexShaderSourceCStr = vertexShaderSource.c_str();
		glShaderSource(vertexShader, 1, &vertexShaderSourceCStr, NULL);
		glCompileShader(vertexShader);

		int  success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// フラグメントシェーダーのロード
		std::string fragmentShaderSource = Engine::LoadFileToString(fragmentPath);
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragmentShaderSourceCStr = fragmentShaderSource.c_str();
		glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// シェーダープログラムの作成
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void Shader::use() {
		glUseProgram(ID);
	}

	void Shader::setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void Shader::setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void Shader::setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void Shader::setVec2(const std::string& name, float x, float y) const {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}

	void Shader::setVec3(const std::string& name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void Shader::setSampler2D(const std::string& name, int textureUnit) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), textureUnit);
	}

	class Mesh {
	public:
		const std::vector<Vertex>& GetVertices() const { return vertices_; }
		const std::vector<unsigned int>& GetIndices() const { return indices_; }
		const std::vector<Texture>& GetTextures() const { return textures_; }

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		void Draw(Shader& shader);
	private:
		std::vector<Vertex> vertices_;
		std::vector<unsigned int> indices_;
		std::vector<Texture> textures_;

		unsigned int vao_, vbo_, ebo_;
		void SetUpMesh();
	};

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) :
		vertices_(std::move(vertices)),
		indices_(std::move(indices)),
		textures_(std::move(textures))
	{
		SetUpMesh();
	}

	void Mesh::Draw(Shader& shader) {
		// LernOpenGLの手法に従い、シェーダーに特定の命名規則があるものとして設定します
		unsigned int diffuse_nr = 1;
		unsigned int specular_nr = 1;
		for(unsigned int i = 0; i < textures_.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			std::string number;
			std::string name = textures_[i].type;
			if(name == "texture_diffuse")
				number = std::to_string(diffuse_nr++);
			else if(name == "texture_specular")
				number = std::to_string(specular_nr++);

			shader.setSampler2D((name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures_[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(vao_);
		glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Mesh::SetUpMesh() {
		// buffer/array objectsの作成
		glGenVertexArrays(1, &vao_);
		glGenBuffers(1, &vbo_);
		glGenBuffers(1, &ebo_);

		glBindVertexArray(vao_);
		// 頂点バッファからデータをロードする
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);

		// 構造体のメモリ仕様（データが横並びになる）を利用し、配列のままデータを送信する
		glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), indices_.data(), GL_STATIC_DRAW);

		// 頂点座標
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		// 頂点の法線データ 
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		// UV座標データ	
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

		glBindVertexArray(0);
	}

	class Model {
	public:
		Model(const char* path) {
			LoadModel(path);
		}
		void Draw(Shader& shader);
	private:
		std::vector<Mesh> meshes;
		std::string directory;

		void LoadModel(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name);
	};

	void Model::Draw(Shader& shader) {
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].Draw(shader);
		}
	}

	void Model::LoadModel(std::string path) {
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
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
		std::vector<Texture> textures;

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
			if(mesh->mTextureCoords[0]) {
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
			std::vector<Texture> diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
			std::vector<Texture> specular_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name)
	{
		std::vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);

			std::string filename = directory + '/'+std::string(str.C_Str());

			Texture texture;
			texture.id = Engine::AssetManager::LoadTexture(filename);
			texture.type = type_name;
			textures.push_back(texture);
		}
		return textures;
	}

	enum MoveDirection {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	class Camera {
	public:
		inline static Camera* current = nullptr;
		inline static std::vector<Camera*> cameras = std::vector<Camera*>();

		Engine::Transform transform;
		float fov = 45.0f; // 視野角
		bool for_debug = false; // デバッグ用カメラかどうか
		float aspect_ratio = 16.0f / 9.0f; // アスペクト比
		float near_plane = 0.1f; // 近平面距離
		float far_plane = 100.0f; // 遠平面距離

		Camera(bool for_debug = false) : transform(), for_debug(for_debug) {
			if (current == nullptr) {
				current = this;
			}
			cameras.push_back(this);
		}

		~Camera() {
			auto it = std::find(cameras.begin(), cameras.end(), this);
			if (it != cameras.end()) {
				cameras.erase(it);
			}
			if (current == this) {
				current = cameras.empty() ? nullptr : cameras.back();
			}

			if (cameras.empty())
			{
				current = nullptr;
				cameras.clear();
			}
		}

		void ProcessMovement(MoveDirection direction, float velocity, float deltaTime) {
			velocity *= deltaTime; // カメラの移動速度

			glm::vec3 forward = transform.GetForwardVector();
			glm::vec3 right = transform.GetRightVector();
			glm::vec3 up = transform.GetUpVector();

			if (direction == FORWARD)
				transform.position += forward * velocity;
			if (direction == BACKWARD)
				transform.position -= forward * velocity;
			if (direction == RIGHT)
				transform.position += right * velocity;
			if (direction == LEFT)
				transform.position -= right * velocity;
			if (direction == UP)
				transform.position += up * velocity;
			if (direction == DOWN)
				transform.position -= up * velocity;
		}

		void ProcessViewMovement(float xoffset, float yoffset) {
			float sensitivity = 0.1f; // マウス感度
			xoffset *= sensitivity;
			yoffset *= sensitivity;
			transform.rotation.y += glm::radians(xoffset);
			transform.rotation.x += glm::radians(yoffset);
			// 上下の回転を制限
			if (transform.rotation.x > glm::radians(89.0f))
				transform.rotation.x = glm::radians(89.0f);
			if (transform.rotation.x < glm::radians(-89.0f))
				transform.rotation.x = glm::radians(-89.0f);
		}

		void ProcessZoom(float yoffset) {
			fov -= yoffset;
			if (fov < 1.0f)
				fov = 1.0f;
			if (fov > 45.0f)
				fov = 45.0f;
		}

		glm::highp_mat4 GetViewMatrix() const {
			glm::vec3 forward = transform.GetForwardVector();
			glm::vec3 up = transform.GetUpVector();
			return glm::lookAt(transform.position, transform.position + glm::normalize(forward), up);
		}

		glm::highp_mat4 GetProjectionMatrix() const {
			return glm::perspective(glm::radians(fov), aspect_ratio, near_plane, far_plane);
		}
	};
}

// 三角形の頂点座標
inline float triangle_vertex[9] = {
	// 1つ目の三角形
	-0.5f,  -0.5f, 0.0f, // 左下頂点
	0.5f, -0.5f, 0.0f, // 右下頂点
	0.0f, 0.5f, 0.0f,  // 上頂点
};

inline float triangle_vertex2[9] = {
	// 2つ目の三角形
	0.5f,  -0.5f, 0.0f, // 左下頂点
	1.5f, -0.5f, 0.0f, // 右下頂点
	1.0f, 0.5f, 0.0f  // 上頂点
};

// 立方体の頂点データ
inline float cube_vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

/// <summary>
/// アプリケーションのエントリポイント
/// </summary>
int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	// 1. エンジンの初期化
	if (!Engine::Init("Template Game", 1280, 720)) {
		return -1;
	}

	// プレイヤーの変数（初期位置、速度、サイズ）
	float playerX = 100.0f;
	float playerY = 100.0f;
	float playerSpeed = 300.0f;
	float playerSize = 50.0f;
	float cameraSpeed = 2.5f;

	std::unique_ptr<Engine::Camera> camera = std::make_unique<Engine::Camera>(true); // デバッグ用カメラを作成

	glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
	float yaw = -90.0f;
	float pitch = 0.0f;

	float fov = 45.0f; // 視野角（Field of View）

	int screenWidth = Engine::Window::Width();
	int screenHeight = Engine::Window::Height();

	// Hello Triangle: https://learnopengl.com/Getting-started/Hello-Triangle

	// 深度テストを有効化
	glEnable(GL_DEPTH_TEST);
	
	// VAOの作成とバインド
	unsigned int triangleVAOs[2];
	glGenVertexArrays(2, triangleVAOs);
	glBindVertexArray(triangleVAOs[0]);
	
	// 三角形の頂点バッファを作成
	unsigned int triangleVBOs[2];
	glGenBuffers(2, triangleVBOs);

	// 描画する三角形の頂点データを設定
	glBindVertexArray(triangleVAOs[0]);
	// 頂点バッファのバインドとデータ転送
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertex), triangle_vertex, GL_STATIC_DRAW);
	// 頂点属性の設定
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 2つ目の三角形も同様に
	glBindVertexArray(triangleVAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertex2), triangle_vertex2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 立方体のVAOとVBOの作成
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);

	// 立方体の頂点バッファのバインドとデータ転送
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	// 頂点座標の頂点属性を設定
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// UV座標の頂点属性を設定
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// 立方体用のテクスチャを作成
	// テクスチャ読み込み
	GLuint texture = Engine::AssetManager::LoadTexture("assets/texture/4K-metal_5-diffuse.jpg");
	std::cout << "Texture ID: " << texture << std::endl;
	
	// 画像リソースの解放はAssetManagerが行うので、ここでは解放しない

#pragma region Create Shader Program

	std::unique_ptr<Engine::Shader> shaderProgram = 
		std::make_unique<Engine::Shader>(
			"assets/shader/simple_vertex_shader.glsl", 
			"assets/shader/simple_fragment_shader.glsl"
		);

#pragma endregion

	std::unique_ptr<Engine::Model> airplane = std::make_unique<Engine::Model>(Engine::Model("assets/model/airplane/airplane.obj"));

	SDL_SetRelativeMouseMode(SDL_TRUE);
			
	// 2. メインゲームループ
	while (Engine::ProcessEvents()) {
		float deltaTime = Engine::Time::GetDeltaTime();

		
		// WASDキー入力によるプレイヤー移動
		float moveDistance = playerSpeed * deltaTime;
		float cameraSpeedDelta = cameraSpeed * deltaTime;
		if (Engine::Input::GetKey(SDL_SCANCODE_W)) {
			// playerY -= moveDistance;
			camera->ProcessMovement(Engine::MoveDirection::FORWARD, cameraSpeed, deltaTime);
		}
		if (Engine::Input::GetKey(SDL_SCANCODE_S)) {
			// playerY += moveDistance;
			camera->ProcessMovement(Engine::MoveDirection::BACKWARD, cameraSpeed, deltaTime);
		}
		if (Engine::Input::GetKey(SDL_SCANCODE_A)) {
			// playerX -= moveDistance;
			camera->ProcessMovement(Engine::MoveDirection::LEFT, cameraSpeed, deltaTime);
		}
		if (Engine::Input::GetKey(SDL_SCANCODE_D)) {
			// playerX += moveDistance;
			camera->ProcessMovement(Engine::MoveDirection::RIGHT, cameraSpeed, deltaTime);
		}
		if(Engine::Input::GetKey(SDL_SCANCODE_ESCAPE)) {
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}

		// マウスの動きによるカメラの向き変更
		glm::vec2 mouseMotion = Engine::GetMouseMotion();
		camera->ProcessViewMovement(mouseMotion.x, mouseMotion.y);

		// マウスホイールによるカメラのズーム
		float mouseWheel = Engine::Input::GetMouseWheel();
		camera->ProcessZoom(mouseWheel);

		// 画面外への飛び出し防止（クランプ処理）
		if (screenWidth > 0 && screenHeight > 0) {
			playerX = std::clamp(playerX, 0.0f, static_cast<float>(screenWidth) - playerSize);
			playerY = std::clamp(playerY, 0.0f, static_cast<float>(screenHeight) - playerSize);
		}

		// vectorの変換テスト
		/*
		glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, Engine::GetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
		vec = trans * vec;
		*/

		// モデルマトリックスの作成
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		// ビュー行列の作成
		glm::mat4 view = glm::mat4(1.0f);
		view = camera->GetViewMatrix();

		// プロジェクション行列の作成
		glm::mat4 projection = glm::mat4(1.0f);
		projection = camera->GetProjectionMatrix();

		// 画面クリア
		Engine::Clear(0.1f, 0.12f, 0.15f, 1.0f);
		// 後でEngine::Clear()に追加
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// シェーダーに値を送信する
		// float time = Engine::GetTime();
		// float greenValue = (sin(time) / 2.0f) + 0.5f;
		shaderProgram->use();
		shaderProgram->setMat4("model", model);
		shaderProgram->setMat4("view", view);
		shaderProgram->setMat4("projection", projection);
		shaderProgram->setSampler2D("ourTexture", 0); // テクスチャユニット0を使用
		shaderProgram->setVec4("ourColor", 1.0f, 1.0f, 1.0f, 1.0f);
		
		// Hello Triangle: 描画処理
		shaderProgram->use();
		// glBindVertexArray(triangleVAOs[0]);
		// glDrawArrays(GL_TRIANGLES, 0, 3);

		// glBindVertexArray(triangleVAOs[1]);
		// glDrawArrays(GL_TRIANGLES, 0, 3);

		// 立方体の描画
		glActiveTexture(GL_TEXTURE0); // テクスチャユニット0をアクティブにする
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 自機の描画
		airplane->Draw(*shaderProgram);

		// ImGui フレーム開始
		Engine::BeginImGui();

		// プレイヤー四角形の描画（ImGui BackgroundDrawList）
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		ImVec2 pMin(playerX, playerY);
		ImVec2 pMax(playerX + playerSize, playerY + playerSize);
		drawList->AddRectFilled(pMin, pMax, IM_COL32(255, 255, 255, 255));
		drawList->AddRect(pMin, pMax, IM_COL32(100, 200, 255, 255), 0.0f, 0, 2.0f);

		// デバッグ・操作情報ウィンドウの描画
		ImGui::SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2(320.0f, 220.0f), ImGuiCond_Once);

		if (ImGui::Begin("Template Controller & Debug")) {
			ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "[Controls]");
			ImGui::BulletText("W / A / S / D : Move Camera");
			ImGui::BulletText("Mouse Drag    : Move Camera View");
			ImGui::BulletText("Mouse Wheel   : Zoom Camera");

			ImGui::Separator();
			ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "[Performance]");
			ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
			ImGui::Text("DeltaTime: %.4f s", Engine::Time::GetDeltaTime());
			ImGui::Text("TotalTime: %.1f s", Engine::Time::GetTotalTime());

			ImGui::Separator();
			ImGui::TextColored(ImVec4(0.4f, 0.8f, 1.0f, 1.0f), "[Player Info]");
			ImGui::Text("Position: (%.1f, %.1f, %1f)", camera->transform.position.x, camera->transform.position.y, camera->transform.position.z);

			if (ImGui::Button("Reset Position")) {
				camera->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
			}
		}
		ImGui::End();

		// ImGui 描画発行
		Engine::EndImGui();

		// バックバッファスワップ（画面反映）
		Engine::Present();
	}

	// 3. エンジンの終了
	Engine::Shutdown();
	return 0;
}
