#include <pazzers/pazzer.hxx>
#include <pazzers/game/cell.hxx>
#include <pazzers/resources/cache.hxx>

namespace pazzers
{
    static const geometry::Rectangle pazzer_views[6][3] =
        {
            {{0,   0, 40, 50}, {40,   0, 40, 50}, {80,   0, 40, 50}},
            {{0,  50, 40, 50}, {40,  50, 40, 50}, {80,  50, 40, 50}},
            {{0, 100, 40, 50}, {40, 100, 40, 50}, {80, 100, 40, 50}},
            {{0, 150, 40, 50}, {40, 150, 40, 50}, {80, 150, 40, 50}},
            {{0, 200, 40, 50}, {40, 200, 40, 50}, {80, 200, 40, 50}},
            {{0, 250, 40, 50}, {40, 250, 40, 50}, {80, 250, 40, 50}}
        };

    static std::vector<const PazzerDescriptor*> descriptors;
    static int last_id = -1;

    static int pazzer_view_index_from_direction(geometry::Direction direction)
    {
        using namespace geometry;

        switch (direction)
        {
            case Direction::UP:
                return 1;

            case Direction::RIGHT:
                return 2;

            case Direction::DOWN:
                return 0;

            case Direction::LEFT:
                return 3;

            default:
                // TODO Should throw error.
                return 0;
        }
    }

    PazzerDescriptor::PazzerDescriptor(const std::string& name, const std::string& image_path):
        name(name),
        image_path(image_path)
    {

    }

    const std::vector<const PazzerDescriptor*>& PazzerDescriptor::get_all()
    {
        static PazzerDescriptor core_descriptors[] =
            {
                {"Claso", "res/pazzer/claso.bmp"},
                {"Darki", "res/pazzer/darki.bmp"},
                {"Mexe", "res/pazzer/mexe.bmp"},
                {"Riba", "res/pazzer/riba.bmp"},
            };

        static bool descriptors_filled = false;

        if (!descriptors_filled)
        {
            for (int i = 0, n = sizeof(core_descriptors) / sizeof(PazzerDescriptor); i < n; ++i)
                descriptors.emplace_back(&core_descriptors[i]);

            descriptors_filled = true;
        }

        return descriptors;
    }

    Pazzer::Pazzer(const PazzerDescriptor& descriptor, game::VirtualController* controller):
        id(++last_id),
        descriptor(descriptor),
        controller(controller),
        image(resources::cache::get_image(descriptor.image_path)),
        direction(geometry::Direction::DOWN),
        in_movement(false),
        field_x(0),
        field_y(0),
        clipping_accumulator(0),
        life_points(100)
    {

    }

    Pazzer::~Pazzer() = default;

    void Pazzer::update(float delta)
    {
        if (is_out_of_game())
            return;

        if (controller->is_bomb_drop_required())
        {
            // ...
        }

        auto new_direction = controller->get_direction();

        if (new_direction == geometry::Direction::NONE)
        {
            in_movement = false;
            clipping_accumulator = 0;
        }
        else
        {
            direction = new_direction;
            in_movement = true;
            apply_movement(delta);
        }
    }

    void Pazzer::draw(const XY& field_origin)
    {
        if (is_out_of_game())
            return;

        const int view_index = pazzer_view_index_from_direction(direction);
        const auto& view = pazzer_views[view_index][get_clip_index()];
        const XY draw_position = {field_origin.x + (int) field_x, field_origin.y + (int) field_y - 10};
        system::window->apply(image, view, draw_position.x, draw_position.y);
    }

    bool Pazzer::is_out_of_game() const noexcept
    {
        return life_points <= 0;
    }

    void Pazzer::apply_movement(float delta)
    {
        auto distance = delta * (PAZZERS_GAME_CELL_SIZE * 3.0f);
        clipping_accumulator += distance;

        switch (direction)
        {
            case geometry::Direction::UP:
                field_y -= distance;
                break;

            case geometry::Direction::RIGHT:
                field_x += distance;
                break;

            case geometry::Direction::DOWN:
                field_y += distance;
                break;

            case geometry::Direction::LEFT:
                field_x -= distance;
                break;
        }
    }

    int Pazzer::get_clip_index()
    {
        if (!in_movement)
            return 0;

        const int time_unit = (PAZZERS_GAME_CELL_SIZE * 1000) / 4200;
        const int time_total = 6 * time_unit;

        while (clipping_accumulator >= time_total)
            clipping_accumulator -= time_total;

        const auto accumulated = (int) clipping_accumulator;

        if (accumulated < (1 * time_unit))
            return 0;

        if (accumulated < (3 * time_unit))
            return 1;

        if (accumulated < (4 * time_unit))
            return 0;

        return 2;
    }
}