#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "app/viewer.h"
#include "app/viewer_ui.h"


ViewerUi::ViewerUi(core::Window& window, Viewer& viewer) :
    Ui{ window }, mViewer{ viewer}
{
}

void ViewerUi::update()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoBackground;



    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Shader Effects Menu")) {
                mViewer.mViewerOpen = !(mViewer.mViewerOpen);
            }
            ImGui::EndMenu();
        }

        //TODO: add fps here
        ImGui::Text("FPS: %f", mViewer.mTimer.getFps());

        ImGui::EndMainMenuBar();
    }

    if (mViewer.mViewerOpen) {

        ImGui::Begin("Shader Effects Menu");

        if (ImGui::Button("Next Mesh")) {
            mViewer.nextMesh();
        }

        ImGui::Text("Mesh: %s",mViewer.getCurrentMesh());
     

        if (ImGui::CollapsingHeader("Vertex/Fragment Effects", ImGuiTreeNodeFlags_DefaultOpen)) {

            if (ImGui::Checkbox("Wax",&mViewer.mWaxShaderEnabled)) {
                mViewer.mIridescentEnabled = false;
                mViewer.mSphereMorphEnabled = false;
            }

            if (ImGui::Checkbox("Iridescence", &mViewer.mIridescentEnabled)) {
                mViewer.mWaxShaderEnabled = false;
                mViewer.mSphereMorphEnabled = false;
            }

            if (ImGui::Checkbox("Sphere Morph", &mViewer.mSphereMorphEnabled)) {
                mViewer.mIridescentEnabled = false;
                mViewer.mWaxShaderEnabled = false;
            }

        }

        if (ImGui::CollapsingHeader("Post-Processing Effects", ImGuiTreeNodeFlags_DefaultOpen))
        {

            if (ImGui::Checkbox("None", &mViewer.mNoPostProcessEnabled)) {
                mViewer.mInvertEnabled = false;
                mViewer.mVignetteEnabled = false;
                mViewer.mBlurEnabled = false;
                mViewer.mSobelEnabled = false;
                mViewer.mNoiseEnabled = false;
            }

            if (ImGui::Checkbox("Invert Effect", &mViewer.mInvertEnabled)) {
                mViewer.mNoPostProcessEnabled = false;
                mViewer.mVignetteEnabled = false;
                mViewer.mBlurEnabled = false;
                mViewer.mSobelEnabled = false;
                mViewer.mNoiseEnabled = false;
            }

            if (ImGui::Checkbox("Vignette Effect", &mViewer.mVignetteEnabled)) {
                mViewer.mNoPostProcessEnabled = false;
                mViewer.mBlurEnabled = false;
                mViewer.mSobelEnabled = false;
                mViewer.mNoiseEnabled = false;
                mViewer.mInvertEnabled = false;
            }

            if (ImGui::Checkbox("Blur Effect", &mViewer.mBlurEnabled)) {
                mViewer.mNoPostProcessEnabled = false;
                mViewer.mVignetteEnabled = false;
                mViewer.mSobelEnabled = false;
                mViewer.mNoiseEnabled = false;
                mViewer.mInvertEnabled = false;
            }

            ImGui::SameLine();

            ImGui::SliderInt("Filter Size", &mViewer.mBlurSize, 3, 15);

            if (ImGui::Checkbox("Sobel Effect", &mViewer.mSobelEnabled)) {
                mViewer.mNoPostProcessEnabled = false;
                mViewer.mVignetteEnabled = false;
                mViewer.mBlurEnabled = false;
                mViewer.mNoiseEnabled = false;
                mViewer.mInvertEnabled = false;
            }



            if (ImGui::Checkbox("Noisy Effect", &mViewer.mNoiseEnabled)) {
                mViewer.mNoPostProcessEnabled = false;
                mViewer.mVignetteEnabled = false;
                mViewer.mSobelEnabled = false;
                mViewer.mBlurEnabled = false;
                mViewer.mInvertEnabled = false;
            }

            ImGui::SameLine();

            ImGui::SliderFloat("Noise Strength", &mViewer.mNoiseStrength,10.0,100.0);

        }

    }
    ImGui::End();

}
