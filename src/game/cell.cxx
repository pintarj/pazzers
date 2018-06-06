#include <pazzers/game/cell.hxx>
#include <pazzers/resources/cache.hxx>

namespace pazzers
{
    namespace game
    {
        Cell::Cell(Type type, XY position):
            type(type),
            position(position),
            image(resources::cache::get_image([type] () -> std::string
                {
                    switch (type)
                    {
                        case Type::FREE: return "res/image/field/free.bmp";
                        case Type::WALL: return "res/image/field/wall.bmp";
                    }
                } ()))
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
