#include "SceneManager.h"
#include "Scene.h"

void SceneManager::Update(float deltaTime)
{
	for(auto& scene : m_scenes)
	{
		scene->Update(deltaTime);
	}
}

void SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

Scene& SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
