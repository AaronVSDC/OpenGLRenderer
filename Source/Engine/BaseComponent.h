#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H
#include <cassert>

namespace Papyrus
{
	class GameObject;
	class BaseComponent
	{
		friend class GameObject;
	public:

		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		virtual void update(float) {}
		virtual void fixedUpdate(float) {}
		virtual void render() const {}
		virtual void start() {}
		virtual void onEnable() {}
		virtual void onDisable() {}

		virtual GameObject* getOwner() const { return m_pOwner; }
		void setOwner(GameObject* owner)
		{
			assert(owner != nullptr && "BaseComponent::setOwner: owner is nullptr");
			if (!owner)
			{
				return;
			}
			m_pOwner = owner;
			// Note: SetOwner is called by GameObject::AddComponent<T> after the component is created
		}
		//void setEnabled(bool enabled)
		//{
		//	m_Enabled = enabled;
		//}
	protected:
		//BaseComponent(GameObject& owner) { m_pOwner = &owner; }
		BaseComponent() = default;

	private:
		GameObject* m_pOwner = nullptr;
	};
}
#endif