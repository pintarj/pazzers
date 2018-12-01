#include <pazzers/game/field.hxx>
#include <pazzers/garbage.hxx>
#include <pazzers/resources/cache.hxx>
#include <pazzers/game/grass-cell.hxx>
#include <pazzers/game/wall-cell.hxx>

namespace pazzers
{
    namespace game
    {
        Field::Field(XY origin, const std::vector<Pazzer*>& players):
            width(11),
            height(11),
            total_cells(width * height),
            origin(origin),
            all_cells(new Cell*[total_cells]),
            cells(new Cell**[height]),
            image(new resources::Image(40 * width, 40 * height))
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

                    if (is_wall)
                        cells[y][x] = new WallCell(this, {x, y});
                    else
                        cells[y][x] = new GrassCell(this, {x, y});
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
                }
            }

            for (int i = 0; i < players.size(); ++i)
            {
                static XY spawn_position_lookup [4] =
                    {
                        {1, 1},
                        {9, 9},
                        {9, 1},
                        {1, 9}
                    };

                auto player = players[i];
                player->current_cell = this->get_cell(spawn_position_lookup[i]);
            }

            for (int i = 0; i < height; ++i)
                for (int j = 0; j < width; ++j)
                    this->get_cell({j, i})->static_draw();
        }

        Field::~Field()
        {
            for (int i = 0; i < total_cells; ++i)
                delete all_cells[i];

            delete[] all_cells;
            delete[] cells;
            delete this->image;
        }

        void Field::draw()
        {
            window->apply(*this->image, origin.x, origin.y);

            for (int i = 0; i < height; ++i)
                for (int j = 0; j < width; ++j)
                    this->cells[i][j]->draw();
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

        resources::Image& Field::get_image()
        {
            return *this->image;
        }
    }
}
