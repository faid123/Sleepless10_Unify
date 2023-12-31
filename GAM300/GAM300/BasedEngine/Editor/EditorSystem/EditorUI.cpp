#include "EditorUI.h"
#include <iostream>
#include <Entity/Entity.h>
#include "Engine.h"
#include <Serializer/SerializerSystem.h>
#include <Graphics/DDSLoader/ModelLoader.h>
#include <Graphics/Graphics/Graphics.h>
#include <Graphics/RenderSystem/RenderSystem.h>
//#include <Graphics/TransformCom.h>
//#include <Physics/RigidBodyComponent.h>
//#include <Camera/CameraCom.h>
//#include "Physics/PhysicsSystem.h"
//#include <Camera/CameraSystem/CameraSystem.h>
#include "Editor/EditorSystem/EditorSystem.h"
#include "Animation/AnimationSystem.h"
#include "Animation/AnimationCom.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include<glm/common.hpp>
#include <Audio/AudioSrcCom.h>
#include <ResourceManager/ResourceManager.h>
#include "GameState.h"
#include <Graphics/TextCom.h>

EntityID entity_ext_id = 0;
EntityID path_ext_id = 0;
EntityID parenting_id = 0;
PrefabID prefab_ext_id = 0;

static char temp_name[1024] = { "empty" };
static std::string compTypeName;
char entityName[64];
char tagName[64];
const char* _stateType[3] = { "None", "Static", "Dynamic" };
static const char* current_item;

// for waypoint
//std::map<std::string, Vector3> WayPoint; // declare WayPoint map
char point_name[64]; // to set the point name
std::vector<EntityID> tileID;

// for prefabs
static bool found_prefab = false;
static std::string propEntryName;
static property::entry property_entry;

static bool isChild = false;
// end of prefabs

//auto updatePrefabsValues = [](std::vector<EntityID>* entity_vector, std::string PropEntryName, property::entry propEntry)
//{
//	std::vector<std::shared_ptr<Component>> comps;
//	std::vector<std::string> names;
//	std::string component_name;
//	std::string substr = "/";
//	std::string::size_type j = propEntry.first.find(substr);
//	component_name = propEntry.first;
//	if (!propEntry.first.empty())
//		component_name = component_name.erase(j);
//
//	for (auto vec : *entity_vector)
//	{
//		std::cout << "EntityID: " << vec << std::endl;
//		Engine::_ecs.getEntityComponentPtrAndName(vec, names, comps);
//		int i = 0;
//		for (auto& E : comps)
//		{
//			if (names[i] == component_name) // change compName
//			{
//				std::cout << "component_name: " + component_name << std::endl;
//				std::cout << "names[i]: " + names[i] << std::endl;
//				std::cout << "propEntry.first: " + propEntry.first << std::endl;
//				property::set(*E, propEntry.first.c_str(), propEntry.second);
//				return;
//				++i;
//			}
//		}
//
//		/*std::shared_ptr<TransformCom> E;
//		if (compName == "TransformCom")
//			 E = Engine::_ecs.getComponent<TransformCom>(vec);*/
//
//		//property::set(*E, PropEntryName.c_str(), Data);
//	}
//};

auto comboTextureLambda = [](const char* propertyName, std::string name)
{
	auto graphics = Engine::_ecs.getSystem<RenderSystem>();
	std::string selected_item;
	static bool is_selected = false;

	if (ImGui::BeginCombo(propertyName, name.c_str()))
	{
		auto textureVec = graphics->GetTextures();
		auto it = textureVec.begin();
		for (; it != textureVec.end(); ++it)
		{
			if (ImGui::Selectable(it->c_str(), &is_selected))
			{
				selected_item = it->c_str();
				break;
			}
		}
		ImGui::EndCombo();
	}
	if (is_selected)
	{
		auto material = Engine::_ecs.getComponent<MaterialCom>(entity_ext_id);
		material->SetTexture(selected_item);
		is_selected = false;
		return selected_item;
	}
	else
		return name;
};
auto comboSpriteLambda = [](const char* propertyName, std::string name)
{
	auto graphics = Engine::_ecs.getSystem<RenderSystem>();
	std::string selected_item;
	static bool is_selected = false;

	if (ImGui::BeginCombo(propertyName, name.c_str()))
	{
		auto textureVec = graphics->GetTextures();
		auto it = textureVec.begin();
		for (; it != textureVec.end(); ++it)
		{
			if (ImGui::Selectable(it->c_str(), &is_selected))
			{
				selected_item = it->c_str();
				break;
			}
		}
		ImGui::EndCombo();
	}
	if (is_selected)
	{
		auto sprite = Engine::_ecs.getComponent<SpriteCom>(entity_ext_id);
		sprite->SpriteName = selected_item;
		is_selected = false;
		return selected_item;
	}
	else
		return name;
};

auto comboModelLambda = [](const char* propertyName, std::string name)
{
	auto graphics = Engine::_ecs.getSystem<RenderSystem>();
	std::string selected_item;
	static bool is_selected = false;

	if (ImGui::BeginCombo(propertyName, name.c_str()))
	{
		auto modelVec = graphics->GetModels();
		auto it = modelVec.begin();
		for (; it != modelVec.end(); ++it)
		{
			if (ImGui::Selectable(it->c_str(), &is_selected))
			{
				selected_item = it->c_str();
				break;
			}
		}
		ImGui::EndCombo();
	}
	if (is_selected)
	{
		auto material = Engine::_ecs.getComponent<MaterialCom>(entity_ext_id);
		material->SetModel(selected_item);
		is_selected = false;
		return selected_item;
	}
	else
		return name;
};

auto comboShaderLambda = [](const char* propertyName, std::string name)
{
	auto graphics = Engine::_ecs.getSystem<RenderSystem>();
	std::string selected_item;
	static bool is_selected = false;

	if (ImGui::BeginCombo(propertyName, name.c_str()))
	{
		auto shaderVec = graphics->GetShaders();
		auto it = shaderVec.begin();
		for (; it != shaderVec.end(); ++it)
		{
			if (ImGui::Selectable(it->c_str(), &is_selected))
			{
				selected_item = it->c_str();
				break;
			}
		}
		ImGui::EndCombo();
	}
	if (is_selected)
	{
		auto material = Engine::_ecs.getComponent<MaterialCom>(entity_ext_id);
		material->SetShader(selected_item);
		is_selected = false;
		return selected_item;
	}
	else
		return name;
};

auto comboSelectionLambda = [](const char* propertyName, std::string name)
{
	std::string propName = propertyName;
	if (propName == "model")
		return comboModelLambda(propertyName, name);
	else if (propName == "texture")
		return comboTextureLambda(propertyName, name);
	else if (propName == "shader")
		return comboShaderLambda(propertyName, name);
	else if (propName == "SpriteName")
		return comboSpriteLambda(propertyName, name);
	return comboModelLambda(propertyName, name);
};

auto comboDropScript = [](const char* propertyName, std::string name)
{
	std::string selected_item;
	static bool is_selected = false;

	if (ImGui::BeginCombo(propertyName, name.c_str()))
	{
		auto scriptVec = RESOURCE->GetScriptVec();
		auto it = scriptVec.begin();
		for (; it != scriptVec.end(); ++it)
		{
			if (ImGui::Selectable(it->c_str(), &is_selected))
			{
				selected_item = it->c_str();
				break;
			}
		}
		ImGui::EndCombo();
	}
	if (is_selected)
	{
		is_selected = false;
		return selected_item;
	}
	else
		return name;
};

auto comboLambdaPrefab = [](const char* propertyName, std::string name, std::vector<std::string> vec)
{
	std::string selected_item;
	static bool is_selected = false;
	if (ImGui::BeginCombo(propertyName, name.c_str()))
	{
		auto it = vec.begin();
		for (; it != vec.end(); ++it)
		{
			if (ImGui::Selectable(it->c_str(), &is_selected))
			{
				selected_item = it->c_str();
				break;
			}
		}
		ImGui::EndCombo();
	}
	if (is_selected)
	{
		is_selected = false;
		return selected_item;
	}
	else
		return name;
};

auto comboSelectionPrefab = [](const char* propertyName, std::string name)
{
	std::string propName = propertyName;
	auto graphics = Engine::_ecs.getSystem<RenderSystem>();
	if (propName == "model")
		return comboLambdaPrefab(propertyName, name, graphics->GetModels());
	else if (propName == "texture")
		return comboLambdaPrefab(propertyName, name, graphics->GetTextures());
	else if (propName == "shader")
		return comboLambdaPrefab(propertyName, name, graphics->GetShaders());
	return comboLambdaPrefab(propertyName, name, graphics->GetModels());
};

void EditorUI::showTopMenu()
{
	bool open = false;
	bool save = false;
	bool saveas = false;

	// place the menu bar on top
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("New", NULL))
			{
				auto serializer = Engine::_ecs.getSystem<SerializerSystem>();
				serializer->setFileName("default.scene");
				serializer->setIsLoaded(true);
			}
			if (ImGui::MenuItem("Open", NULL))
				open = true;
			if (ImGui::MenuItem("Save", NULL))
				save = true;
			if (ImGui::MenuItem("Save As", NULL))
				saveas = true;

			ImGui::EndMenu();
		}
		/* show current scene name at top bar menu */
		std::string sceneName = Engine::_ecs.getSystem<SerializerSystem>()->getFileName();
		std::string formatting = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
		formatting += sceneName;
		ImGui::TextUnformatted(formatting.c_str());

		ImGui::EndMainMenuBar();
	}

	//Remember the name to ImGui::OpenPopup() and showFileDialog() must be same...
	if (open)
		ImGui::OpenPopup("Open File");
	else if (saveas)
		ImGui::OpenPopup("Save File");
	else if (save)
	{
		/* save the current .scene file without opening file explorer */
		std::string sceneName = Engine::_ecs.getSystem<SerializerSystem>()->getFileName();
		auto serializer = Engine::_ecs.getSystem<SerializerSystem>();
		serializer->setFileName(sceneName);
		serializer->setIsSaved(true);
	}

	if (file_dialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".scene,.json,.txt,*.*"))
	{
		std::cout << file_dialog.selected_fn << std::endl;      // The name of the selected file or directory in case of Select Directory dialog mode
		std::cout << file_dialog.selected_path << std::endl;    // The absolute path to the selected file
		auto serializer = Engine::_ecs.getSystem<SerializerSystem>();
		serializer->setFileName(file_dialog.selected_fn);
		serializer->setIsLoaded(true);
	}
	else if (file_dialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".scene,.json,.txt,*.*"))
	{
		// find ".ext in filename and remove it"
		std::string toErase = ".scene";
		size_t pos = file_dialog.selected_fn.find(toErase);
		if (pos != std::string::npos)
		{
			// If found then erase it from string
			file_dialog.selected_fn.erase(pos, toErase.length());
		}
		std::cout << file_dialog.selected_fn << std::endl;      // The name of the selected file or directory in case of Select Directory dialog mode
		std::cout << file_dialog.selected_path << std::endl;    // The absolute path to the selected file
		auto serializer = Engine::_ecs.getSystem<SerializerSystem>();
		serializer->setFileName(file_dialog.selected_fn + file_dialog.ext);
		serializer->setIsSaved(true);
	}
}

