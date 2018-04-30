
#ifndef PAZZERS_GAME_KEYCONTROLLER_HXX
#define PAZZERS_GAME_KEYCONTROLLER_HXX

#include <pazzers/flow/key-listener.hxx>
#include <pazzers/game/virtual-controller.hxx>

namespace pazzers
{
    namespace game
    {
        /**
         * \brief The enumeration of key controller keys map.
         * */
        enum class KeyControllerMap
        {
            ARROWS = 0,
            WASD   = 1,
            IJKL   = 2,
            NUMPAD = 3
        };

        /**
         * \brief A controller, that responds to keys on keyboard.
         * */
        class KeyController: public VirtualController, public flow::KeyListener
        {
        public:
            /**
             * \brief The controller keys map.
             * */
            const KeyControllerMap map;

            /**
             * \brief Create a key controller with the specified keys map.
             * \param map The keys map.
             * */
            KeyController(KeyControllerMap map);

            /**
             * \brief Default implementation.
             * */
            virtual ~KeyController() = default;

            /**
             * \brief Propagate a "keydown" event.
             * \param key The key.
             * */
            virtual void on_keydown(int key) override;

            /**
             * \brief Propagate a "keyup" event.
             * \param key The key.
             * */
            virtual void on_keyup(int key) override;
        };
    }
}

#endif
