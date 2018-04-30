
#ifndef PAZZERS_GAME_VIRTUALCONTROLLER_HXX
#define PAZZERS_GAME_VIRTUALCONTROLLER_HXX

#include <pazzers/geometry/direction.hxx>

namespace pazzers
{
    namespace game
    {
        /**
         * \brief Abstraction of a game controller.
         * */
        class VirtualController
        {
        public:
            /**
             * Create a virtual controller.
             * */
            VirtualController();

            /**
             * \brief Return the direction that is applied on the controller.
             * \return The direction.
             * */
            geometry::Direction get_direction() const;

            /**
             * \brief Tells if a bomb drop was required on the controller.
             * \return True if it's required, false otherwise.
             *
             * This method resets the "required" flag after it's invoked.
             * */
            bool is_bomb_drop_required();

        protected:
            /**
             * \brief Sets the direction that is applied on the controller.
             * \param direction The direction.
             * */
            void set_direction(geometry::Direction direction);

            /**
             * \brief Tells that a bomb drop was required on the controller.
             * */
            void require_bomb_drop();

        private:
            /**
             * \brief The direction that is applied on the controller.
             * */
            geometry::Direction direction;

            /**
             * \brief Tells if a bomb drop was required on the controller.
             * */
            bool bomb_drop_required;
        };
    }
}

#endif