void EditorUI::showFps()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Begin("Scene");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / io.Framerate, io.Framerate);
	ImGui::End();
}

void EditorUI::showEntity()
{
	//printf("Testing: %s\n", property::getTableByType<CameraCom>().m_pName);
	//printf("Testing: %s\n", property::getTableByType<TransformCom>().m_pName);
	//printf("Testing: %s\n", property::getTableByType<MaterialCom>().m_pName);
	//printf("Testing: %s\n", property::getTableByType<RigidDynamicCom>().m_pName);
	//printf("Testing: %s\n", property::getTableByType<RigidStaticCom>().m_pName);


	// property windows
	ImGui::Begin("Hierarchy", NULL, ImGuiWindowFlags_MenuBar);
	//static ImVec2 w_position = ImGui::GetWindowPos();
	//static ImVec2 w_size = ImGui::GetWindowSize();
	//ImGui::SetWindowPos({ 5.0f, 70.0f });
	//ImGui::SetWindowSize({ 350.0f, 350.0f });
	auto entities = Engine::_ecs.getActiveEntities();

	// List of total Entities
	//ImGui::Text("List of %d/%d Entities:", entities.size(), MAX_ENTITIES);
	if (ImGui::BeginMenuBar())
	{

		std::string entity = "";
		if (ImGui::BeginMenu("New"))
		{
			if (ImGui::MenuItem("Add new Entity"))
			{
				entity = "Add Entity";
			}
			ImGui::EndMenu();
		}
		else if (ImGui::BeginMenu("Prefab"))
		{
			if (ImGui::MenuItem("Save as prefab"))
			{
				std::string filePath = "Resources/Prefabs/";
				std::string fileName = Engine::_ecs.getEntityName(entity_ext_id);
				if (SerializePrefabToFile((filePath + fileName + ".prefab"), fileName))
				{
					std::cout << "SUCCESS - Saved Entity: " + fileName + " as prefab" << std::endl;
					RESOURCE->setNewPrefab(filePath + fileName + ".prefab");
					RESOURCE->setIsRefresh(true);
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
		if (entity == "Add Entity")
			ImGui::OpenPopup("Add Entity");
	}

	if (ImGui::BeginPopupModal("Add Entity", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		showCreateEntity();
		ImGui::EndPopup();
	}


	//if (ImGui::ListBoxHeader(""))
	//{
		// begin map iterator it getActiveEntities()
		//auto it = entities.begin();
		//static bool item_selected = false;
		// loop through entity list
	//for (auto& it : entities)
	//{
	//	auto tag = it._tags.find("Particles");
	//	if (tag == it._tags.end())
	//	{
	//		if (ImGui::Selectable(it._entityName.c_str(), entity_ext_id == it._entityID))
	//		{
	//			entity_ext_id = it._entityID;
	//		}
	//	}
	//}

	// ENTITY LISTING
	if (ImGui::TreeNode("Entity tree"))
	{
		for (auto& it : entities)
		{
			auto tag = it._tags.find("Particles");
			if (tag == it._tags.end())
			{
				std::set<EntityID> childIDs = Engine::_ecs.getChildEntities(it._entityID);
				// check if it is a child, if it is not a child list normally
				// check if not parent so list normally
				if (it._parentID == -1 && childIDs.empty())
				{
					if (ImGui::Selectable(it._entityName.c_str(), entity_ext_id == it._entityID))
					{
						entity_ext_id = it._entityID;
					}
				}
				else if (!childIDs.empty()) // list down the parent and child from the childentities list
				{
					// display parent
					ImGui::Unindent();
					ImGui::Bullet();
					ImGui::Indent();
					if (ImGui::Selectable(it._entityName.c_str(), entity_ext_id == it._entityID))
					{
						entity_ext_id = it._entityID;
					}
					// display children
					ImGui::Indent();
					for (auto child : childIDs)
					{
						//Entity entity = Engine::_ecs.getEntity(child);
						Entity childEntity = Engine::_ecs.getEntity(child);
						auto childtag = childEntity._tags.find("Particles");
						if (childtag == childEntity._tags.end())
						{
							std::string entity_name = Engine::_ecs.getEntityName(child);
							if (ImGui::Selectable(entity_name.c_str(), entity_ext_id == child))
							{
								entity_ext_id = child;
							}
						}
					}
					ImGui::Unindent();
				}
			}
		}

		ImGui::TreePop();
	}


	//	ImGui::ListBoxFooter();
	//}

	ImGui::End();
}

void EditorUI::showPrefab()
{
	static bool check = false;
	static int qty = 0;
	ImGui::Begin("Prefabs");

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 255, 0)));
	if (ImGui::Button("Add Selected Prefab"))
	{
		/* remove the "Prefab_" initial */
		std::string prefab_name = Engine::_ecs.getPrefabName(prefab_ext_id);
		std::string toErase = "Prefab_";
		size_t pos = prefab_name.find(toErase);
		if (pos != std::string::npos)
		{
			// If found then erase it from string
			prefab_name.erase(pos, toErase.length());
		}
		if (qty > 1)
		{
			EntityID parent_id = 0;
			std::vector<EntityID> entity_vec;
			EntityID child_id = 0;
			for (int i = 0; i < qty; ++i)
			{
				//std::string prefab_name = Engine::_ecs.getPrefabName(prefab_ext_id);
				EntityID id = Engine::_ecs.instantiatePrefab(prefab_ext_id, prefab_name);
				if (id >= 0)
				{
					if (i == 0)
					{
						parent_id = id;
						Engine::_ecs.changeEntityName(prefab_name, "P_" + prefab_name);
					}
					else
					{
						entity_vec.push_back(id);
						child_id = id;
						Engine::_ecs.changeEntityName(prefab_name, "C_" + prefab_name + std::to_string(i));
						Engine::_ecs.setEntityParent(child_id, parent_id);
					}
					std::cout << "Parent ID: " + std::to_string(parent_id) + "\n" + "Child ID: " + std::to_string(child_id) << std::endl;
				}
			}
			auto childSet = Engine::_ecs.getChildEntities(parent_id);
			for (auto child : childSet)
			{
				std::cout << "Child ID Set: " + std::to_string(child) << std::endl;
			}
			Engine::_ecs.setPrefabParentToChild(parent_id, entity_vec);
		}
		// Instantiate the prefab onto the scene
		EntityID entityID = Engine::_ecs.instantiatePrefab(prefab_ext_id, prefab_name);
		if (entityID >= 0)
		{
			if (prefab_name == "Text")
			{
				RESOURCE->insertTextMap(entityID, 0);
				RESOURCE->textID = entityID;
				RESOURCE->isAddText = true;
			}
			std::cout << "PREFAB INSTANTIATION SUCCESS!" << std::endl;
			std::cout << "PrefabID: " + std::to_string(prefab_ext_id)
				+ "\t" + "PrefabName: " + Engine::_ecs.getPrefabName(prefab_ext_id)
				<< std::endl;
		}
	}
	ImGui::PopStyleColor();

	if (ImGui::Checkbox("Create multiple?", &check))
	{
		std::cout << "checkbox test" << std::endl;
	}
	if (check)
	{
		ImGui::DragInt("qty", &qty);
	}

	for (auto prefabVec : Engine::_ecs.getActivePrefabs())
	{
		if (ImGui::Selectable(prefabVec._prefabName.c_str(), prefab_ext_id == prefabVec._prefabID))
		{
			prefab_ext_id = prefabVec._prefabID;
			std::cout << "SELECTED PREFAB ID: " << std::to_string(prefab_ext_id) + "\n"
				<< "SELECTED PREFAB NAME: " << prefabVec._prefabName << std::endl;
		}
	}

	//PREFABMAP::iterator it = RESOURCE->GetPrefabMap()->begin();
	//for (; it != RESOURCE->GetPrefabMap()->end(); it++)
	//{
	//	if (ImGui::Selectable(it->second._prefabName.c_str(), prefab_ext_id == it->second._prefabID))
	//	{
	//		prefab_ext_id = it->second._prefabID;
	//		//std::cout << "SELECTED PREFAB ID: " << std::to_string(prefab_ext_id) + "\n"
	//		//	<< "SELECTED PREFAB NAME: " << it->second._prefabName << std::endl;
	//	}
	//}

	ImGui::End();
}

void EditorUI::showProperty()
{
	if (ImGui::Begin("Property Inspector"))
	{
		showComponents();
		std::vector<std::string> selectedEntityCompsNames;
		std::vector<std::shared_ptr<Component>> selectedEntityComps;

		if (Engine::_ecs.hasEntity(entity_ext_id))
		{
			Engine::_ecs.getEntityComponentPtrAndName(entity_ext_id, selectedEntityCompsNames, selectedEntityComps);
			//if (Engine::_ecs.hasEntity(entity_ext_id))
			//{

			std::string oldEntityName = Engine::_ecs.getEntityName(entity_ext_id);
			ImGui::PushItemWidth(120);
			ImGui::InputText("Entity Name", entityName, IM_ARRAYSIZE(entityName));
			if (ImGui::Button("Rename"))
			{
				Engine::_ecs.changeEntityName(oldEntityName, entityName);
			}
			ImGui::Separator();
			ImGui::PopItemWidth();
			//}
			int i = 0;
			std::string ComponentName;
			for (auto& E : selectedEntityComps)
			{
				ComponentName = selectedEntityCompsNames[i++];
				//ImGui::Text(ComponentName.c_str());

				property::entry PropEntry;
				if (ImGui::CollapsingHeader(ComponentName.c_str()))
				{
					property::DisplayEnum(*E, [&](std::string_view PropertyName, property::data&& Data, const property::table& table, std::size_t, property::flags::type Flags)
						{
							(void)Flags; (void)table;
							// Get properties name
							std::string Property;

							if (PropertyName.find('/' != std::string::npos))
							{
								Property = PropertyName.substr(PropertyName.find_last_of('/') + 1);
							}
							propEntryName = PropEntry.first = PropertyName;
							std::string second;
							std::visit([&](auto&& Value)
								{
									using T = std::decay_t<decltype(Value)>;

									if constexpr (std::is_same_v<T, int>)
									{
										if (ComponentName == "TransformCom")
										{
											T n = Value;
											ImGui::Combo(Property.c_str(), &n, _stateType, IM_ARRAYSIZE(_stateType));
											PropEntry.second = n;

										}
										else if (ComponentName == "LightCom")
										{
											T n = Value;
											ImGui::DragInt(Property.c_str(), &n, 0.01f);
											PropEntry.second = n;

											if (table.m_pEntry->m_pHelp)
											{
												ImGui::SameLine();
												ShowHelpMarker(table.m_pEntry->m_pHelp);
											}
										}
										else
										{
											T n = Value;
											ImGui::DragInt(Property.c_str(), &n, 0.01f);
											PropEntry.second = n;
										}
									}
									else if constexpr (std::is_same_v<T, float>)
									{
										T n = Value;
										ImGui::DragFloat(Property.c_str(), &n, 0.01f);
										PropEntry.second = n;
									}
									else if constexpr (std::is_same_v <T, uint32_t>)
									{
										T n = Value;
										int32_t x = (int32_t)n;
										ImGui::DragInt(Property.c_str(), &x, 0.01f);
										PropEntry.second = n;
									}
									else if constexpr (std::is_same_v<T, bool>)
									{
										T check = Value;
										ImGui::Checkbox(Property.c_str(), &check);
										PropEntry.second = check;
									}
									else if constexpr (std::is_same_v<T, string_t>)
									{
										if (ComponentName == "MaterialCom")
										{
											std::string text = Value;
											std::string newVal;
											// model, shader, texture
											newVal = comboSelectionLambda(Property.c_str(), text);
											second = newVal;
											PropEntry.second = newVal;
										}
										else if (ComponentName == "SpriteCom")
										{
											std::string text = Value;
											std::string newVal;
											// sprite name
											newVal = comboSelectionLambda(Property.c_str(), text);
											second = newVal;
											PropEntry.second = newVal;
										}
										else if (ComponentName == "uaScriptCom")
										{
											std::string text = Value;
											std::string newVal;
											newVal = comboDropScript(Property.c_str(), text);
											second = newVal;
											PropEntry.second = newVal;
										}
										else if (ComponentName == "TextCom")
										{
											T text = Value;
											ImGui::InputTextMultiline(Property.c_str(), text.data(), 10000);
											second = text;
											PropEntry.second = text;
										}
										else
										{
											T text = Value;
											ImGui::InputText(Property.c_str(), text.data(), 256);
											second = text;
											PropEntry.second = text;
										}
									}
									else if constexpr (std::is_same_v<T, oobb>)
									{
										// do not use
									}
									else if constexpr (std::is_same_v<T, Vector2>)
									{
										Vector2 vector = Value;
										ImGui::DragFloat2(Property.c_str(), &vector.x, 0.01f);
										PropEntry.second = vector;
									}
									else if constexpr (std::is_same_v<T, Vector3>)
									{
										Vector3 vector = Value;
										ImGui::DragFloat3(Property.c_str(), &vector.x, 0.01f);
										PropEntry.second = vector;
									}
									else if constexpr (std::is_same_v<T, Vector4>)
									{
										Vector4 vector = Value;
										ImGui::DragFloat4(Property.c_str(), &vector.x, 0.01f);
										PropEntry.second = vector;
									}
									else if constexpr (std::is_same_v<T, Matrix4>)
									{
										Matrix4 mat4 = Value;
										ImGui::DragFloat4(Property.c_str(), &mat4._m[0][0]);
										ImGui::DragFloat4(Property.c_str(), &mat4._m[1][0]);
										ImGui::DragFloat4(Property.c_str(), &mat4._m[2][0]);
										ImGui::DragFloat4(Property.c_str(), &mat4._m[3][0]);
										PropEntry.second = mat4;
									}
									else if constexpr (std::is_same_v<T, Quat>)
									{
										Quat quat = Value;
										ImGui::DragFloat4(Property.c_str(), &quat.x, 0.01f);
										PropEntry.second = quat;
									}
									else if constexpr (std::is_same_v<T, glm::vec3>)
									{
										glm::vec3 vector = Value;
										ImGui::DragFloat3(Property.c_str(), &vector.x, 0.01f);
										PropEntry.second = vector;
									}
									else assert(always_false<T>::value);

								}, Data);

							//property_entry.first = PropEntry.first;
							//property_entry.second = PropEntry.second;
							updateParentChildProperty(PropEntry.first, PropEntry.second, second);
							property::set(*E, PropEntry.first.c_str(), PropEntry.second);
							/*if (found_prefab)
							{
								updatePrefabsValues(entity_vector, ComponentName, PropEntry.first, PropEntry.second);
								entity_vector.clear();
								found_prefab = false;
							}	*/
						});

					/*if (ImGui::Button("Add New Body"))
					{
						auto transform = Engine::_ecs.getComponent<TransformCom>(entity_ext_id);
						auto staticbody = Engine::_ecs.getComponent<RigidStaticCom>(entity_ext_id);
						Engine::_ecs.getSystem<PhysicsSystem>()->AddRigidStaticBody(entity_ext_id, *staticbody, *transform);
					}*/

					if (ComponentName == "AnimationCom")
					{
						int n = Engine::_ecs.getSystem<AnimationSystem>()->allAnimators[entity_ext_id]->totalAnimations;
						std::string name = "Total Animations: " + std::to_string(n);
						ImGui::Text(name.c_str());
					}

					if (ImGui::Button("Delete Component", ImVec2(120.0f, 0.0f)))
					{
						if (ComponentName == "TransformCom")
							Engine::_ecs.removeComponentFromEntity<TransformCom>(entity_ext_id);
						else if (ComponentName == "CameraCom")
							Engine::_ecs.removeComponentFromEntity<CameraCom>(entity_ext_id);
						else if (ComponentName == "LightCom")
							Engine::_ecs.removeComponentFromEntity<LightCom>(entity_ext_id);
						else if (ComponentName == "MaterialCom")
							Engine::_ecs.removeComponentFromEntity<MaterialCom>(entity_ext_id);
						else if (ComponentName == "RigidDynamicCom")
						{
							auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(entity_ext_id);
							Engine::_ecs.getSystem<PhysicsSystem>()->RemoveActor(*dynamicbody->body);
							Engine::_ecs.removeComponentFromEntity<RigidDynamicCom>(entity_ext_id);
						}

						else if (ComponentName == "RigidStaticCom")
						{
							auto staticbody = Engine::_ecs.getComponent<RigidStaticCom>(entity_ext_id);
							Engine::_ecs.getSystem<PhysicsSystem>()->RemoveActor(*staticbody->body);
							Engine::_ecs.removeComponentFromEntity<RigidStaticCom>(entity_ext_id);
						}

						else if (ComponentName == "ParticleCom")
							Engine::_ecs.removeComponentFromEntity<ParticleCom>(entity_ext_id);
						else if (ComponentName == "EmitterCom")
						{
							auto _emitter = Engine::_ecs.getComponent<EmitterCom>(entity_ext_id);
							/*for (auto particle : _emitter->particles)
								Engine::_ecs.destroyEntity(particle);*/
							Engine::_ecs.removeComponentFromEntity<EmitterCom>(entity_ext_id);
						}
						else if (ComponentName == "AudioSrcCom")
							Engine::_ecs.removeComponentFromEntity<AudioSrcCom>(entity_ext_id);
						else if (ComponentName == "uaScriptCom")
							Engine::_ecs.removeComponentFromEntity<LuaScriptCom>(entity_ext_id);
						else if (ComponentName == "AnimationCom")
							Engine::_ecs.removeComponentFromEntity<AnimationCom>(entity_ext_id);
						else if (ComponentName == "TextCom")
							Engine::_ecs.removeComponentFromEntity<TextCom>(entity_ext_id);
					}
				}


			}
		}
	}
	ImGui::End();

}

void EditorUI::showParenting()
{
	if (ImGui::Begin("Parenting"))
	{
		auto entities = Engine::_ecs.getActiveEntities();
		ImGui::Text("Entities");
		if (ImGui::ListBoxHeader("", ImVec2(ImGui::GetWindowWidth() * 0.8f, ImGui::GetWindowHeight() * 0.6f)))
		{
			for (auto& it : entities)
			{
				auto tag = it._tags.find("Particles");
				if (tag == it._tags.end())
				{
					if (ImGui::Selectable(it._entityName.c_str(), parenting_id == it._entityID))
					{
						parenting_id = it._entityID;
					}
				}
			}
			ImGui::ListBoxFooter();
		}

		if (ImGui::Button("remove child/parent"))
		{
			// TODO: check if it is an existing child to a parent, if so removeEntityParent()
			Entity entity = Engine::_ecs.getEntity(parenting_id);
			if (entity._parentID != -1)
			{
				std::cout << "Removing parentID: " << std::to_string(entity._parentID) << std::endl;
				Engine::_ecs.removeEntityParent(parenting_id);
				std::cout << "Removed parent!" << std::endl;
			}
			else if (!Engine::_ecs.getChildEntities(parenting_id).empty())
			{
				std::cout << "Removing all child from parentID: " << std::to_string(parenting_id) << std::endl;
				Engine::_ecs.removeEntityChild(parenting_id);
				std::cout << "Removed all child!" << std::endl;
			}
			else
			{
				std::cout << "Entity is not a child or parent!" << std::endl;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("make child"))
		{
			if (parenting_id != entity_ext_id)
			{
				// TODO: check if it is an existing child to a parent, if so removeEntityParent()
				Entity entity = Engine::_ecs.getEntity(parenting_id);
				if (entity._parentID >= 0)
				{
					std::cout << "Removing parentID: " << std::to_string(entity._parentID) << std::endl;
					Engine::_ecs.removeEntityParent(parenting_id);
					std::cout << "Removed parent!" << std::endl;
				}
				Engine::_ecs.setEntityParent(parenting_id, entity_ext_id);
				std::cout << "EntityID: " + std::to_string(parenting_id) + " made child to EntityID: " + std::to_string(entity_ext_id) << std::endl;
			}
			else
			{
				std::cout << "ERROR!: Cannot child the same entity!" << std::endl;
			}
		}

		// PRINT THE TEXT INFO ABOUT PARENTING

		static bool isChildren = false;
		static bool isParent = false;
		std::set<EntityID> childIDs = Engine::_ecs.getChildEntities(entity_ext_id);
		if (childIDs.empty())
		{
			isParent = false; isChildren = false;
		}
		else
		{
			isParent = true;
			for (auto child : childIDs)
			{
				if (child == parenting_id)
				{
					isChildren = true;
					break;
				}
				else
					isChildren = false;
			}
		}

		// check if it is a child or parent to any entities
		static bool p_child = false;
		static bool p_parent = false;
		std::set<EntityID> parentingIDs = Engine::_ecs.getChildEntities(parenting_id);

		if (parentingIDs.empty())
		{
			p_parent = false; p_child = false;
		}
		else
		{
			p_parent = true;
			for (auto child : parentingIDs)
			{
				if (child == entity_ext_id)
				{
					p_child = true;
					break;
				}
				else
					p_child = false;
			}
		}

		std::string isParentStr = isParent ? "true" : "false";
		std::string isChildStr = p_child ? "true" : "false";

		std::string pParentStr = p_parent ? "true" : "false";
		std::string pChildStr = isChildren ? "true" : "false";

		std::string entityInfo; std::string selectedInfo;
		if (Engine::_ecs.hasEntity(entity_ext_id))
		{
			entityInfo = "Heirarchy EntityID: " + std::to_string(entity_ext_id) + "\n"
				+ "Name: " + Engine::_ecs.getEntityName(entity_ext_id) + "\n"
				+ "isParent: " + isParentStr + "\t" + "isChild: " + isChildStr + "\n";
		}
		if (parenting_id != entity_ext_id)
		{
			if (Engine::_ecs.hasEntity(parenting_id))
			{
				selectedInfo = "\nParent-child info: " + std::to_string(parenting_id) + "\n"
					+ "Name: " + Engine::_ecs.getEntityName(parenting_id) + "\n"
					+ "isParent: " + pParentStr + "\t" + "isChild: " + pChildStr + "\n";
			}
		}
		std::string parentingInfo = entityInfo + selectedInfo;
		ImGui::TextUnformatted(parentingInfo.c_str());
	}
	ImGui::End();
}

void EditorUI::showWayPoint()
{
	// check if car spawner to display option
	if (Engine::_ecs.hasEntity(entity_ext_id))
	{
		if (Engine::_ecs.getEntityName(entity_ext_id).find("Vehicle") != std::string::npos)
		{
			path_ext_id = entity_ext_id;
		}
		std::string spawnerName = Engine::_ecs.getEntityName(path_ext_id);
		if (ImGui::Begin("WayPoint Setter"))
		{
			ImGui::Text(spawnerName.c_str());
			ImGui::TextUnformatted("Please select road tiles\nto set waypoint. \nOnly select, start_point, \nintersection and end_point.");

			if (Engine::_ecs.getEntityName(path_ext_id).find("Vehicle") != std::string::npos)
			{
				std::string selected_vehicle_spawner = "Selected Vehicle Spawner:\n " + spawnerName;
				ImGui::Text(selected_vehicle_spawner.c_str());
				ImGui::NewLine();
				std::string selected_name = Engine::_ecs.getEntityName(entity_ext_id);
				if (selected_name.find("Tile") != std::string::npos || selected_name.find("Floor") != std::string::npos)
				{
					ImGui::InputText("Point Name", point_name, IM_ARRAYSIZE(point_name));
					if (ImGui::Button("Add Point"))
					{
						auto transform = Engine::_ecs.getComponent<TransformCom>(entity_ext_id);
						// change color of tile when added to waypoint map
						auto material = Engine::_ecs.getComponent<MaterialCom>(entity_ext_id);
						material->_saturation.y = 0;
						material->_saturation.z = 4.0f;
						tileID.push_back(entity_ext_id);
						std::string str(point_name);
						//WayPoint.insert(std::pair<std::string, Vector3>(str, transform->pos));
						RESOURCE->AddWayPoint(str, transform->pos);
					}

					for (auto map : *RESOURCE->GetWayPointMap())
					{
						ImGui::Text(map.first.c_str());
						for (auto point : map.second)
						{
							ImGui::Text(std::to_string(point.x).c_str());
							ImGui::Text(std::to_string(point.y).c_str());
							ImGui::Text(std::to_string(point.z).c_str());
						}
					}

					if (ImGui::Button("Clear"))
					{
						//WayPoint.clear();
						for (auto tile : tileID)
						{
							auto material = Engine::_ecs.getComponent<MaterialCom>(tile);
							material->_saturation.y = 1.0f;
							material->_saturation.z = 1.0f;
						}
						tileID.clear();
						std::string str(point_name);
						RESOURCE->RemoveWayPoint(str);
					}
					ImGui::SameLine();
					if (ImGui::Button("Save"))
					{
						// do saving into text file
						if (SerializeWayPoint(point_name, point_name))
						{
							for (auto tile : tileID)
							{
								auto material = Engine::_ecs.getComponent<MaterialCom>(tile);
								material->_saturation.y = 1.0f;
								material->_saturation.z = 1.0f;
							}
							tileID.clear();
							std::cout << "Waypoint saved successfully!" << std::endl;

						}
						else
							std::cout << "Failed to save Waypoint!" << std::endl;
					}
				}
			}
		}
		ImGui::End();
	}

}

void EditorUI::updateParentChildProperty(std::string str, property::data Data, std::string second)
{
	// make an outer check to see if game is played, don't update same values for entity-child
	if (!Engine::_isPlay)
	{
		std::set<EntityID> childIDSet = Engine::_ecs.getChildEntities(entity_ext_id);
		if (!childIDSet.empty())
			isChild = true;
		if (isChild && Engine::_ecs.hasEntity(entity_ext_id))
		{
			for (auto childID : childIDSet)
			{
				if (Engine::_ecs.hasEntity(childID))
				{
					std::vector<std::string> selectedEntityCompsNames;
					std::vector<std::shared_ptr<Component>> selectedEntityComps;
					Engine::_ecs.getEntityComponentPtrAndName(childID, selectedEntityCompsNames, selectedEntityComps);
					std::string ComponentName;
					int i = 0;
					for (auto& E : selectedEntityComps)
					{
						ComponentName = selectedEntityCompsNames[i++];
						//std::cout << ComponentName << std::endl;
						//std::cout << "update data: " + str << std::endl;

						if (str != "TransformCom/pos")
						{
							//if (ComponentName == "MaterialCom")
							//{
							//	// model, shader, texture
							//	auto material = Engine::_ecs.getComponent<MaterialCom>(childID);
							//	if (str == "MaterialCom/model")
							//		material->SetModel(second);
							//	else if (str == "MaterialCom/texture")
							//		material->SetTexture(second);
							//	else if (str == "MaterialCom/shader")
							//		material->SetShader(second);
							//}

							property::set(*E, str.c_str(), Data);
						}
					}
				}
			}
			isChild = false;
		}
	}
}

//void EditorUI::updatePrefabsProperty()
//{
//	std::vector<EntityID> entity_vector = Engine::_ecs.findPrefabParentEntity(entity_ext_id);
//	if (!entity_vector.empty())
//		found_prefab = true;
//	if (found_prefab)
//	{
//		updatePrefabsValues(&entity_vector, propEntryName, property_entry);
//		found_prefab = false;
//		entity_vector.clear();
//	}
//}

void EditorUI::showPropertyPrefab()
{

	if (ImGui::Begin("Prefab Inspector"))
	{
		std::vector<std::string> selectedPrefabCompsNames;
		std::vector<std::shared_ptr<Component>> selectedPrefabComps;

		if (Engine::_ecs.hasPrefab(prefab_ext_id))
		{
			//std::cout << "SELECTED PREFAB ID: " << std::to_string(prefab_ext_id) + "\n"
			//	<< "SELECTED PREFAB NAME: " << Engine::_ecs.getPrefabName(prefab_ext_id) << std::endl;
			Engine::_ecs.getEntityComponentPtrAndNamePrefab(prefab_ext_id, selectedPrefabCompsNames, selectedPrefabComps);

			ImGui::Separator();
			int i = 0;
			std::string ComponentNamePrefab;
			for (auto& E : selectedPrefabComps)
			{
				ComponentNamePrefab = selectedPrefabCompsNames[i++];
				//ImGui::Text(ComponentName.c_str());

				property::entry PropEntry;
				if (ImGui::CollapsingHeader(ComponentNamePrefab.c_str()))
				{
					property::DisplayEnum(*E, [&](std::string_view PropertyName, property::data&& Data, const property::table&, std::size_t, property::flags::type Flags)
						{
							(void)Flags;
							// Get properties name
							std::string Property;

							if (PropertyName.find('/' != std::string::npos))
							{
								Property = PropertyName.substr(PropertyName.find_last_of('/') + 1);
							}
							PropEntry.first = PropertyName;

							std::visit([&](auto&& Value)
								{
									using T = std::decay_t<decltype(Value)>;

									if constexpr (std::is_same_v<T, int>)
									{
										if (ComponentNamePrefab == "TransformCom")
										{
											T n = Value;
											ImGui::Combo(Property.c_str(), &n, _stateType, IM_ARRAYSIZE(_stateType));
											PropEntry.second = n;
										}
										else
										{
											T n = Value;
											ImGui::DragInt(Property.c_str(), &n, 0.01f);
											PropEntry.second = n;
										}
									}
									else if constexpr (std::is_same_v<T, float>)
									{
										T n = Value;
										ImGui::DragFloat(Property.c_str(), &n, 0.01f);
										PropEntry.second = n;
									}
									else if constexpr (std::is_same_v <T, uint32_t>)
									{
										T n = Value;
										int32_t x = (int32_t)n;
										ImGui::DragInt(Property.c_str(), &x, 0.01f);
										PropEntry.second = n;
									}
									else if constexpr (std::is_same_v<T, bool>)
									{
										T check = Value;
										ImGui::Checkbox(Property.c_str(), &check);
										PropEntry.second = check;
									}
									else if constexpr (std::is_same_v<T, string_t>)
									{
										if (ComponentNamePrefab == "MaterialCom")
										{
											std::string text = Value;
											std::string newVal;
											// model, shader, texture
											newVal = comboSelectionPrefab(Property.c_str(), text);
											PropEntry.second = newVal;
										}
										else
										{
											T text = Value;
											ImGui::InputText(Property.c_str(), text.data(), text.size());
											PropEntry.second = text;
										}
									}
									else if constexpr (std::is_same_v<T, oobb>)
									{
										// do not use
									}
									else if constexpr (std::is_same_v<T, Vector3>)
									{
										Vector3 vector = Value;
										ImGui::DragFloat3(Property.c_str(), &vector.x, 0.01f);
										PropEntry.second = vector;
									}
									else if constexpr (std::is_same_v<T, Vector4>)
									{
										Vector4 vector = Value;
										ImGui::DragFloat4(Property.c_str(), &vector.x, 0.01f);
										PropEntry.second = vector;
									}
									else if constexpr (std::is_same_v<T, Matrix4>)
									{
										Matrix4 mat4 = Value;
										ImGui::DragFloat4(Property.c_str(), &mat4._m[0][0]);
										ImGui::DragFloat4(Property.c_str(), &mat4._m[1][0]);
										ImGui::DragFloat4(Property.c_str(), &mat4._m[2][0]);
										ImGui::DragFloat4(Property.c_str(), &mat4._m[3][0]);
										PropEntry.second = mat4;
									}
									else if constexpr (std::is_same_v<T, Quat>)
									{
										Quat quat = Value;
										ImGui::DragFloat4(Property.c_str(), &quat.x, 0.01f);
										PropEntry.second = quat;
									}
									else if constexpr (std::is_same_v<T, glm::vec3>)
									{
										glm::vec3 vector = Value;
										ImGui::DragFloat3(Property.c_str(), &vector.x, 0.01f);
										PropEntry.second = vector;
									}
									else assert(always_false<T>::value);

								}
							, Data);

							property::set(*E, PropEntry.first.c_str(), PropEntry.second);
						});
				}
			}
		}
	}
	ImGui::End();
}

void EditorUI::showComponents()
{

	//ImGui::Begin("Inspector");
	std::set<std::string> entityTags;
	static bool is_delete_tag = false;
	static std::string delete_tag_name;
	ImGui::Text("EntityID: %d", entity_ext_id);
	if (Engine::_ecs.hasEntity(entity_ext_id))
	{
		ImGui::Text("EntityName: %s", Engine::_ecs.getEntityName(entity_ext_id).c_str());
		entityTags = Engine::_ecs.getEntityTag(entity_ext_id);
	}

	if (ImGui::TreeNode("Entity Tags"))
	{
		ImGui::Indent(-20.0f);
		if (ImGui::ListBoxHeader("Entity Tags", ImVec2(145, 50)))
		{
			for (auto tag : entityTags)
			{
				if (ImGui::Selectable(tag.c_str()))
				{
					delete_tag_name = tag;
					is_delete_tag = true;
				}
			}
			ImGui::ListBoxFooter();
		}

		static bool is_selected = false;
		static std::string selected_tag = "Choose Tag";
		if (ImGui::BeginCombo("Tag List", selected_tag.c_str()))
		{
			std::vector<std::string> tagList = Engine::_ecs.getTagsList();
			for (auto tag : tagList)
			{
				if (ImGui::Selectable(tag.c_str(), &is_selected))
				{
					is_selected = true;
					selected_tag = tag;
					break;
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Add Tag"))
		{
			if (is_selected)
			{
				std::cout << "Selected Tag: " + selected_tag << std::endl;
				Engine::_ecs.addEntityTag(entity_ext_id, selected_tag);
				is_selected = false;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete Tag"))
		{
			if (is_delete_tag)
			{
				// check if entity is a parent
				// if entity is a parent, remove entity from parent and de-child the other entities
				// else if entity is a child, remove entity id from parent map
				Engine::_ecs.removeEntityTag(entity_ext_id, delete_tag_name);
				std::cout << "Removed tag: " + delete_tag_name +
					"\t EntityID: " + std::to_string(entity_ext_id) << std::endl;
				is_delete_tag = false;
			}
		}

		ImGui::InputText("New Tag", tagName, IM_ARRAYSIZE(tagName));
		ImGui::SameLine();
		// change icon font
		if (ImGui::Button("+"))
		{
			Engine::_ecs.createNewTag(tagName);
		}
		ImGui::Indent();
		ImGui::TreePop();
	}

	ImGui::Separator();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(255, 0, 0)));
	std::string destroy = "";
	if (ImGui::Button("Delete", ImVec2(60.0f, 0.0f)))
	{
		if (Engine::_ecs.hasEntity(entity_ext_id))
		{
			destroy = "Delete Entity";

		}
		if (destroy == "Delete Entity")
			ImGui::OpenPopup("Delete Entity");
	}
	ImGui::PopStyleColor();

	if (ImGui::BeginPopupModal("Delete Entity", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Confirm to Delete Object?");
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(255, 0, 0)));
		if (ImGui::Button("Yes", ImVec2(60.0f, 0.0f)))
		{
			if (Engine::_ecs.getEntityName(entity_ext_id).find("Text") != std::string::npos)
			{
				// clear the map from _textMap resource
				int id = RESOURCE->getTextID(entity_ext_id);
				RESOURCE->insertTextToDeleteMap(id);
				RESOURCE->eraseTextMap(entity_ext_id);
			}
			ImGui::Text("Destroying EntityID: %d", entity_ext_id);
			Engine::_ecs.destroyEntity(entity_ext_id);
			entity_ext_id = ROOTID;
			//std::cout << entity_ext_id << std::endl;
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor();
		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(60.0f, 0.0f)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}


	if (Engine::_ecs.hasEntity(entity_ext_id))
	{

		auto compnames = Engine::_ecs.getEntityComponentNames(entity_ext_id);
		auto it = compnames.begin();
		//ecs listallcomponent
		//ecs getentitycomponentnames
		//ImGui::Text("Size of Components: %d", compnames.size());
		//auto tags = Engine::_ecs.getEntity(entity_ext_id)._tags;

		//for (auto& tag : tags)
		//	ImGui::Text(tag.c_str());
		static int c_array_choice = 0;


		static bool com_select = false;

		std::vector<std::string> has_comp_list = Engine::_ecs.getEntityComponentNames(entity_ext_id);


		std::vector<std::string> comp_list = Engine::_ecs.listAllComponents();
		std::vector<const char*> comp_array;
		for (auto& i : comp_list)
		{
			comp_array.push_back(i.c_str());
		}
		ImGui::PushItemWidth(120);
		ImGui::Combo(" ", &c_array_choice, comp_array.data(), static_cast<int>(comp_array.size()));
		compTypeName = comp_array[c_array_choice];

		ImGui::SameLine();
		if (ImGui::Button("Add", ImVec2(40.0f, 0.0f)) && !compTypeName.empty())
		{
			if (compTypeName == "LightCom" && !Engine::_ecs.hasComponent<LightCom>(entity_ext_id))
			{
				LightCom c;
				Engine::_ecs.addComponentToEntity(entity_ext_id, c);
			}
			else if (compTypeName == "TransformCom" && !Engine::_ecs.hasComponent<TransformCom>(entity_ext_id))
			{
				TransformCom c;
				Engine::_ecs.addComponentToEntity(entity_ext_id, c);
			}
			else if (compTypeName == "CameraCom" && !Engine::_ecs.hasComponent<CameraCom>(entity_ext_id))
			{
				CameraCom c;
				Engine::_ecs.addComponentToEntity(entity_ext_id, c);
			}
			else if (compTypeName == "RigidStaticCom" && !Engine::_ecs.hasComponent<RigidStaticCom>(entity_ext_id))
			{
				RigidStaticCom c;
				auto transform = Engine::_ecs.getComponent<TransformCom>(entity_ext_id);
				if (transform)
					Engine::_ecs.getSystem<PhysicsSystem>()->AddRigidStaticBody(entity_ext_id, c, *transform);
				Engine::_ecs.addComponentToEntity(entity_ext_id, c);
			}
			else if (compTypeName == "RigidDynamicCom" && !Engine::_ecs.hasComponent<RigidDynamicCom>(entity_ext_id))
			{
				RigidDynamicCom c;
				auto transform = Engine::_ecs.getComponent<TransformCom>(entity_ext_id);
				if (transform)
					Engine::_ecs.getSystem<PhysicsSystem>()->AddRigidDynamicBody(entity_ext_id, c, *transform);
				Engine::_ecs.addComponentToEntity(entity_ext_id, c);
			}
			else if (compTypeName == "EmitterCom" && !Engine::_ecs.hasComponent<EmitterCom>(entity_ext_id))
			{
				EmitterCom c;
				Engine::_ecs.addComponentToEntity(entity_ext_id, c);
			}
			else if (compTypeName == "ParticleCom" && !Engine::_ecs.hasComponent<ParticleCom>(entity_ext_id))
			{
				ParticleCom c;
				Engine::_ecs.addComponentToEntity(entity_ext_id, c);
			}
			else if (compTypeName == "MaterialCom" && !Engine::_ecs.hasComponent<MaterialCom>(entity_ext_id))
			{
				MaterialCom c;
				Engine::_ecs.addComponentToEntity(entity_ext_id, c);

			}
			else if (compTypeName == "uaScriptCom" && !Engine::_ecs.hasComponent<LuaScriptCom>(entity_ext_id))
			{
				LuaScriptCom c;
				Engine::_ecs.addComponentToEntity(entity_ext_id, c);
				c.OnImGuiDisplay();
			}
			else if (compTypeName == "AudioSrcCom" && !Engine::_ecs.hasComponent<AudioSrcCom>(entity_ext_id))
			{
				AudioSrcCom c;
				Engine::_ecs.getSystem<AudioSystem>()->CreateChannelGroup(
					Engine::_ecs.getEntityName(entity_ext_id).c_str(), c);
				Engine::_ecs.addComponentToEntity(entity_ext_id, c);
			}
			else if (compTypeName == "AnimationCom" && !Engine::_ecs.hasComponent<AnimationCom>(entity_ext_id))
			{
				AnimationCom c;
				Engine::_ecs.addComponentToEntity(entity_ext_id, c);

			}
			else if (compTypeName == "UICom" && !Engine::_ecs.hasComponent<UICom>(entity_ext_id))
			{
				UICom c;
				Engine::_ecs.addComponentToEntity(entity_ext_id, c);
				/*auto uiPtr = Engine::_ecs.getComponent<UICom>(entity_ext_id);
				uiPtr->registerToSystem();*/
			}
			else if (compTypeName == "SpriteCom" && !Engine::_ecs.hasComponent<SpriteCom>(entity_ext_id))
			{
				SpriteCom c;
				Engine::_ecs.addComponentToEntity(entity_ext_id, c);
			}
			else if (compTypeName == "TextCom" && !Engine::_ecs.hasComponent<TextCom>(entity_ext_id))
			{
				TextCom c;
				Engine::_ecs.addComponentToEntity(entity_ext_id, c);
			}
			else
			{
				return;
			}
		}
		ImGui::PopItemWidth();

		//for (; it != compnames.end(); it++)
		//{
		//	ImGui::Text(it->c_str());
		//}

		//		if (Engine::_ecs.hasComponent<LightCom>(entity_ext_id))
		//		{
		//			auto light = Engine::_ecs.getComponent<LightCom>(entity_ext_id);
		//			if (ImGui::CollapsingHeader("Light"))
		//			{
		//				if (light)
		//				{
		//					Vector3 ambient = light->ambient;
		//					Vector3 diffuse = light->diffuse;
		//					Vector3 specular = light->specular;

		//					//ImGui::Text("Ambient");
		//					ImGui::DragFloat3("Ambient", &ambient.x, 0.01f, -2000.0f, 2000.0f, "%.2f");

		//					//ImGui::Text("Diffuse");
		//					ImGui::DragFloat3("Diffuse", &diffuse.x, 0.01f, -2000.0f, 2000.0f, "%.2f");

		//					//ImGui::Text("Specular");
		//					ImGui::DragFloat3("Specular", &specular.x, 0.01f, -2000.0f, 2000.0f, "%.2f");

		//					light->ambient = ambient;
		//					light->diffuse = diffuse;
		//					light->specular = specular;

		//					/*light->linear;
		//					light->quadratic;*/
		//				}
		//				if (ImGui::Button("Delete Component", ImVec2(120.0f, 0.0f)))
		//				{
		//					Engine::_ecs.removeComponentFromEntity<LightCom>(entity_ext_id);
		//				}
		//			}
		//		}

		//		if (Engine::_ecs.hasComponent<TransformCom>(entity_ext_id))
		//		{
		//			auto rigid = Engine::_ecs.getComponent<RigidDynamicCom>(entity_ext_id);
		//			auto _transform = Engine::_ecs.getComponent<TransformCom>(entity_ext_id);
		//			if (ImGui::CollapsingHeader("Transform"))
		//			{
		//				/*glm::vec3 pos = { _transform->pos };
		//				ImGui::Text("%d Position %d, %d, %d", entity_ext_id, pos.x, pos.y, pos.z);
		//				ImGui::DragFloat("Position X", &pos.x, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//				ImGui::DragFloat("Position Y", &pos.y, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//				ImGui::DragFloat("Position Z", &pos.z, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//				_transform->pos = pos;*/

		//				/*PxVec3 newPos = rigid->body->getGlobalPose().p;
		//				glm::vec3 pos = { newPos.x, newPos.y, newPos.z };
		//				ImGui::Text("Rigid Position");
		//				ImGui::DragFloat("Position X", &pos.x, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//				ImGui::DragFloat("Position Y", &pos.y, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//				ImGui::DragFloat("Position Z", &pos.z, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//				newPos = { pos.x, pos.y, pos.z };
		//				_transform->pos = { pos.x, pos.y, pos.z };*/
		//				if (rigid)
		//				{
		//					Vector3 newPos = rigid->body->getGlobalPose().p;
		//					Vector3 pos = { newPos.x, newPos.y, newPos.z };
		//					//ImGui::Text("Rigid Position");
		//					ImGui::DragFloat3("Position", &pos.x, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//					newPos = { pos.x, pos.y, pos.z };
		//					_transform->pos = { pos.x, pos.y, pos.z };

		//					Vector3 scale = { _transform->scale };
		//					//ImGui::Text("Scale");
		//					ImGui::DragFloat3("Scale", &scale.x, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//					_transform->scale = scale;

		//					Quat newRot = rigid->body->getGlobalPose().q;
		//					Vector3 rot = Quat_to_Euler(newRot);
		//					//ImGui::Text("Rigid Rotation");
		//					ImGui::DragFloat3("Rotation", &rot.x, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//					//ImGui::DragFloat("Rotation Y", &rot.y, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//					//ImGui::DragFloat("Rotation Z", &rot.z, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//					//ImGui::DragFloat("Rotation W", &rot.w, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//					newRot = Euler_to_Quat(rot);
		//					//newRot.Normalize();
		//					_transform->rotate = { rot.x, rot.y, rot.z };
		//					rigid->body->setGlobalPose(PxTransform(newPos.Vec3toPVex3(), newRot.QuattoPQuat()));
		//				}
		//				else
		//				{
		//					//PxVec3 newPos = rigid->body->getGlobalPose().p;
		//					Vector3 pos = _transform->pos;
		//					//ImGui::Text("Rigid Position");
		//					ImGui::DragFloat3("Position", &pos.x, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//					//newPos = { pos.x, pos.y, pos.z };
		//					_transform->pos = { pos.x, pos.y, pos.z };

		//					Vector3 scale = { _transform->scale };
		//					//ImGui::Text("%d Scale %d, %d, %d", entity_ext_id, scale.x, scale.y, scale.z);
		//					ImGui::DragFloat3("Scale", &scale.x, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//					_transform->scale = scale;

		//					//PxQuat newRot = rigid->body->getGlobalPose().q;
		//					Vector3 rot =  _transform->rotate;
		//					//ImGui::Text("%d Rigid Rotation %d, %d, %d", entity_ext_id, rot.x, rot.y, rot.z);
		//					ImGui::DragFloat3("Rotation", &rot.x, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//					//ImGui::DragFloat("Rotation Y", &rot.y, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//					//ImGui::DragFloat("Rotation Z", &rot.z, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//					//ImGui::DragFloat("Rotation W", &rot.w, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//					//newRot = { rot.x, rot.y, rot.z, rot.w };
		//					//newRot.normalize();
		//					_transform->rotate = rot;
		//					//rigid->body->setGlobalPose(PxTransform(newPos, newRot));
		//				}
		//				if (ImGui::Button("Delete Component", ImVec2(120.0f, 0.0f)))
		//				{
		//					Engine::_ecs.removeComponentFromEntity<TransformCom>(entity_ext_id);
		//				}

		//			}
		//		
		//		}
		//		if (Engine::_ecs.hasComponent<MaterialCom>(entity_ext_id))
		//		{
		//			if (ImGui::CollapsingHeader("Material"))
		//			{
		//				bool open_explorer1 = false;
		//				if (ImGui::Button("Change Texture", ImVec2(120.0f, 0.0f)))
		//				{
		//					open_explorer1 = true;
		//				}
		//				if (open_explorer1)
		//					ImGui::OpenPopup("Open File");

		//				if (file_dialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".dds,*.*"))
		//				{
		//					std::cout << file_dialog.selected_fn << std::endl;      // The name of the selected file or directory in case of Select Directory dialog mode
		//					std::cout << file_dialog.selected_path << std::endl;    // The absolute path to the selected file

		//					//size_t pos = file_dialog.selected_fn.find(".dds");
		//					//std::string filename = file_dialog.selected_fn;
		//				
		//					texture_name = file_dialog.selected_path;
		//					//texture_name.erase(pos);
		//					Engine::_ecs.getSystem<RenderSystem>()->_graphics.add_texture(file_dialog.selected_fn, modelLoader::mLoader::LoadTextureDDS(texture_name.c_str()));
		//					auto material = Engine::_ecs.getComponent<MaterialCom>(entity_ext_id);
		//					material->m_texture = file_dialog.selected_fn;
		//					material->isInitialised = false;
		//				
		//					//mdl.modelLoader::mLoader::setTextureDDS(file_dialog.selected_fn.c_str());
		//				}
		//			
		//			}

		//		}
		//		if (Engine::_ecs.hasComponent<RigidDynamicCom>(entity_ext_id))
		//		{
		//			/*auto rigid = Engine::_ecs.getComponent<RigidDynamicCom>(entity_ext_id);
		//			auto _transform = Engine::_ecs.getComponent<TransformCom>(entity_ext_id);*/

		//			if (ImGui::CollapsingHeader("RigidBodyDynamic"))
		//			{
		//				/*PxVec3 newPos = rigid->body->getGlobalPose().p;
		//				glm::vec3 pos = { newPos.x, newPos.y, newPos.z };
		//				ImGui::Text("Rigid Position");
		//				ImGui::DragFloat("Position X", &pos.x, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//				ImGui::DragFloat("Position Y", &pos.y, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//				ImGui::DragFloat("Position Z", &pos.z, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//				newPos = { pos.x, pos.y, pos.z };*/
		//				/*rigid->body->setGlobalPose(PxTransform(newPos, PxQuat(0, 0, 0, 1)));*/
		//				/*_transform->pos.x = newPos.x;
		//				_transform->pos.y = newPos.y;
		//				_transform->pos.z = newPos.z;*/

		//				/*PxQuat newRot = rigid->body->getGlobalPose().q;
		//				glm::vec4 rot = { newRot.x, newRot.y, newRot.z, newRot.w };
		//				ImGui::Text("%d Rigid Rotation %d, %d, %d", entity_ext_id, rot.x, rot.y, rot.z);
		//				ImGui::DragFloat("Rotation X", &rot.x, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//				ImGui::DragFloat("Rotation Y", &rot.y, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//				ImGui::DragFloat("Rotation Z", &rot.z, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//				ImGui::DragFloat("Rotation W", &rot.w, 0.01f, -2000.0f, 2000.0f, "%.2f");
		//				newRot = { rot.x, rot.y, rot.z, rot.w };
		//				newRot.normalize();
		//				rigid->body->setGlobalPose(PxTransform(newPos, newRot));*/

		//				/*_transform->rotate.x = newRot.x;
		//				_transform->rotate.y = newRot.y;
		//				_transform->rotate.z = newRot.z;
		//				_transform->rotate.w = newRot.w;*/

		//				/*ImGui::Text("Rigid Mass");
		//				ImGui::InputFloat("Mass", &rigid->mass);
		//				rigid->body->setMass(rigid->mass);*/

		//				if (ImGui::Button("Delete Component", ImVec2(120.0f, 0.0f)))
		//				{
		//					Engine::_ecs.removeComponentFromEntity<TransformCom>(entity_ext_id);
		//				}
		//			}
		//		
		//		}

		//		if (Engine::_ecs.hasComponent<CameraCom>(entity_ext_id))
		//		{
		//			auto _cam = Engine::_ecs.getComponent<CameraCom>(entity_ext_id);
		//			if (ImGui::CollapsingHeader("Camera"))
		//			{
	 //                   ImGui::Text("Projection");
	 //                   ImGui::DragFloat("FOV degrees", &(_cam->_m_fov), 0.01f, 0.10f, 100.0f, "%.2f");
	 //                   ImGui::DragFloat("Near Plane", &(_cam->_m_near), 0.1f, 0.10f, 100.0f, "%.2f");
	 //                   ImGui::DragFloat("Far Plane", &(_cam->_m_far), 0.1f, 0.10f, 2000.0f, "%.2f");

	 //                   glm::vec3 poss = { _cam->_pos.Vec3toGlmVec3() };
	 //                   ImGui::Text("Camera Position");
	 //                   ImGui::DragFloat3("Camera Pos", &poss[0], 0.01f, -30.0f, 30.0f, "%.2f");
	 //                   _cam->_pos = poss;
	 //                   Engine::_ecs.getSystem<CameraSystem>()->updateViewMatrix(_cam);

	 //                   if (ImGui::Button("Delete Component", ImVec2(150.0f, 25.0f)))
		//				{
		//					Engine::_ecs.removeComponentFromEntity<CameraCom>(entity_ext_id);
		//				}
		//			}
		//		
		//		}
		//		if (Engine::_ecs.hasComponent<EmitterCom>(entity_ext_id))
		//		{
		//			auto _emitter = Engine::_ecs.getComponent<EmitterCom>(entity_ext_id);
		//			if (ImGui::CollapsingHeader("Emitter"))
		//			{
		//				ImGui::Checkbox("Emit", &(_emitter->emit));
		//				ImGui::ColorEdit4("Birth Color", &(_emitter->colorBegin.x));
		//				ImGui::ColorEdit4("Death Color", &(_emitter->colorEnd.x));
		//				ImGui::DragFloat3("VelocityVar", &(_emitter->velocityVariation.x));
		//				ImGui::DragFloat("SizeBegin", &(_emitter->sizeBegin), 0.01f, 0.f, 30.f);
		//				//ImGui::DragFloat("SizeEnd", &(_emitter->SizeEnd));
		//				ImGui::DragFloat("Rotation", &(_emitter->rotation));
		//				ImGui::DragFloat("SizeVar", &(_emitter->sizeVariation), 0.01f, 0.f, 30.f);
		//				ImGui::DragFloat("Life Time", &(_emitter->lifeTime), 0.1f, 0.0f, 4.0f);
		//				ImGui::DragFloat("Timer", &(_emitter->timeset), 0.1f, 0.0f, 10.0f);

		//				if (ImGui::Button("Delete Component", ImVec2(120.0f, 0.0f)))
		//				{
		//					for(auto particle : _emitter->particles)
		//						Engine::_ecs.destroyEntity(particle);
		//					Engine::_ecs.removeComponentFromEntity<EmitterCom>(entity_ext_id);
		//				}
		//			}
		//		}
		//	
	}

	//ImGui::End();
}

void EditorUI::showConsole()
{
	ImGui::Begin("Console", NULL, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(255, 0, 0)));
		if (ImGui::Button("Clear", ImVec2(50.0f, 0.0f)))
		{
			bufGI.clear();
			console_buffer.str(std::string());
		};
		ImGui::PopStyleColor();
		ImGui::EndMenuBar();
	}


	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	//Use imgui text formatting
	bufGI.append(console_buffer.str().c_str());
	ImGui::TextUnformatted(bufGI.begin());
	bufGI.clear();

	console_buffer.clear();
	if (!ImGui::IsWindowHovered())
		ImGui::SetScrollHere(1.0f);

	ImGui::EndChild();
	ImGui::End();
}

void EditorUI::showViewport()
{
	Graphics& graphics = Engine::_ecs.getSystem<RenderSystem>()->_graphics;

	ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_MenuBar);
	bool focused = ImGui::IsWindowFocused();

	auto viewportOffset = ImGui::GetCursorPos(); // iuncludes tab bar
	_viewportPanalSize = ImGui::GetContentRegionAvail();
	ImVec2 minBound = ImGui::GetWindowPos();
	minBound.x += viewportOffset.x;
	minBound.y += viewportOffset.y;
	ImVec2 middleofPanal = { minBound.x + (_viewportPanalSize.x / 2), minBound.y + (_viewportPanalSize.y / 2) };
	ImVec2 maxBound = { minBound.x + _viewportPanalSize.x, minBound.y + _viewportPanalSize.y };
	_viewportBounds[0] = Vector2{ minBound.x, minBound.y };
	_viewportBounds[1] = Vector2{ maxBound.x, maxBound.y };



	auto [mx, my] = ImGui::GetMousePos();

	mx -= _viewportBounds[0].x;
	my -= _viewportBounds[0].y;
	Vector2 viewportSize = _viewportBounds[1] - _viewportBounds[0];
	my = viewportSize.y - my;
	_mousePosition_x = (int)mx;
	_mousePosition_y = (int)my;


	if (focused && !Engine::_isPlay)
	{
		if (_mousePosition_x >= 0 && _mousePosition_y >= 0 && _mousePosition_x < (int)viewportSize.x && _mousePosition_y < (int)viewportSize.y)
		{

			if (Inputs::get_Mos_Triggered(GLFW_MOUSE_BUTTON_LEFT) && !ImGuizmo::IsOver())
			{

				auto originalPos = ImGui::GetMousePos();
				Inputs::set_MosPos((int)minBound.x, (int)minBound.y);
				auto minPos = ImGui::GetMousePos();
				Inputs::set_MosPos((int)maxBound.x, (int)maxBound.y);
				auto maxPos = ImGui::GetMousePos();

				//reset to original Pos
				ImVec2 poss;
				poss.x = originalPos.x;// +minBound.x;
				poss.y = originalPos.y;// +minBound.y;
				Inputs::set_MosPos((int)poss.x, (int)poss.y);

				float diffX = (originalPos.x - minBound.x) / (maxBound.x - minBound.x)
					, diffY = (originalPos.y - minBound.y) / (maxBound.y - minBound.y);

				diffX *= (float)1280;
				diffY *= (float)720;
				diffY = 720.0f - diffY;
				int pixeldata = graphics.readPixel(1, (int)(diffX), (int)(diffY));

				if (pixeldata < Engine::_ecs.getActiveEntities().size())
				{
					//std::string name = Engine::_ecs.getEntityName(pixeldata);
					entity_ext_id = pixeldata;
				}

			}
		}
	}

	if (!graphics.videoplaying)
		ImGui::GetWindowDrawList()->AddImage((ImTextureID)(intptr_t)(graphics.getColorBuffer()), minBound, maxBound, { 0,1 }, { 1,0 });
	else
		ImGui::GetWindowDrawList()->AddImage((ImTextureID)(intptr_t)(graphics.getVideoBuffer()), minBound, maxBound, { 0,0 }, { 1,1 });

	if (ImGui::BeginMenuBar())
	{
		if (Engine::_isPlay)
		{
			if (Inputs::get_Key_Triggered(GLFW_KEY_ESCAPE))
			{
				if (!Engine::_isGameMode)
				{
					Engine::_ecs.getSystem<SerializerSystem>()->callReset();
				}

			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(255, 0, 0)));
				if (ImGui::Button("Stop", ImVec2(50.0f, 0.0f)))
				{
					Engine::_ecs.getSystem<CameraSystem>()->_camType = CameraSystem::CameraType::EDITOR;
					Engine::_isPlay = false;
					Engine::_ecs.getSystem<WindowSystem>()->setViewableCustor();
				}
				ImGui::PopStyleColor();
			}
		}
		else
		{
			//if (ImGui::Button("Play", ImVec2(50.0f, 0.0f)))
			//{
			//	Engine::isPlay = true;
			//	Engine::_ecs.getSystem<CameraSystem>()->_camType = CameraSystem::CameraType::PLAYER;


			//	Inputs::set_MosPos((int)middleofPanal.x, (int)middleofPanal.y);
			//	//Engine::_ecs.getSystem<CameraSystem>()->resetClickPos();
			//}
			//ImGui::Columns(2, "Play_Reset", false);
			if (ImGui::Button("Play", ImVec2(50.0f, 0.0f)))
			{
				Engine::_isPlay = true;
				_gizmoType = -1;
				Engine::_ecs.getSystem<CameraSystem>()->_camType = CameraSystem::CameraType::PLAYER;
				Engine::_ecs.getSystem<ScriptingSystem>()->LoadSceneScripts();

				Inputs::set_MosPos((int)middleofPanal.x, (int)middleofPanal.y);
				Engine::_ecs.getSystem<WindowSystem>()->setViewableCustor();
				// set the restart scene to run				
				auto serializer = Engine::_ecs.getSystem<SerializerSystem>();
				serializer->setFileName("reset.scene");
				serializer->setIsSaved(true);
				if (EditorSystem::_PlayInFullScreen)
				{
					Engine::_ecs.getSystem<EditorSystem>()->_isActive = false;
					Engine::_ecs.getSystem<RenderSystem>()->toggle_renderFramebuffer(false);
				}

				Engine::_ecs.getSystem<AudioSystem>()->SaveSoundsToJson("./Resources/AudioFiles/AudioFiles.json");
				if (Engine::_isGameMode)
				{
					GameStateManager::_curState = (int)GameState::LOADING_START;
				}
				//Engine::_ecs.getSystem<CameraSystem>()->resetClickPos();
			}
			ImGui::SameLine(120.0f);
			static bool isChecked = false;
			if (ImGui::Checkbox("Play in Fullscreen", &isChecked))
			{
				EditorSystem::_PlayInFullScreen = isChecked;
				std::cout << std::to_string(EditorSystem::_PlayInFullScreen) << std::endl;
			}
			ImGui::SameLine(320.0f);
			if (ImGui::Checkbox("Play in GameMode", &Engine::_isGameMode))
			{
				if (Engine::_isGameMode)
				{
					GameStateManager::_curState = (int)GameState::LOADING_START;
				}
				else
				{
					GameStateManager::_curState = (int)GameState::NONE;
				}
			}
			ImGui::SameLine(500.0f);
			if (ImGui::Checkbox("Fullscreen", &WindowSystem::_isFullScreen))
				Engine::_ecs.getSystem<WindowSystem>()->setFullScreen();

			ImGui::SameLine(650.f);
			if (ImGui::Button("Reset", ImVec2(50.0f, 0.0f)))
			{
				auto serializer = Engine::_ecs.getSystem<SerializerSystem>();
				serializer->setFileName(serializer->getFileName());
				serializer->setIsLoaded(true);
			}
		}
		ImGui::EndMenuBar();
	}

	//Gizmos
	if (focused)
	{
		//can be put in editor input somewhere idk
		if (Inputs::get_Key_Triggered(GLFW_KEY_Q))
			_gizmoType = -1;
		if (Inputs::get_Key_Triggered(GLFW_KEY_W))
			_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
		if (Inputs::get_Key_Triggered(GLFW_KEY_E))
			_gizmoType = ImGuizmo::OPERATION::ROTATE;
		if (Inputs::get_Key_Triggered(GLFW_KEY_R))
			_gizmoType = ImGuizmo::OPERATION::SCALE;
	}
	if (Engine::_ecs.hasEntity(entity_ext_id) && _gizmoType != -1)
	{
		//check for transform
		auto transCom = Engine::_ecs.getComponent<TransformCom>(entity_ext_id);
		if (transCom != nullptr)
		{
			//check for camera
			std::shared_ptr<CameraCom> cam = nullptr;
			switch (Engine::_ecs.getSystem<CameraSystem>()->_camType)
			{
			case CameraSystem::CameraType::EDITOR:
				cam = Engine::_ecs.getComponent<CameraCom>(
					Engine::_ecs.getEntityID("Camera"));
				break;
			default:
				break;
			}
			//Draw gizmo
			if (cam != nullptr)
			{
				EditTransform(cam, transCom);
			}
		}
	}

	ImGui::End();
}

