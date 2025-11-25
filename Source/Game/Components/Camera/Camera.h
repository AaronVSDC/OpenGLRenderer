#ifndef CAMERA_H
#define CAMERA_H
#include "BaseComponent.h"

namespace Papyrus
{
	class Camera final : public BaseComponent
	{

	public:

		Camera();
		~Camera();
		Camera& operator=(const Camera& other) = delete;
		Camera& operator=(const Camera&& other) = delete;
		Camera(const Camera& rhs) = delete;
		Camera(const Camera&& rhs) = delete;





	private:


	};
}

#endif
