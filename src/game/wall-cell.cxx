#include <pazzers/game/wall-cell.hxx>
#include <pazzers/resources/cache.hxx>

namespace pazzers
{
    namespace game
    {
        WallCell::WallCell(pazzers::game::Field* field, pazzers::XY position):
            Cell(field, position),
            wall_cell_image(resources::cache::get_image("res/arena/cells/wall.bmp"))
        {

        }

        void WallCell::draw()
        {
            this->field->get_image().apply(this->wall_cell_image, 40 * position.x, 40 * position.y);
        }
    }
}