void EditorUI::showDebug(float delta_time, std::list<std::pair<std::string, double>>& _TimerList)
{
	//IMGUI interface begin
	ImGui::Begin("Debug"); // begin window
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text(" ");
	ImGui::Separator();
	bool wireframe = EditorSystem::_RenderObjWireframe;
	ImGui::Checkbox("Render Wireframe", &wireframe);
	EditorSystem::_RenderObjWireframe = wireframe;
	ImGui::SameLine();
	bool wireAABB = EditorSystem::_RenderObjAABB;
	ImGui::Checkbox("Render AABB", &wireAABB);
	EditorSystem::_RenderObjAABB = wireAABB;
	if (wireAABB)
		EditorSystem::_selectedEntity = entity_ext_id;

	bool lBox = EditorSystem::_RenderLightBox;
	ImGui::Checkbox("Render Light Box", &lBox);
	EditorSystem::_RenderLightBox = lBox;
	ImGui::SameLine();
	bool ui = EditorSystem::_RenderUI;
	ImGui::Checkbox("Render UI", &ui);
	EditorSystem::_RenderUI = ui;

	ImGui::Text(" ");
	ImGui::Separator();
	ImGui::Text("System Performance:");

	std::map<std::string, double> TimerMap;
	while (!_TimerList.empty())
	{
		TimerMap[_TimerList.front().first] = (_TimerList.front().second) / delta_time;
		_TimerList.pop_front();
	}

	for (auto& x : TimerMap)
	{
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.25f);
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);
		ImGui::ProgressBar(static_cast<float>(x.second), ImVec2(0, 0), NULL);
		ImGui::PopStyleVar();
		ImGui::SameLine();
		std::string name = x.first.c_str();
		name.erase(0, 5);
		const char* c_name = name.c_str();
		ImGui::Text(c_name);
	}

	ImGui::End(); // end window
}

