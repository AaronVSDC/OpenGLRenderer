#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include <string>
#include <vector>
 
#include "GameObject.h"
 
namespace Papyrus
{ 
	class CameraComponent; 
	class SceneManager;
	class Scene final
	{
		friend class SceneManager;
	public:
		void add(std::unique_ptr<GameObject> object);
		void removeAll();

		void update(float deltaTime);
		void fixedUpdate(float fixedTimeStep);
		void render() const;
		void start();
		void onEnable();
		void onDisable();


		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void setIsActive(bool value) { m_IsActive = value;  }
		bool getIsActive() const { return m_IsActive;  }
		CameraComponent* getActiveCamera();


		std::vector<std::unique_ptr<GameObject>>& getGameObjects() { return m_Objects;  }

	private:
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::unique_ptr<GameObject>> m_Objects{};

		static unsigned int m_idCounter;

		bool m_IsActive; 

	};

}
#endif SCENE_H