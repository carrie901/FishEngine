#include "EditorGUI.hpp"

#include <sstream>
#include <iostream>
#include <fstream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_dock.h>
#include <imgui/imgui_user.h>
#include <boost/lexical_cast.hpp>

#include <GameObject.hpp>
#include <Scene.hpp>
#include <Material.hpp>
#include <MeshFilter.hpp>
#include <MeshRenderer.hpp>
#include <Mesh.hpp>
#include <Common.hpp>
#include <Debug.hpp>
#include <Time.hpp>
#include <Matrix4x4.hpp>
#include <Bounds.hpp>
#include <Ray.hpp>
#include <ModelImporter.hpp>
#include <Gizmos.hpp>
#include <BoxCollider.hpp>
#include <SphereCollider.hpp>
#include <CapsuleCollider.hpp>
#include <Rigidbody.hpp>
#include <Pipeline.hpp>
#include <SkinnedMeshRenderer.hpp>
#include <Camera.hpp>
#include <Component_gen.hpp>
#include <Light.hpp>

#include "FishEditorWindow.hpp"
#include "Selection.hpp"
#include "EditorRenderSystem.hpp"
#include "EditorInput.hpp"

using namespace FishEngine;

namespace FishEditor
{
    constexpr float inspector_indent_width              = 4;

    int     EditorGUI::m_idCount                        = 0;

    PSceneViewEditor EditorGUI::m_mainSceneViewEditor;

    bool    EditorGUI::s_locked = false;
    //int     EditorGUI::m_selectedAxis                   = -1;
    bool    EditorGUI::s_isAnyItemClicked               = false;
    bool    EditorGUI::s_openMenuPopup                  = false;
    bool    EditorGUI::m_showAssectSelectionDialogBox   = false;

    bool    EditorGUI::s_mouseEventHandled              = false;

    ImGuiWindowFlags globalWindowFlags = 0;
    
    FishEngine::Int2 EditorGUI::m_sceneSize{1, 1};

    const char* ToString(TransformSpace& space)
    {
        if (space == TransformSpace::Global)
            return "Global";
        else
            return "Local";
    }

    struct EditoGUISettings
    {
        float mainMenubarHeight;
        float mainToolbarHeight;
    };

    EditoGUISettings g_editorGUISettings;


    void EditorGUI::Init()
    {
#if FISHENGINE_PLATFORM_WINDOWS
        const std::string root_dir = "../../../../assets/";
#else
        const std::string root_dir = "/Users/yushroom/program/graphics/FishEngine/assets/";
#endif

        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF((root_dir + "fonts/DroidSans.ttf").c_str(), 14.0f);
        io.IniFilename = nullptr;

        ImGuiContext& g = *GImGui;
        ImGuiStyle& style = g.Style;
        style.FrameRounding = 4.f;
        style.WindowRounding = 0.f;
        style.Colors[ImGuiCol_Text]         = ImVec4(0, 0, 0, 1);
        style.Colors[ImGuiCol_Button]       = ImVec4(171/255.f, 204/255.f, 242/255.f, 1.f);
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(190 / 255.f, 224 / 255.f, 262 / 255.f, 1.f);
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(181 / 255.f, 214 / 255.f, 232 / 255.f, 1.f);
        style.Colors[ImGuiCol_WindowBg]     = ImVec4(0.8f, 0.8f, 0.8f, 0.6f);
        style.Colors[ImGuiCol_MenuBarBg]    = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
        style.Colors[ImGuiCol_TitleBg]      = ImVec4(0.5f, 0.5f, 0.5f, 0.8f);
        style.Colors[ImGuiCol_PopupBg]      = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        style.Colors[ImGuiCol_ComboBg]      = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
        //style.GrabRounding = 0.f;
        //style.WindowTitleAlign = ImGuiAlign_Left | ImGuiAlign_VCenter;
        //style.WindowMinSize = ImVec2(128, 128);

        globalWindowFlags |= ImGuiWindowFlags_NoCollapse;
        globalWindowFlags |= ImGuiWindowFlags_NoResize;
        globalWindowFlags |= ImGuiWindowFlags_ShowBorders;
        globalWindowFlags |= ImGuiWindowFlags_NoMove;
        //CalculateWindowSizeAndPosition();
        m_mainSceneViewEditor = std::make_shared<SceneViewEditor>();
        m_mainSceneViewEditor->Init();
    }