//void EditorUI::setMouseRay(float screenWidth, float screenHeight)
//{
//	CameraCom editorCam = *(Engine::_ecs.getSystem<CameraSystem>()->_editorCamera);
//
//	Vector3 ray_ndc = Vector3{ (2.f * _mousePosition_x) / screenWidth - 1.f, (2.f * _mousePosition_y) / screenHeight - 1.f, 1.f };
//
//	float verticalFOV = editorCam._m_fov * (3.14159f / 180.f);
//	float aspectratio = 16.f / 9.f;
//	float horizontalFOV = verticalFOV * aspectratio;
//
//	float nd = editorCam._m_near;
//	float nx = nd * tanf((horizontalFOV / 2.0f) * ray_ndc.x);
//	float nh = nd / cosf((horizontalFOV / 2.0f) * ray_ndc.x);
//	float ny = nh * tanf((verticalFOV / 2.0f) * ray_ndc.y);
//
//	Vector3 origin = editorCam._pos;
//	Vector3 cam_right = editorCam._up.Cross(editorCam._eye);
//	Vector3 nearPoint = origin - (editorCam._eye * nd);
//	nearPoint += cam_right * nx;
//	nearPoint += editorCam._up * ny;
//
//	float fd = editorCam._m_far;
//	float fx = fd * tanf((horizontalFOV / 2.0f) * ray_ndc.x);
//	float fh = fd / cosf((horizontalFOV / 2.0f) * ray_ndc.x);
//	float fy = fh * tanf((verticalFOV / 2.0f) * ray_ndc.y);
//
//	Vector3 farPoint = origin - (editorCam._eye * fd);
//	farPoint += cam_right * fx;
//	farPoint += editorCam._up * fy;
//
//	Vector3 direction = farPoint - nearPoint;
//	direction = direction.Normalized();
//	_rayDirWorld = direction;
//}
//
//Vector3 EditorUI::rayCast(Vector3 ray_start, Vector3 ray_direction, float t)
//{
//	Vector3 ray_cast = ray_start + ray_direction * t;
//	return ray_cast;
//}


