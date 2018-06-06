#include <pazzers/pazzer.hxx>
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
        in_movement(false)
    {
        mun = 2;
        pac = 0;
        mun_max = mun;
        power = 4;
        atk = 26;
        def = 1;
        life = 100;
        count = 0;
        time = SDL_GetTicks() - 4000;
        xy[0].x = id * 100;
        xy[0].y = id * 100;
        message.time = -1;
        dead = -1;
        dmg = 0;
        speed = 4;
    }

    Pazzer::~Pazzer() = default;

    void Pazzer::status()
    {

    }

    void Pazzer::make_fun(int phase)
    {
        phase -= dead;

        if (phase < 1500)
        {
            system::window->apply(image, pazzer_views[4][(phase / 250) % 2], xy[0].x, xy[0].y - 40);
        }
        else if (phase < 1750)
        {
            system::window->apply(image, pazzer_views[4][2], xy[0].x, xy[0].y - 40);
        }
        else if (phase < 1900)
        {
            system::window->apply(image, pazzer_views[5][2], xy[0].x, xy[0].y - 40);
        }
        else if (xy[0].y > -50)
        {
            system::window->apply(image, pazzer_views[5][(phase / 100) % 2], xy[0].x, xy[0].y - 40);

            xy[0].y -= count / 2;

            if (phase > 2000)
                count += 1;
        }
        else
            dead = -2;
    }

    void Pazzer::update(float delta)
    {
        if (dead != -1)
            return;

        if (controller->is_bomb_drop_required())
        {
            // ...
        }

        auto new_direction = controller->get_direction();

        if (new_direction == geometry::Direction::NONE)
            in_movement = false;
        else
        {
            direction = new_direction;
            in_movement = true;
        }
    }

    int Pazzer::alive(Uint8 i)
    {
        if (!in_movement)
            return 0;

        if (i < 4)
            return 0;
        else if (i < 12)
            return 1;
        else if (i < 16)
            return 0;
        else
            return 2;
    }

    void Pazzer::show()
    {
        const int view_index = pazzer_view_index_from_direction(direction);

        system::window->apply(image, pazzer_views[view_index][alive(++count)], xy[0].x, xy[0].y - 40);
        count %= 22;
    }
}