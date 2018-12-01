#include <pazzers/game/cell.hxx>

namespace pazzers
{
    namespace game
    {
        Cell::Cell(Field* field, XY position):
            field(field),
            position(position)
        {
            for (auto& neighbour : neighbours)
                neighbour = nullptr;
        }

        Cell* Cell::get_neighbour(geometry::Direction direction)
        {
            return neighbours[(int) direction];
        }

        void Cell::set_neighbour(geometry::Direction direction, Cell* neighbour)
        {
            neighbours[(int) direction] = neighbour;
        }
    }
}
