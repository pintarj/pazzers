#include <pazzers/game/grass-cell.hxx>
#include <pazzers/resources/cache.hxx>

namespace pazzers
{
    namespace game
    {
        GrassCell::GrassCell(pazzers::game::Field* field, pazzers::XY position):
            Cell(field, position),
            grass_cell_image(resources::cache::get_image("res/arena/cells/grass.bmp"))
        {

        }

        void GrassCell::static_draw()
        {
            this->field->get_image().apply(this->grass_cell_image, 40 * position.x, 40 * position.y);
        }
    }
}
