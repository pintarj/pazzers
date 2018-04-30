#include <pazzers/game/virtual-controller.hxx>

namespace pazzers
{
    namespace game
    {
        VirtualController::VirtualController():
            direction(geometry::Direction::NONE),
            bomb_drop_required(false)
        {

        }

        geometry::Direction VirtualController::get_direction() const
        {
            return direction;
        }

        bool VirtualController::is_bomb_drop_required()
        {
            bool was_required = bomb_drop_required;
            bomb_drop_required = false;
            return was_required;
        }

        void VirtualController::set_direction(geometry::Direction direction)
        {
            this->direction = direction;
        }

        void VirtualController::require_bomb_drop()
        {
            bomb_drop_required = true;
        }
    }
}
