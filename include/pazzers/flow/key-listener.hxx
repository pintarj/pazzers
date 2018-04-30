
#ifndef PAZZERS_FLOW_KEYLISTENER_HXX
#define PAZZERS_FLOW_KEYLISTENER_HXX

#include <set>

namespace pazzers
{
    namespace flow
    {
        /**
         * \brief A key event listener.
         * */
        class KeyListener
        {
        public:
            /**
             * \brief Default implementation.
             * */
            virtual ~KeyListener() = default;

            /**
             * \brief Propagate a "keydown" event.
             * \param key The key.
             *
             * This method is meant to be redefined, but the super-method should be always called.
             * */
            virtual void on_keydown(int key);

            /**
             * \brief Propagate a "keyup" event.
             * \param key The key.
             *
             * This method is meant to be redefined, but the super-method should be always called.
             * */
            virtual void on_keyup(int key);

            /**
             * \brief The listeners that will be called with super-methods on_keydown() and on_keyup().
             * */
            std::set<KeyListener*> listeners;
        };
    }
}

#endif
