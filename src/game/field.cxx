#include <pazzers/game/field.hxx>
#include <pazzers/system.hxx>

namespace pazzers
{
    namespace game
    {
        Field::Field():
            width(21),
            height(17),
            total_cells(width * height),
            all_cells(new Cell*[total_cells]),
            cells(new Cell**[height]),
            image(new resources::Image(width * PAZZERS_GAME_CELL_SIZE, height * PAZZERS_GAME_CELL_SIZE))
        {
            for (int y = 0; y < height; ++y)
                cells[y] = &all_cells[y * width];

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    const bool is_wall =
                        y == 0 ||
                        y == height - 1 ||
                        x == 0 ||
                        x == width - 1 ||
                        (x % 2 == 0 && y % 2 == 0);

                    const Cell::Type type = (is_wall) ? Cell::Type::WALL : Cell::Type::FREE;
                    cells[y][x] = new Cell(type, {x, y});
                }
            }

            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    Cell* cell = cells[y][x];
                    cell->set_neighbour(geometry::Direction::UP,    get_cell({x, y - 1}));
                    cell->set_neighbour(geometry::Direction::RIGHT, get_cell({x + 1, y}));
                    cell->set_neighbour(geometry::Direction::DOWN,  get_cell({x, y + 1}));
                    cell->set_neighbour(geometry::Direction::LEFT,  get_cell({x - 1, y}));

                    image->apply(cell->image, x * PAZZERS_GAME_CELL_SIZE, y * PAZZERS_GAME_CELL_SIZE);
                }
            }
        }

        Field::~Field()
        {
            for (int i = 0; i < total_cells; ++i)
                delete all_cells[i];

            delete[] all_cells;
            delete[] cells;
            delete image;
        }

        Cell* Field::get_cell(const XY& position)
        {
            if (position.x < 0 ||
                position.x >= width ||
                position.y < 0 ||
                position.y >= height)
                return nullptr;

            return cells[position.y][position.x];
        }

        void Field::draw(XY position)
        {
            system::window->apply(*image, position.x, position.y);
        }
    }
}
