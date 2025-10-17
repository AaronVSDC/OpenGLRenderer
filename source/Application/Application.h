#ifndef APPLICATION_H 
#define APPLICATION_H
#include <memory>
namespace Papyrus
{
	class Window;
	class Application final
	{
	public:
		Application();
		Application& operator=(const Application& rhs) = delete;
		Application& operator=(const Application&& rhs) = delete;
		Application(const Application& other) = delete;
		Application(const Application&& other) = delete;
		~Application(); 

		void run();
	private:
		std::unique_ptr<Window> m_Window; 


	};

}
#endif