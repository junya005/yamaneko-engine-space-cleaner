add_library(yamaneko_engine STATIC
	# エンジンコア
	core/config/Config.h core/config/Config.cpp
	core/debug/Log.h
	core/math/Math.h
	core/math/ViewInfo.h
	core/math/Transform.h
	core/math/Random.h
	core/io/FileSystem.h core/io/FileSystem.cpp
	core/Core.h

	# OS
	platform/sdl/Input.h platform/sdl/Input.cpp
	platform/sdl/Time.h platform/sdl/Time.cpp
	platform/sdl/Window.h platform/sdl/Window.cpp
	platform/sdl/MediaLoader.h platform/sdl/MediaLoader.cpp
	platform/imgui/ImGuiContext.h platform/imgui/ImGuiContext.cpp

	# Renderer
	 
	renderer/3d/Vertex.h 
	renderer/3d/Mesh.h renderer/3d/Mesh.cpp 
	renderer/3d/Model.h renderer/3d/Model.cpp
	renderer/ui/UIRenderer.h renderer/ui/UIRenderer.cpp

	# RHI
	rhi/RHIBuffer.h
	rhi/RHICommandList.h
	rhi/RHIDevice.h
	rhi/RHIDeviceDesc.h
	rhi/RHIShader.h
	rhi/RHITexture.h
	rhi/RHI.h rhi/RHI.cpp
	rhi/backend/opengl/GLBuffer.h
	rhi/backend/opengl/GLDevice.h
	rhi/backend/opengl/GLShader.h 
	rhi/backend/opengl/GLTexture.h
	rhi/backend/opengl/GLCommandList.h

	# Scene
	scene/Scene.h
	scene/SceneManager.h scene/SceneManager.cpp
	scene/GameObject.h  
	scene/module/Module.h  scene/module/Module.cpp
	scene/module/sub/SubModule.h scene/module/sub/SubModule.cpp
	scene/module/Camera.h
	scene/Collision.h

	# エントリーポイント
	main/Application.h main/Application.cpp
	main/EntryPoint.h
	Engine.h
 "rhi/RHI.cpp" "core/debug/Log.cpp" "core/math/Random.cpp")

target_include_directories(yamaneko_engine PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(yamaneko_engine PUBLIC
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    glad::glad
    imgui::imgui
    assimp::assimp
    OpenGL::GL
)
