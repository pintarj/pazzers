#include <pazzers/game/cell.hxx>

namespace pazzers
{
    namespace game
    {
        Cell::Cell(Type type, XY position):
            type(type),
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
