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
	mNoPostProcessEnabled{true},
	mInvertEnabled{false},
	mVignetteEnabled{false},
	mBlurEnabled{false},
	mSobelEnabled{false},
	mNoiseEnabled{false},
	mBlurSize{3},
	mNoiseStrength{30.0},
	mMeshIndex{0},
	mTimer{50}
{

}

Viewer::~Viewer()
{
	glDeleteTextures(1, &mTex);
	glDeleteTextures(1, &mFrameBufferTex);
	glDeleteRenderbuffers(1, &mFrameBufferDepth);
	glDeleteVertexArrays(1, &mPostVAO);
	glDeleteFramebuffers(1, &mFrameBuffer);
}

void Viewer::init()
{

	//Create graphics assets
	mWaxShader.init(SHADER_DIR, "wax.vs", "wax.fs");
	mIridescentShader.init(SHADER_DIR, "iri.vs", "iri.fs");
	mSphereMorphShader.init(SHADER_DIR, "morph.vs", "wax.fs");
	mPostProcessShader.init(SHADER_DIR, "post.vs", "post.fs");


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

	//Create framebuffer
	//Attach a color attachment and a depth attachment
	glGenTextures(1, &mFrameBufferTex);

	glBindTexture(GL_TEXTURE_2D, mFrameBufferTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWindowWidth, mWindowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenRenderbuffers(1, &mFrameBufferDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, mFrameBufferDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWindowWidth, mWindowHeight);

	glGenFramebuffers(1, &mFrameBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

	//Attach color attachment here
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,mFrameBufferTex, 0
	);

	//Attach depth buffer
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,mFrameBufferDepth
	);

	

	glGenVertexArrays(1, &mPostVAO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "not complete\n";
	}

	
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

	mPostProcessShader.use();
	mPostProcessShader.setUniformFloat("dx", 1.0f / static_cast<float>(mWindowWidth));
	mPostProcessShader.setUniformFloat("dy", 1.0f / static_cast<float>(mWindowHeight));
	mPostProcessShader.setUniformInt("tex", 0);
	mPostProcessShader.setUniformFloat("noiseStrength", mNoiseStrength);

	

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


	mViewMatrix = mCamera->getViewMatrix();


	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
	glBindTexture(GL_TEXTURE_2D, mTex);

	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



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


	mMesh->Render();




	//Post-process here
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);


	mPostProcessShader.use();

	mPostProcessShader.setUniformBool("invertEnabled", mInvertEnabled);
	mPostProcessShader.setUniformBool("vignetteEnabled", mVignetteEnabled);
	mPostProcessShader.setUniformBool("blurEnabled", mBlurEnabled);
	mPostProcessShader.setUniformBool("sobelEnabled", mSobelEnabled);
	mPostProcessShader.setUniformBool("noiseEnabled", mNoiseEnabled);
	mPostProcessShader.setUniformInt("blurSize", mBlurSize);
	mPostProcessShader.setUniformFloat("time", mTimer.getTimeElapsed());
	mPostProcessShader.setUniformFloat("noiseStrength",mNoiseStrength);


	glActiveTexture(GL_TEXTURE0);
	mPostProcessShader.setUniformInt("tex", 0);
	glBindTexture(GL_TEXTURE_2D, mFrameBufferTex);
	glBindVertexArray(mPostVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);


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
