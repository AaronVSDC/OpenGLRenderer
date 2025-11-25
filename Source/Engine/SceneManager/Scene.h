#ifndef SCENE_H
#define SCENE_H
#include <memory>
#include <string>
#include <vector>
 
#include "GameObject.h"
 
namespace Papyrus
{ 
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

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::unique_ptr<GameObject>> m_Objects{};

		static unsigned int m_idCounter;

	};

}
#endif SCENE_H