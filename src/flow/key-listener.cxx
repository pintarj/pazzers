#include <pazzers/flow/key-listener.hxx>

namespace pazzers
{
    namespace flow
    {
        void KeyListener::on_keydown(int key)
        {
            for (auto& listener : listeners)
                listener->on_keydown(key);
        }

        void KeyListener::on_keyup(int key)
        {
            for (auto& listener : listeners)
                listener->on_keyup(key);
        }
    }
}
