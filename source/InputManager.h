#include "Singleton.h"
namespace Papyrus
{
    class InputManager final : public Singleton<InputManager>
    {
    public:
        bool processInput();
    private:

    };

}