void EditorUI::showCreateEntity()
{
	// adding new entity
	//std::string temp = "/0";

	ImGui::InputText("Entity Name", temp_name, sizeof(temp_name));
	if (ImGui::Button("Create Entity", ImVec2(120.0f, 0.0f)))
	{
		EntityID entityID;
		if (strcmp(temp_name, "empty") == 0)
		{
			std::string temp;
			temp = "temp";
			temp = newName(temp);
			entityID = Engine::_ecs.createNewEntity(temp);
		}
		else
		{
			strcpy(temp_name, newName(temp_name).c_str());
			entityID = Engine::_ecs.createNewEntity(temp_name);
		}
		Engine::_ecs.addComponentToEntity<TransformCom>(entityID, TransformCom());
		Engine::_ecs.addComponentToEntity<MaterialCom>(entityID, MaterialCom());
		ImGui::CloseCurrentPopup();

	}

	ImGui::SameLine();
	if (ImGui::Button("Cancel", ImVec2(120, 0)))
	{
		ImGui::CloseCurrentPopup();
	}
}

std::string EditorUI::newName(std::string name)
{
	int i = 0;
	//char num [100];
	std::string copy_name = name;
	while (true)
	{
		if (Engine::_ecs.checkName(name))
		{
			name = copy_name + std::to_string(i);
			++i;
		}
		else
		{
			return name;
		}
	}
}

