#ifndef VIEWER_H
#define VIEWER_H

#include <memory>

#include "core/app.h"
#include "core/shader.h"
#include "core/mesh.h"
#include "core/asset_loader.h"
#include "core/camera.h"
#include "core/timer.h"

#include "math/matrix.h"


static const char* meshes[] = {
	"cow.obj",
	"cube.obj",
	"sphere.obj",
	"suzanne.obj",
	"spot.obj",
	"teapot.obj"
};

class Viewer : public core::App {
public:
	Viewer();
	~Viewer() override;
	void init() override;
	void update();
	void render();

	const char* getCurrentMesh() const;
	void nextMesh();
	void loadNewMesh(const char* modelFilename);



	//Input state variables
	bool mViewerOpen;
	bool mMousePressed;
	bool mMouseInit;
	float mMouseX;
	float mMouseY;

	//Toggles for different effects
	bool mWaxShaderEnabled;
	bool mIridescentEnabled;
	bool mSphereMorphEnabled;

	//Shaders
	core::Shader mWaxShader;
	core::Shader mIridescentShader;
	core::Shader mSphereMorphShader;

	//Shader uniforms
	math::Mat4 mModelMatrix{};
	math::Mat4 mViewMatrix{};
	math::Mat4 mProjMatrix{};

	//Model and camera
	std::unique_ptr<core::Mesh> mMesh;
	std::unique_ptr<core::ArcCamera> mCamera;
	core::Timer mTimer;

private:

	void replaceMesh(std::unique_ptr<core::Mesh> newMesh);

	//Mesh data
	int mMeshIndex;

	//Wax texture
	GLuint mTex;

	core::AssetLoader mLoader{};


};


#endif