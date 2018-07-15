#include <pazzers/game/cell.hxx>
#include <pazzers/resources/cache.hxx>

namespace pazzers
{
    namespace game
    {
        Cell::Cell(const resources::CellDescriptor& descriptor, XY position):
            descriptor(descriptor),
            position(position),
            image(resources::cache::get_image(descriptor.image_path))
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
