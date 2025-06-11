#include "GameObject.h"
#include "ResourceManager.h"

GameObject::~GameObject() = default;

void GameObject::SetParent(GameObject* parent,bool keepWorldPosition)
{
	if (parent == this || (parent && parent->IsDescendant(this)))
	{
		return;
	}
	
	if(parent==nullptr)
	{
		SetLocalPosition(m_transform.GetWorldPosition());
	}
	else
	{
		if(keepWorldPosition)
			SetLocalPosition(GetWorldPosition()-parent->GetWorldPosition());
		SetPositionDirty();
	}
	if (m_Parent)
	{
		m_Parent->RemoveChild(this);
	}
	m_Parent = parent;

	if (m_Parent)
	{
		m_Parent->m_Children.push_back(this);
	}
}

void GameObject::AddChild(GameObject* child,bool keepWorldPosition)
{
	if(child == nullptr) return;
	child->SetParent(this,keepWorldPosition);
}

void GameObject::RemoveChild(GameObject* child)
{
	if(child == nullptr) return;
	auto it = std::find(m_Children.begin(), m_Children.end(), child);
	if (it != m_Children.end())
	{
		child->SetParent(nullptr,true);
		m_Children.erase(it);
	}
}

GameObject* GameObject::GetChildAt(int index) const
{
	if(index>=0&&static_cast<unsigned>(index) < m_Children.size()&&m_Children.size()!=0)
	{
		return m_Children[index];
	}
	return nullptr;
}

bool GameObject::IsDescendant(GameObject* child) const
{
	for(GameObject* c : m_Children)
	{
		if(child == c) return true;
		if(c->IsDescendant(child)) return true;
	}
	return false;
}

void GameObject::SetLocalPosition(const glm::vec2& pos)
{
	m_transform.SetLocalPosition({ pos,0 });
	SetPositionDirty();
}

void GameObject::SetPositionDirty()
{
	m_IsPositionDirty = true;
	for (auto child : m_Children)
	{
		child->SetPositionDirty();
	}
}

const glm::vec3& GameObject::GetWorldPosition()
{
	if(m_IsPositionDirty)
		UpdateWorldPosition();
	return m_transform.GetWorldPosition();
}

void GameObject::UpdateWorldPosition()
{
	if(m_IsPositionDirty)
	{
		if(m_Parent == nullptr)
			m_transform.SetWorldPosition(m_transform.GetLocalPosition());
		else
			m_transform.SetWorldPosition(m_Parent->GetWorldPosition()+m_transform.GetLocalPosition());
	}
	m_IsPositionDirty = false;
}

void GameObject::Update(float deltaTime)
{
	for(auto& component : m_Components)
	{
		component->Update(deltaTime);
	}
}

void GameObject::Render() const
{
	for(auto& component : m_Components)
	{
		component->Render();
	}
}


void GameObject::SetPosition(float x, float y)
{
	m_transform.SetWorldPosition(glm::vec3(x, y, 0));
}
