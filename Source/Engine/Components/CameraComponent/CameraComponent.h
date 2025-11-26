#ifndef CAMERA_H
#define CAMERA_H
#include "BaseComponent.h"
#include <glm/glm.hpp>

namespace Papyrus
{
	class CameraComponent final : public BaseComponent
	{

	public:

		CameraComponent();
		~CameraComponent() = default;
		CameraComponent& operator=(const CameraComponent& other) = delete;
		CameraComponent& operator=(const CameraComponent&& other) = delete;
		CameraComponent(const CameraComponent& rhs) = delete;
		CameraComponent(const CameraComponent&& rhs) = delete;

		void update(float deltaTime) override;

		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix() const;

		glm::vec3 getFront() const { return m_Front; }
		

	private:

		glm::vec3 m_Front;
		glm::vec3 m_Up;

		float m_Yaw;
		float m_Pitch;

		float m_Fov;

		float m_LastX;
		float m_LastY;
		bool m_FirstMouse;

	};
}

#endif
