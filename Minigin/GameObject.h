#pragma once
#include <memory>
#include "Transform.h"
#include "BaseComponent.h"
#include <vector>
#include <string>
#include <typeindex>

class Texture2D;



// todo: this should become final.
class GameObject final
{
public:
	virtual void Update(float deltaTime);
	virtual void Render() const;

	void SetPosition(float x, float y);

	#pragma region componentSystem
	template<typename T,typename... Args>
	T* AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of_v<BaseComponent,T>,"T Must derive from BaseComponent");

		auto component = std::make_unique<T>(this,std::forward<Args>(args)...);
		T* rawPtr = component.get();

		m_Components.push_back(std::move(component));
		return rawPtr;
	}

	template<typename T>
	T* GetComponent()const
	{
		for(auto& component : m_Components)
		{
			if(dynamic_cast<T*>(component.get()) != nullptr)
			{
				return static_cast<T*>(component.get());
			}
		}
		
		return nullptr;
	}
	
	template<typename T>
	void RemoveComponent()
	{
		m_Components.erase(std::type_index(typeid(T)));
	}

	template<typename T>
	bool HasComponent()
	{
		for(auto& component : m_Components)
		{
			if(dynamic_cast<T*>(component.get()) != nullptr)
			{
				return true;
			}
		}
		return false;
	}
#pragma endregion
	
	GameObject() = default;
	virtual ~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	const Transform& GetTransform()const {return m_transform;}

	GameObject* GetParent() const {return m_Parent;}
	void SetParent(GameObject* parent,bool keepWorldPosition);

	int GetChildCount()const {return static_cast<int>(m_Children.size());}
	GameObject* GetChildAt(int index) const;

	bool IsDescendant(GameObject* child)const;

	void SetLocalPosition(const glm::vec2& pos);
	
	void SetPositionDirty();
	const glm::vec3& GetWorldPosition();
	void UpdateWorldPosition();


private:
	Transform m_transform{};

	void AddChild(GameObject* child,bool keepWorldPosition);
	void RemoveChild(GameObject* child);
	GameObject* m_Parent = nullptr;
	std::vector<GameObject*> m_Children;
	std::vector<std::unique_ptr<BaseComponent>> m_Components;
	bool m_IsPositionDirty = false;
};
