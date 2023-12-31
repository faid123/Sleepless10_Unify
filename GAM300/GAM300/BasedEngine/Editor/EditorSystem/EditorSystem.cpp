#include "EditorSystem.h"
#include "Engine.h"

bool EditorSystem::_RenderObjWireframe;
bool EditorSystem::_RenderObjAABB;
bool EditorSystem::_RenderLightBox;
bool EditorSystem::_PlayInFullScreen;
bool EditorSystem::_RenderUI;
EntityID EditorSystem::_selectedEntity;

void EditorSystem::init()
{
   
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    //io.IniFilename = NULL;
    //io.IniSavingRate = -1;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    imgui_editor_mode = 1;
    auto windowSystem = Engine::_ecs.getSystem<WindowSystem>();
    getWinPtr(windowSystem->getWindowPtr());
    _RenderUI = true;
}

void EditorSystem::load()
{

}

void EditorSystem::update(float delta_time)
{
    // Update other events like input handling 
    glfwPollEvents();

    // Update all game components using previously computed delta_time ...

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    //When Frame Buffer is done, uncomment this section (SAM)
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    //editorWindow.DrawPropertyWindow();
    editorWindow.showTopMenu();
    //editorWindow.showFps();
    editorWindow.showPrefab();
    editorWindow.showEntity();
    editorWindow.showProperty();
    editorWindow.showParenting();
    editorWindow.showWayPoint();
    //editorWindow.updatePrefabsProperty();
    editorWindow.showPropertyPrefab();
    //editorWindow.showComponents();
    editorWindow.showConsole();
    editorWindow.showViewport();
    editorWindow.showDebug(delta_time, timerList);
    editorWindow.ShowAudioInspector();
    //Debug(delta_time);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorSystem::unload()
{

}

void EditorSystem::release()
{
    //Imgui Exits
    //ImGui sShutdown
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}


void EditorSystem::getWinPtr(GLFWwindow* wptr)
{
    ImGui_ImplGlfw_InitForOpenGL(wptr, true);
}

void EditorSystem::setCamID(const EntityID& camId)
{
    _camID = camId;
}

void EditorSystem::AddSysTime(std::pair<std::string, double> time)
{
    timerList.push_back(time);
}