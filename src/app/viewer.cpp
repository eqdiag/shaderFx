#include "stb_image.h"

#include "core/settings.h"

#include "app/viewer.h"



Viewer::Viewer():
	App{},
	mViewerOpen{true},
	mMousePressed{false},
	mMouseInit{false},
	mMouseX{},
	mMouseY{},
	mWaxShaderEnabled{true},
	mIridescentEnabled{false},
	mSphereMorphEnabled{false},
	mMeshIndex{0},
	mTimer{50}
{

}

Viewer::~Viewer()
{
	glDeleteTextures(1, &mTex);
}

void Viewer::init()
{

	//Create graphics assets
	mWaxShader.init(SHADER_DIR, "wax.vs", "wax.fs");
	mIridescentShader.init(SHADER_DIR, "iri.vs", "iri.fs");
	mSphereMorphShader.init(SHADER_DIR, "morph.vs", "wax.fs");


	stbi_set_flip_vertically_on_load(true);
	int width, height, numChannels;

	std::string path{IMAGE_DIR};
	path += "wax.png";

	unsigned char* img = stbi_load(path.c_str(), &width, &height, &numChannels, 0);

	if (img == nullptr) {
		std::cout << "couldn't load img!\n";
	}
	else {
		std::cout << "WIDTH: " << width << std::endl;
		std::cout << "HEIGHT: " << height << std::endl;
		std::cout << "NUM_CHANNELS: " << numChannels << std::endl;
	}

	glGenTextures(1, &mTex);

	glBindTexture(GL_TEXTURE_2D, mTex);

	//Set tex params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(img);
	
	
	mMesh = mLoader.loadMesh(MODEL_DIR, "cow.obj");
	mCamera = std::make_unique<core::ArcCamera>(mMesh->getCentroid(), 2.0 * mMesh->getBoundingRadius());

	auto eye = mCamera->getEye();

	mModelMatrix = math::Mat4::identity();
	mViewMatrix = mCamera->getViewMatrix();
	mProjMatrix = math::Mat4::perspectiveProjection(3.14*45.0/180.0, 800.0 / 600.0, 0.1, 1000.);


	mWaxShader.use();
	mWaxShader.setUniformMat4("model", mModelMatrix.getRawData());
	mWaxShader.setUniformMat4("view",mViewMatrix.getRawData());
	mWaxShader.setUniformMat4("proj",mProjMatrix.getRawData());
	mWaxShader.setUniformInt("waxTex", 0);


	mIridescentShader.use();
	mIridescentShader.setUniformMat4("model", mModelMatrix.getRawData());
	mIridescentShader.setUniformMat4("view", mViewMatrix.getRawData());
	mIridescentShader.setUniformMat4("proj", mProjMatrix.getRawData());

	mSphereMorphShader.use();
	mSphereMorphShader.setUniformMat4("model", mModelMatrix.getRawData());
	mSphereMorphShader.setUniformMat4("view", mViewMatrix.getRawData());
	mSphereMorphShader.setUniformMat4("proj", mProjMatrix.getRawData());

	

	glClearColor(0.22f, 0.22f, 0.22f, 1.0f);
	glEnable(GL_DEPTH_TEST);

}

void Viewer::update()
{
	float t = mTimer.getTimeElapsed();
	math::Vec3 lightPos{2.0f*cos(t), 2.0f*sin(2.0f*t), 2.0f * sin(t) };

	mIridescentShader.use();
	mIridescentShader.setUniformFloat3("lightPos", lightPos);
}

void Viewer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mViewMatrix = mCamera->getViewMatrix();


	//Wax shader
	if (mWaxShaderEnabled) {
		mWaxShader.use();
		mWaxShader.setUniformMat4("view", mViewMatrix.getRawData());
	}

	//Iridescent shader
	if (mIridescentEnabled) {
		mIridescentShader.use();
		mIridescentShader.setUniformMat4("view", mViewMatrix.getRawData());
	}

	if (mSphereMorphEnabled) {
		mSphereMorphShader.use();
		mSphereMorphShader.setUniformMat4("view", mViewMatrix.getRawData());
		mSphereMorphShader.setUniformFloat("time", mTimer.getTimeElapsed());

	}


	//Sphere morph shader


	mMesh->Render();

	mTimer.tick();
}

const char* Viewer::getCurrentMesh() const
{
	return meshes[mMeshIndex];
}

void Viewer::nextMesh()
{
	mMeshIndex = (mMeshIndex + 1) % (sizeof(meshes) / sizeof(char * ));
	loadNewMesh(meshes[mMeshIndex]);
}

void Viewer::loadNewMesh(const char* modelFilename)
{
	replaceMesh(mLoader.loadMesh(MODEL_DIR, modelFilename));
}

void Viewer::replaceMesh(std::unique_ptr<core::Mesh> newMesh)
{
	mMesh.swap(newMesh);

	mCamera->reset(mMesh->getCentroid(), 2.0 * mMesh->getBoundingRadius());
}