void EditorUI::setTextureName(std::string name)
{
	texture_name = name;
}

std::string EditorUI::getTextureName()
{
	return texture_name;
}

//To help show tool tip/ question marks
void EditorUI::ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void EditorUI::ShowAudioInspector()
{
	ImGui::Begin("Audio Inspector");
	std::vector<std::string> soundNames = Engine::_ecs.getSystem<AudioSystem>()->getSoundNames();
	for (auto& soundName : soundNames)
	{
		if (ImGui::CollapsingHeader(soundName.c_str()))
		{
			float vol = RESOURCE->GetSoundMap()[soundName].volume;
			ImGui::DragFloat("Volume", &vol, 0.01f, 0, 1.0f, "%.2f");
			RESOURCE->setSoundVolume(soundName, vol);
			bool muted = RESOURCE->GetSoundMap()[soundName].mute;
			ImGui::Checkbox("Mute", &muted);
			RESOURCE->setSoundMute(soundName, muted);
			int loopcount = RESOURCE->GetSoundMap()[soundName].loopcount;
			ImGui::DragInt("Loopcount", &loopcount, 1, -1, 10, "%d");
			ShowHelpMarker("-1: Loop Forever\n0: oneshot\n>= 1: loop that number of times then stop");
			RESOURCE->setSoundLoopcount(soundName, loopcount);


			if (RESOURCE->GetSoundMap()[soundName].mType == SoundType::SFX || RESOURCE->GetSoundMap()[soundName].mType == SoundType::SFX_3D 
				|| RESOURCE->GetSoundMap()[soundName].mType == SoundType::UI)
			{
				if (ImGui::Button("Play"))
				{
					Engine::_ecs.getSystem<AudioSystem>()->PlayUISound(soundName);
					Engine::_ecs.getSystem<AudioSystem>()->SaveSoundsToJson("./Resources/AudioFiles/AudioFiles.json");
				}
			}
			else
			{
				if (!RESOURCE->GetSoundMap()[soundName].isplaying)
				{
					if (ImGui::Button("Play"))
					{
						Engine::_ecs.getSystem<AudioSystem>()->PlayUISound(soundName);
						Engine::_ecs.getSystem<AudioSystem>()->SaveSoundsToJson("./Resources/AudioFiles/AudioFiles.json");
					}
				}
				else
				{
					if (ImGui::Button("Stop"))
					{
						Engine::_ecs.getSystem<AudioSystem>()->StopUISound(soundName);
						Engine::_ecs.getSystem<AudioSystem>()->SaveSoundsToJson("./Resources/AudioFiles/AudioFiles.json");
					}
				}
			}
		}
	}

	ImGui::End();
}

