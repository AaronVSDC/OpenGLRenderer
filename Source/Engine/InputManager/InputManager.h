#include "Singleton.h"
namespace Papyrus
{
    struct MouseDelta
    {
        float x = 0.0f;
        float y = 0.0f;
    };

    class InputManager final : public Singleton<InputManager>
    {
    public:
        bool processInput();
        MouseDelta getMouseDelta() const { return m_MouseDelta; }

    private:
        MouseDelta m_MouseDelta;

    };

}