    void EditorGUI::Update()
    {
        s_mouseEventHandled = false;
        auto selectedGO = Selection::selectedGameObjectInHierarchy();

        if (EditorInput::GetKeyDown(KeyCode::F)) {
            Camera::main()->FrameSelected(selectedGO);
        }
        
        if (EditorInput::GetKeyDown(KeyCode::W))
        {
            m_mainSceneViewEditor->m_transformToolType = TransformToolType::Translate;
        }
        else if (EditorInput::GetKeyDown(KeyCode::E))
        {
            m_mainSceneViewEditor->m_transformToolType = TransformToolType::Rotate;
        }
        else if (EditorInput::GetKeyDown(KeyCode::R))
        {
            m_mainSceneViewEditor->m_transformToolType = TransformToolType::Scale;
        }

        if (EditorInput::GetKeyDown(KeyCode::LeftControl) && EditorInput::GetKeyDown(KeyCode::Z))
        {
            if (EditorInput::GetKeyDown(KeyCode::LeftShift))
            {
                Debug::LogWarning("Ctrl+Shift+Z");
                CommandManager::Redo();
            }
            else
            {
                Debug::LogWarning("Ctrl+Z");
                CommandManager::Undo();
            }
        }
        

        DrawMainMenu();
        DrawMainToolbar();
        DrawHierarchyWindow();
        DrawInspectorWindow();
        DrawProjectWindow();
        DrawSceneView();

        auto size = ImGui::GetIO().DisplaySize;
        float pos_y = g_editorGUISettings.mainMenubarHeight + g_editorGUISettings.mainToolbarHeight;
        size.y -= pos_y;
        ImGui::RootDock(ImVec2(0, pos_y), size);

    //    if (m_showAssectSelectionDialogBox) {
    //        ImGui::OpenPopup("Select ...");
    //        ImGuiWindowFlags window_flags = 0;
    //        window_flags |= ImGuiWindowFlags_NoCollapse;
    //        if (ImGui::BeginPopupModal("Select ...", &m_showAssectSelectionDialogBox, window_flags)) {
    //            for (auto m : Mesh::m_meshes) {
    //                if (ImGui::Button(m->name().c_str())) {
    //                    Debug::Log("%s", m->name().c_str());
    //                }
    //            }
    //            ImGui::Separator();
    //            ImGui::EndPopup();
    //        }
    //    }

        ImGui::Render();
        
        //if (!s_mouseEventHandled && Input::GetMouseButtonDown(0))
        //{
        //    Ray ray = Camera::main()->ScreenPointToRay(Input::mousePosition());
        //    auto go = Scene::IntersectRay(ray);
        //    Selection::setSelectedGameObjectInHierarchy(go);
        //}

        //if (EditorInput::GetKeyDown(KeyCode::P))
        //{
        //    Debug::Log("save layout");
        //    std::ofstream fout("./FishEditorLayout.json");
        //    ImGui::SaveDock(fout);
        //}

        //if (EditorInput::GetKeyDown(KeyCode::O))
        //{
        //    Debug::Log("load layout");
        //    std::ifstream fin("./FishEditorLayout.json");
        //    ImGui::LoadDock(fin);
        //}
    }

    void EditorGUI::Clean()
    {
        ImGui::ShutdownDock();
        ImGui_ImplGlfwGL3_Shutdown();
    }

    bool EditorGUI::Button(const char* text)
    {
        ImGuiContext& g = *GImGui;
        ImGuiStyle& style = g.Style;
        float w = ImGui::GetWindowWidth() - style.WindowPadding.x * 2.f;
        return ImGui::Button(text, ImVec2(w, 0));
    }

    void EditorGUI::Matrix4x4(const std::string& label, FishEngine::Matrix4x4& mat)
    {
        ImGui::InputFloat4((label + "##row1").c_str(), mat.rows[0].data());
        ImGui::InputFloat4((label + "##row2").c_str(), mat.rows[1].data());
        ImGui::InputFloat4((label + "##row3").c_str(), mat.rows[2].data());
        ImGui::InputFloat4((label + "##row4").c_str(), mat.rows[3].data());
    }