void EditorUI::EditTransform(const std::shared_ptr<CameraCom>& cam, std::shared_ptr<TransformCom>& transCom)
{
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();

	float wWidth = _viewportBounds[1].x - _viewportBounds[0].x;
	float wHeight = _viewportBounds[1].y;

	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, wWidth, wHeight);

	// view/projection transformations
	glm::mat4 projection = Matrix4(cam->_proj_mtx).Mtx4toGlmMat4();
	glm::mat4 view = Matrix4(cam->_view_mtx).Mtx4toGlmMat4();

	glm::mat4 oScale(glm::scale(glm::identity<glm::mat4>(), transCom->scale.Vec3toGlmVec3())),
		oTranslate(glm::translate(glm::identity<glm::mat4>(), transCom->pos.Vec3toGlmVec3()));

	//convert to radians
	Vector3 convertedRotate = transCom->rotate * ((float)M_PI / 180.0f);

	glm::mat4
		xRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.x,glm::vec3(1.0f,0.0f,0.0f)) },
		yRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.y,glm::vec3(0.0f,1.0f,0.0f)) },
		zRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.z,glm::vec3(0.0f,0.0f,1.0f)) };


	glm::mat4 oRotate = zRotate * yRotate * xRotate;
	//glm::mat4 oRotate = glm::mat4(glm::quat(transCom->rotate.Vec3toGlmVec3()));
	glm::mat4 transform = oTranslate * oRotate * oScale;
	ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
		(ImGuizmo::OPERATION)_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform));

	if (ImGuizmo::IsUsing())
	{
		glm::vec3 translation, rotation, scale;
		Matrix4::DecomposeTransform(transform, translation, rotation, scale);

		//convert back to degrees
		convertedRotate = Vector3(rotation * (180.0f / (float)M_PI));
		//convertedRotate = Vector3(rotation);

		switch (_gizmoType)
		{
		case(ImGuizmo::TRANSLATE):
			transCom->pos = translation;
			break;
		case(ImGuizmo::ROTATE):
			transCom->rotate = convertedRotate;
			break;
		case(ImGuizmo::SCALE):
			transCom->scale = scale;
			break;
		default:
			break;
		}
	}

}