    void EditorGUI::SelectMeshDialogBox(std::function<void(std::shared_ptr<Mesh>)> callback)
    {
        //    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
        //    bool is_open = true;
        //    if (ImGui::BeginPopupModal("Select ...", &is_open, window_flags)) {
        //        for (auto m : Mesh::m_meshes) {
        //            if (ImGui::Button(m->name().c_str())) {
        //                Debug::Log("%s", m->name().c_str());
        //                //SetMesh(m);
        //                callback(m);
        //                ImGui::CloseCurrentPopup();
        //            }
        //        }
        //        ImGui::Separator();
        //        ImGui::EndPopup();
        //    }
    }

    void EditorGUI::OnWindowSizeChanged(const int width, const int height)
    {
        //CalculateWindowSizeAndPosition();
    }

    void EditorGUI::HierarchyItem(std::shared_ptr<GameObject> gameObject)
    {
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (gameObject == Selection::selectedGameObjectInHierarchy())
        {
            node_flags |= ImGuiTreeNodeFlags_Selected;
        }

        bool is_leaf = (gameObject->transform()->childCount() == 0);
        if (is_leaf)
        {
            // no children
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        }

        if (!gameObject->activeSelf())
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.20f, 0.20f, 0.20f, 1.00f));
        }

        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)m_idCount, node_flags, "%s", gameObject->name().c_str());

        if (ImGui::IsItemClicked())
        {
            if (!s_locked)
            {
                Selection::setActiveGameObject(gameObject);
            }
            Selection::setSelectedGameObjectInHierarchy(gameObject);
            s_isAnyItemClicked = true;
        }

        if (!s_openMenuPopup && ImGui::IsItemClicked(1))
        {
            if (!s_locked)
            {
                Selection::setActiveGameObject(gameObject);
            }
            Selection::setSelectedGameObjectInHierarchy(gameObject);
            s_isAnyItemClicked = true;
            Debug::LogWarning("Right button");
            ImGui::OpenPopup("HierarchyItem.menu");
            s_openMenuPopup = true;
        }


        // child node
        if (!is_leaf)
        {
            if (node_open)
            {
                for (auto t : gameObject->transform()->m_children)
                    HierarchyItem(t.lock()->gameObject());
                ImGui::TreePop();
            }
        }
        m_idCount++;
        if (!gameObject->activeSelf())
        {
            ImGui::PopStyleColor();
        }
    }

    void EditorGUI::DrawHierarchyWindow()
    {
        s_isAnyItemClicked = false;
        //auto selectedGO = Selection::activeGameObject();
        auto selectedGO = Selection::selectedGameObjectInHierarchy();
        // Hierarchy view
        //ImGui::BeginDock("Hierarchy");

        //if (s_windowResized)
        //{
        //    ImGui::SetNextWindowPos(hierarchyWindowPos);
        //    ImGui::SetNextWindowSize(hierarchyWindowSize);
        //}
        //ImGui::Begin("Hierarchy", nullptr, globalWindowFlags);
        ImGui::BeginDock("Hierarchy", nullptr);
        //static ImGuiTextFilter filter;
        //filter.Draw();
        static char filterStr[128];
        if (ImGui::InputText("Filter", filterStr, 127))
        {
            Debug::LogWarning("Filter changed");
        }


        if (ImGui::Button("Create"))
        {
            s_isAnyItemClicked = true;
            auto go = Scene::CreateGameObject("GameObject");
            if (Selection::selectedGameObjectInHierarchy() != nullptr)
            {
                go->transform()->SetParent(Selection::selectedGameObjectInHierarchy()->transform());
            }
        }

        m_idCount = 0;
        ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()); // Increase spacing to differentiate leaves from expanded contents.
        for (auto& go : Scene::m_gameObjects)
        {
            if (go->transform()->parent() == nullptr)
            {
                HierarchyItem(go);
            }
        }
        ImGui::PopStyleVar();

        if (s_openMenuPopup)
        {
            ImGui::OpenPopup("HierarchyItem.menu");
            s_openMenuPopup = false;
        }
        if (ImGui::BeginPopup("HierarchyItem.menu"))
        {
            ImGui::Selectable("Copy");
            ImGui::Selectable("Paste");
            ImGui::Separator();
            ImGui::Selectable("Rename");
            ImGui::Selectable("Duplicate");
            if (ImGui::Selectable("Delete") && selectedGO != nullptr)
            {
                Object::DestroyImmediate(selectedGO);
                s_openMenuPopup = false;
            }
            ImGui::Separator();
            ImGui::Selectable("Select Prefab");
            ImGui::Separator();
            ImGui::Selectable("Create Empty");
            ImGui::EndPopup();
        }

        if (ImGui::IsKeyDown(ImGuiKey_Delete))
        {
            Debug::LogWarning("delete");
        }

        // TODO: remove this
        if (!s_isAnyItemClicked && ImGui::IsMouseClicked(0) && ImGui::IsMouseHoveringWindow())
        {
            Selection::setSelectedGameObjectInHierarchy(nullptr);
            if (!s_locked)
                Selection::setActiveGameObject(nullptr);
            Selection::setSelectedGameObjectInHierarchy(nullptr);
        }

        ImGui::EndDock();
        //ImGui::End();
    }


    void EditorGUI::DrawInspectorWindow()
    {
        auto selectedGO = Selection::activeGameObject();

        // Inspector Editor
        //ImGui::BeginDock("Inspector", nullptr);

        //if (s_windowResized)
        //{
        //    ImGui::SetNextWindowPos(inspectorWindowPos);
        //    ImGui::SetNextWindowSize(inspectorWindowSize);
        //}
        ImGui::BeginDock("Inspector", nullptr);
        //ImGui::Begin("Inspector", nullptr, globalWindowFlags);
        ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.55f);
        if (selectedGO == nullptr) {
            ImGui::EndDock(); // Inspector Editor
            //ImGui::End();
            return;
        }
        if (ImGui::Checkbox("Lock", &s_locked)) {
            if (!s_locked)
                Selection::setActiveGameObject(Selection::selectedGameObjectInHierarchy());
        }
        ImGui::PushID("Inspector.selected.active");
        ImGui::Checkbox("", &selectedGO->m_activeSelf);
        ImGui::PopID();
        char name[128] = { 0 };
        memcpy(name, selectedGO->name().c_str(), selectedGO->name().size());
        name[selectedGO->m_name.size()] = 0;
        ImGui::SameLine();
        ImGui::PushID("Inspector.selected.name");
        if (ImGui::InputText("", name, 127)) {
            selectedGO->m_name = name;
        }
        ImGui::PopID();

        ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.3f);
        //ImGui::LabelText("", "Tag");
        ImGui::Text("Tag");
        ImGui::SameLine();
        ImGui::LabelText("##Tag", "%s", selectedGO->tag().c_str());
        ImGui::SameLine();
        ImGui::Text("Layer");
        ImGui::SameLine();
        ImGui::LabelText("##Layer", "Layer %d", selectedGO->m_layer);
        ImGui::PopItemWidth();

        if (ImGui::CollapsingHeader("Transform##header", ImGuiTreeNodeFlags_DefaultOpen)) {
            //selectedGO->m_transform->OnInspectorGUI();
            ImGui::Indent(inspector_indent_width);
            OnInspectorGUI<FishEngine::Transform>(std::static_pointer_cast<FishEngine::Transform>(selectedGO->m_transform));
            ImGui::Unindent(inspector_indent_width);
        }

        int local_id = 0;

        //auto headerName = [&local_id](const std::string& className) {
        //    return camelCaseToReadable(className) + "##header" + boost::lexical_cast<std::string>(local_id++);
        //};

        std::shared_ptr<Component> componentToBeDestroyed = nullptr;
        for (auto c : selectedGO->m_components)
        {
            ImGui::PushID(local_id++);
            bool is_open = true;
            if (ImGui::CollapsingHeader(c->ClassName().c_str(), &is_open, ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Indent(inspector_indent_width);
                OnInspectorGUI(c);
                ImGui::Unindent(inspector_indent_width);
            }
            if (!is_open)
            {
                //Object::DestroyImmediate(c);
                componentToBeDestroyed = c;
            }
            ImGui::PopID();
        }

        if (componentToBeDestroyed != nullptr)
        {
            Object::DestroyImmediate(componentToBeDestroyed);
        }


        std::shared_ptr<Script> scriptToBeDestroyed = nullptr;
        for (auto s : selectedGO->m_scripts)
        {
            ImGui::PushID(local_id++);
            bool is_open = true;
            if (ImGui::CollapsingHeader(s->ClassName().c_str(), &is_open, ImGuiTreeNodeFlags_DefaultOpen))
            {
                //OnInspectorGUI(s);
                ImGui::Indent(inspector_indent_width);
                s->OnInspectorGUI();
                ImGui::Unindent(inspector_indent_width);
            }
            if (!is_open)
            {
                //Object::DestroyImmediate(s);
                scriptToBeDestroyed = s;
            }
            ImGui::PopID();
        }

        if (scriptToBeDestroyed != nullptr)
        {
            Object::DestroyImmediate(scriptToBeDestroyed);
        }

        /************************************************************************/
        /*                          Add Component                               */
        /************************************************************************/
        if (selectedGO != nullptr)
        {
            if (EditorGUI::Button("Add Component"))
            {
                ImGui::OpenPopup("AddComponent");
                //auto comp = Component::CreateComponent("Camera");
                //selectedGO->AddComponent(comp);
            }

            static const char* names[] = {
                "Camera", "Animator", "MeshFilter", "MeshRenderer", "Rigidbody", "SkinnedMeshRenderer",
                "BoxCollider", "CapsuleCollider", "SphereCollider", "SphereCollider",
            };

            bool addComponentFailed = false;
            static const char* failedComponentName = nullptr;
            if (ImGui::BeginPopup("AddComponent"))
            {
                ImGui::Text("Components");
                ImGui::Separator();
                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                {
                    if (ImGui::Selectable(names[i]))
                    {
                        //auto comp = Component::CreateComponent(names[i]);
                        //selectedGO->AddComponent(comp);
                        if (nullptr == AddComponentToGameObject(names[i], selectedGO))
                        {
                            //ImGui::OpenPopup("SameComponentWarning");
                            failedComponentName = names[i];
                            addComponentFailed = true;
                            break;
                        }
                    }
                }
                ImGui::EndPopup();
            }

            if (addComponentFailed)
                ImGui::OpenPopup("SameComponentWarning");
            if (ImGui::BeginPopupModal("SameComponentWarning", nullptr,
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
            {
                ImGui::Text("The Component %s can't be added \n because %s already contains the same component.",
                    failedComponentName,
                    selectedGO->name().c_str());
                if (ImGui::Button("Cancel"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
        }

        ImGui::EndDock(); // Inspector Editor
        //ImGui::End();
    }


    void EditorGUI::DrawProjectWindow()
    {
        //if (s_windowResized)
        //{
        //    ImGui::SetNextWindowPos(projectWindowPos);
        //    ImGui::SetNextWindowSize(projectWindowSize);
        //}
        //ImGui::Begin("Project", nullptr, globalWindowFlags);
        ImGui::BeginDock("Project", nullptr);
        
        ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
        ImGui::BeginChild("Sub1", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.1f, 0));
        
        static int selected = -1;
        if (ImGui::Selectable("Textures", selected==0))
        {
            selected = 0;
        }
        if (ImGui::Selectable("Materials", selected==1))
        {
            selected = 1;
        }
        if (ImGui::Selectable("Shaders", selected==2))
        {
            selected = 2;
        }
        if (ImGui::Selectable("Scripts", selected==3))
        {
            selected = 3;
        }
        if (ImGui::Selectable("Models", selected==4))
        {
            selected = 4;
        }
        ImGui::EndChild();
        ImGui::PopStyleVar();
        
        ImGui::SameLine();
        
        
//        ImGui::BeginChild("Sub2", ImVec2(0,0), true);
//        //ImGui::Text("With border");
//        ImGui::Columns(2);
//        for (int i = 0; i < 100; i++)
//        {
//            if (i == 50)
//                ImGui::NextColumn();
//            char buf[32];
//            sprintf(buf, "%08x", i*5731);
//            ImGui::Button(buf, ImVec2(-1.0f, 0.0f));
//        }
//        ImGui::EndChild();
        
        ImGui::BeginChild("Sub2", ImVec2(0,0), true);
        if (selected == 2)
        {
            for (const auto& m : Shader::allShaders())
            {
                ImGui::Selectable(m.first.c_str());
            }
        }
        ImGui::EndChild();

        
        //ImGui::End();
        ImGui::EndDock();
    }

    void EditorGUI::DrawSceneView()
    {
        ImGui::BeginDock("Scene", nullptr, ImGuiWindowFlags_NoScrollWithMouse);
        m_mainSceneViewEditor->m_focused = ImGui::IsWindowFocused();
        m_mainSceneViewEditor->m_isMouseHovered = ImGui::IsWindowHovered();
        auto position = ImGui::GetCurrentWindow()->DC.CursorPos;
        m_mainSceneViewEditor->m_position.x = position.x;
        m_mainSceneViewEditor->m_position.y = position.y;
        auto size = ImGui::GetContentRegionAvail();
        m_sceneSize.x = static_cast<int>(size.x);
        m_sceneSize.y = static_cast<int>(size.y);
        //EditorInput::CopyToInput();
        m_mainSceneViewEditor->Render();
        auto& rt = m_mainSceneViewEditor->m_sceneViewRenderTexture;
        ImGui::Image((void*)rt->GLTexuture(), size, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndDock();
    }

    void EditorGUI::DrawMainMenu()
    {
        static float time_stamp = 0;

        float menu_height = 0;

        // Main menu bar
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Scene", "Ctrl+N"))
                {
                    //Debug::LogWarning("New");
                    //std::files
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                ImGui::EndMenu();
            }

            float new_time = (float)glfwGetTime();
            int fps = (int)roundf(1.f / float(new_time - time_stamp));
            time_stamp = new_time;
            std::string fps_str = "FPS: " + std::to_string(fps);
            auto fps_stats_size = ImGui::CalcTextSize(fps_str.c_str());
            ImGui::SameLine(ImGui::GetContentRegionMax().x - fps_stats_size.x);
            ImGui::Text("%s", fps_str.c_str());
            g_editorGUISettings.mainMenubarHeight = ImGui::GetWindowSize().y;
            ImGui::EndMainMenuBar();
        }
    }

    void EditorGUI::DrawMainToolbar()
    {
        auto frame_padding = ImGui::GetStyle().FramePadding;
        float padding = frame_padding.y * 2;
        float height = 24 + padding;
        ImVec2 toolbar_size(ImGui::GetIO().DisplaySize.x, height);
        //auto size = ImGui::GetIO().DisplaySize;
        if (ImGui::BeginToolbar("MainToolBar", ImVec2(1, g_editorGUISettings.mainMenubarHeight), toolbar_size))
        {
            ImGui::SameLine();
            if (FishEditorWindow::InPlayMode())
            {
                if (ImGui::Button("Stop"))
                {
                    FishEditorWindow::Stop();
                }
            }
            else
            {
                if (ImGui::Button("Play"))
                {
                    FishEditorWindow::Play();
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Translate"))
            {
                m_mainSceneViewEditor->m_transformToolType = TransformToolType::Translate;
            }

            ImGui::SameLine();
            if (ImGui::Button("Rotate"))
            {
                m_mainSceneViewEditor->m_transformToolType = TransformToolType::Rotate;
            }

            ImGui::SameLine();
            if (ImGui::Button("Scale"))
            {
                m_mainSceneViewEditor->m_transformToolType = TransformToolType::Scale;
            }

            ImGui::SameLine();
            if (ImGui::Button(ToString(m_mainSceneViewEditor->m_transformSpace)))
            {
                m_mainSceneViewEditor->m_transformSpace = m_mainSceneViewEditor->m_transformSpace == TransformSpace::Global ?
                    TransformSpace::Local : TransformSpace::Global;
            }
        }
        ImGui::EndToolbar();
        g_editorGUISettings.mainToolbarHeight = height;
    }

    // read-write
    bool Float(const char* label, float* value)
    {
        return ImGui::InputFloat(label, value);
    }

    bool Int(const char* label, int* value)
    {
        return ImGui::InputInt(label, value);
    }

    bool Bool(const char* label, bool* value)
    {
        return ImGui::Checkbox(label, value);
    }

    bool Float3(const char* label, Vector3* value)
    {
        return ImGui::InputFloat3(label, value->data());
    }

    // read-only version
    void Float(const char* label, float value)
    {
        ImGui::InputFloat(label, &value, 0.f, 0.f, -1, ImGuiInputTextFlags_ReadOnly);
    }

    void Int(const char* label, int value)
    {
        ImGui::InputInt(label, &value, 0, 0, ImGuiInputTextFlags_ReadOnly);
    }

    void Bool(const char* label, bool value)
    {
        ImGui::Checkbox(label, &value);
    }

    void Float3(const char* label, Vector3& value)
    {
        ImGui::InputFloat3(label, value.data(), -1, ImGuiInputTextFlags_ReadOnly);
    }
    
    template<typename T>
    void Enum(const char* label, const T& e)
    {
        int index = ToIndex(e);
        ImGui::Combo(label, &index, LightTypeStrings, EnumCount<T>());
    }
    
    template<typename T>
    void Enum(const char* label, T* e)
    {
        int index = ToIndex(*e);
        if (ImGui::Combo(label, &index, LightTypeStrings, EnumCount<T>()))
        {
            *e = ToEnum<T>(index);
        }
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::Transform>& transform)
    {
        if (Float3("Position", &transform->m_localPosition)) {
            transform->MakeDirty();
        }
        if (Float3("Rotation", &transform->m_localEulerAngles)) {
            transform->m_localRotation.setEulerAngles(transform->m_localEulerAngles);
            transform->MakeDirty();
        }
        if (Float3("Scale", &transform->m_localScale)) {
            transform->MakeDirty();
        }
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::Camera>& camera)
    {
        const char* listbox_items[] = {
            "Perspective", "Orthographic"
        };
        int list_item_current = camera->m_orthographic ? 1 : 0;
        ImGui::Combo("Projection", &list_item_current, listbox_items, 2);
        if (camera->m_orthographic != (list_item_current == 1)) {
            camera->setOrthographic(!camera->m_orthographic);
        }

        if (camera->m_orthographic) {
            if (Float("Size", &camera->m_orthographicSize)) {
                camera->m_isDirty = true;
            }
        }
        else {
            if (ImGui::SliderFloat("Field of View", &camera->m_fieldOfView, 1, 179)) {
                camera->m_isDirty = true;
            }
        }

        if (Float("Clipping Planes(Near)", &camera->m_nearClipPlane)) {
            camera->m_isDirty = true;
        }
        if (Float("Clipping Planes(Far)", &camera->m_farClipPlane)) {
            camera->m_isDirty = true;
        }
        ImGui::InputFloat4("Viewport Rect", camera->m_viewport.data());
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::Animator>& animator)
    {
        if (animator->m_animation != nullptr) {
            int channels = (int)animator->m_animation->channels.size();
            Int("Channel count", channels);
        }
        if (animator->m_playing) {
            if (ImGui::Button("Stop")) {
                animator->Stop();
            }
        }
        else {
            if (ImGui::Button("Play")) {
                animator->Play();
            }
            ImGui::SameLine();
            if (ImGui::Button("Play Once")) {
                animator->PlayOnce();
            }
        }

        if (ImGui::Button("Next Frame")) {
            animator->NextFrame();
        }
        Float("Time", &animator->m_time);
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::MeshFilter>& meshFilter)
    {
        if (ImGui::Button("Change")) {
            ImGui::OpenPopup("Select ...");
        }
        EditorGUI::SelectMeshDialogBox([&meshFilter](PMesh mesh)->void {
            meshFilter->SetMesh(mesh);
        });
        //ImGui::SameLine();
        ImGui::LabelText("Mesh", "%s", meshFilter->m_mesh->name().c_str());

        //bool skinned = meshFilter->m_mesh->m_skinned;
        //ImGui::Checkbox("Skinned", &skinned);
        if (meshFilter->m_mesh->m_skinned) {
            int boneCount = (int)meshFilter->m_mesh->m_boneNameToIndex.size();
            Int("Bone Count", boneCount);
        }
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<Material>& material)
    {
        if (ImGui::CollapsingHeader(material->m_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            auto& uniforms = material->m_shader->uniforms();
            for (auto& u : uniforms) {
                if (u.type == GL_FLOAT) {
                    ImGui::SliderFloat(u.name.c_str(), &material->m_uniforms.floats[u.name], 0, 1);
                }
                else if (u.type == GL_FLOAT_VEC3) {
                    ImGui::InputFloat3(u.name.c_str(), material->m_uniforms.vec3s[u.name].data());
                }
                else if (u.type == GL_FLOAT_VEC4) {
                    ImGui::InputFloat4(u.name.c_str(), material->m_uniforms.vec4s[u.name].data());
                }
                else if (u.type == GL_SAMPLER_2D || u.type == GL_SAMPLER_CUBE) {
                    auto& tex = material->m_textures[u.name];
                    ImGui::LabelText(u.name.c_str(), "%s", tex->name().c_str());
                    ImGui::Image((void*)tex->GLTexuture(), ImVec2(64, 64));
                    ImGui::SameLine();
                    ImGui::Button("Select");
                }
            }
            ImGui::Unindent();
        }
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<Renderer>& renderer)
    {
        if (ImGui::CollapsingHeader("Materials", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Indent(inspector_indent_width);
            Int("Size", (int)renderer->m_materials.size());
            for (auto& m : renderer->m_materials)
            {
                OnInspectorGUI(m);
            }
            ImGui::Unindent(inspector_indent_width);
        }
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::MeshRenderer>& renderer)
    {
        OnInspectorGUI<Renderer>(renderer);
    }


    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::SkinnedMeshRenderer>& renderer)
    {
        OnInspectorGUI<Renderer>(renderer);

        ImGui::LabelText("Mesh", "%s", renderer->m_sharedMesh->name().c_str());
        ImGui::LabelText("Root Bone", "%s", renderer->m_rootBone.lock()->name().c_str());

        ImGui::Text("Bounds");
        Bounds bounds = renderer->localBounds();
        auto center = bounds.center();
        auto extents = bounds.extents();
        Float3("Center", center);
        Float3("Extents", extents);
    }


    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::Rigidbody>& rigidBody)
    {
        Float("Mass", &rigidBody->m_mass);
        Float("Drag", &rigidBody->m_drag);
        Float("Angular", &rigidBody->m_angularDrag);
        Bool("Use Gravity", &rigidBody->m_useGravity);
        Bool("Is Kinematic", &rigidBody->m_isKinematic);
    }


    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::BoxCollider>& boxCollider)
    {
        Bool("Is Trigger", &boxCollider->m_isTrigger);
        Float3("Center", &boxCollider->m_center);
        Float3("Size", &boxCollider->m_size);
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::SphereCollider>& sphereCollider)
    {
        Bool("Is Trigger", &sphereCollider->m_isTrigger);
        Float3("Center", &sphereCollider->m_center);
        Float("Radius", &sphereCollider->m_radius);
    }


    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::CapsuleCollider>& capsuleCollider)
    {
        Bool("Is Trigger", &capsuleCollider->m_isTrigger);
        Float3("Center", &capsuleCollider->m_center);
        Float("Radius", &capsuleCollider->m_radius);
        Float("Height", &capsuleCollider->m_height);
        //ImGui::InputFloat3("Direction", capsuleCollider->m_direction);
        const char* listbox_items[] = {
            "X-Axis", "Y-Axis", "Z-Axis"
        };
        ImGui::Combo("Direction", &capsuleCollider->m_direction, listbox_items, 3);
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::Light>& light)
    {
        Enum<LightType>("Type", &light->m_type);
        ImGui::ColorEdit4("Color", light->m_color.m);
        Float("Intensity", &light->m_intensity);
        ImGui::DragFloat("Range", &light->m_range);
        Float("Bias", &light->m_shadowBias);
        Float("Normal Bias", &light->m_shadowNormalBias);
        Float("Shadow Near Plane", &light->m_shadowNearPlane);
    }

    template<>
    void EditorGUI::OnInspectorGUI(const std::shared_ptr<FishEngine::Component>& component)
    {
#define Case(T) else if (component->ClassName() == FishEngine::T::StaticClassName()) { OnInspectorGUI(std::static_pointer_cast<FishEngine::T>(component)); }

        if (component->ClassName() == FishEngine::Transform::StaticClassName()) {
            OnInspectorGUI(std::static_pointer_cast<FishEngine::Transform>(component));
        }
        Case(Camera)
        Case(Animator)
        Case(MeshFilter)
        Case(MeshRenderer)
        Case(BoxCollider)
        Case(SphereCollider)
        Case(CapsuleCollider)
        Case(Rigidbody)
        Case(SkinnedMeshRenderer)
        Case(Light)
#undef Case
    }
    
    FishEngine::Int2 EditorGUI::sceneViewSize()
    {
        return m_sceneSize;
    }
}
