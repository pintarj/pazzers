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

    static void show_num(int x, int y, const char* str)
    {
        int start = 0;
        int color;

        if (str[0] == '-')
        {
            color = 1;
            window->apply(*number, *numbs[1][10], x, y);
            start = 1;
        }
        else
        {
            color = 0;

            if (str[0] == '+')
            {
                window->apply(*number, *numbs[0][10], x, y);
                start = 1;
            }
        }

        for (int i = start; str[i] != '\0'; ++i)
            window->apply(*number, *numbs[color][str[i] - '0'], x + (i * 10), y);
    }

    static void ApplySurfaceSDL(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
        SDL_Rect offset;

        offset.x = x;
        offset.y = y - 40;
        SDL_BlitSurface(source, clip, destination, &offset);
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
        xy[0].x = initxy[id][0];
        xy[0].y = initxy[id][1];
        message.time = -1;
        dead = -1;
        dmg = 0;
        speed = 120 / FPS;
    }

    Pazzer::~Pazzer() = default;

    void Pazzer::status()
    {
        const SDL_Color black = {0, 0, 0};
        char str[10];

        window->apply(*status_img, 17, 10 + id * 190);
        window->apply(image, pazzer_views[dead == -2 ? 5 : 0][0], 67, 30 + id * 190);
        geometry::Rectangle life_clip(2, 0, life, 19);
        window->apply(*life_img, life_clip, 54, 90 + id * 190);

        SDL_Surface* text = nullptr;

        sprintf(str, "%d", life);
        text = TTF_RenderText_Solid(font1, (const char*) str, black);
        ApplySurfaceSDL(27, id * 190 + 130, text, window->surface, nullptr);
        SDL_FreeSurface(text);

        sprintf(str, "%d", atk);
        text = TTF_RenderText_Solid(font1, (const char*) str, black);
        ApplySurfaceSDL(atk > 9 ? 106 : 109, id * 190 + 156, text, window->surface, nullptr);
        SDL_FreeSurface(text);

        sprintf(str, "%d", def);
        text = TTF_RenderText_Solid(font1, (const char*) str, black);
        ApplySurfaceSDL(def > 9 ? 106 : 109, id * 190 + 183, text, window->surface, nullptr);
        SDL_FreeSurface(text);

        sprintf(str, "%d", mun_max);
        text = TTF_RenderText_Solid(font2, (const char*) str, black);
        ApplySurfaceSDL(mun_max > 9 ? 45 : 47, id * 190 + 153, text, window->surface, nullptr);
        SDL_FreeSurface(text);

        sprintf(str, "%d", power);
        text = TTF_RenderText_Solid(font2, (const char*) str, black);
        ApplySurfaceSDL(power > 9 ? 45 : 47, id * 190 + 170, text, window->surface, nullptr);
        SDL_FreeSurface(text);

        sprintf(str, "%d", speed);
        text = TTF_RenderText_Solid(font2, (const char*) str, black);
        ApplySurfaceSDL(speed > 9 ? 45 : 47, id * 190 + 187, text, window->surface, nullptr);
        SDL_FreeSurface(text);
    }

    void Pazzer::make_fun(int phase)
    {
        phase -= dead;

        if (phase < 1500)
        {
            window->apply(image, pazzer_views[4][(phase / 250) % 2], xy[0].x, xy[0].y - 40);
        }
        else if (phase < 1750)
        {
            window->apply(image, pazzer_views[4][2], xy[0].x, xy[0].y - 40);
        }
        else if (phase < 1900)
        {
            window->apply(image, pazzer_views[5][2], xy[0].x, xy[0].y - 40);
        }
        else if (xy[0].y > -50)
        {
            window->apply(image, pazzer_views[5][(phase / 100) % 2], xy[0].x, xy[0].y - 40);

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
            if (area[xy[3].x][xy[3].y].type == 0 && mun > 0)
            {
                area[xy[3].x][xy[3].y].owner = id;
                area[xy[3].x][xy[3].y].type = 1;
                area[xy[3].x][xy[3].y].time = SDL_GetTicks();
                area[xy[3].x][xy[3].y].power = power;
                area[xy[3].x][xy[3].y].img = 0;
                area[xy[3].x][xy[3].y].atk = atk;
                mun--;
            }
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

        if (pac)
            // should draw pacman
            window->apply(image, pazzer_views[view_index][alive(++count)], xy[0].x, xy[0].y - 40);
        else
            // should blink with negative
            window->apply(image, pazzer_views[view_index][alive(++count)], xy[0].x, xy[0].y - 40);

        count %= 22;

        if (message.time != -1)
        {
            show_num(message.xy.x, message.xy.y, message.text);
            --message.xy.y;

            if (SDL_GetTicks() - message.time > 1000)
                message.time = -1;
        }
    }


    void Pazzer::move()
    {
        if (in_movement)
        {
            switch (direction)
            {
                case geometry::Direction::UP:
                    xy[0].y += -speed;
                    break;

                case geometry::Direction::DOWN:
                    xy[0].y += speed;
                    break;

                case geometry::Direction::LEFT:
                    xy[0].x += -speed;
                    break;

                case geometry::Direction::RIGHT:
                    xy[0].x += speed;
                    break;

                default:
                    break;
            }
        }

        xy[1].x = xy[0].x + 2;
        xy[1].y = xy[0].y + 12;
        xy[2].x = xy[0].x + 38;
        xy[2].y = xy[0].y + 49;
        xy[3].x = (((xy[1].x + xy[2].x) / 2) - FX1) / 40;
        xy[3].y = (((xy[1].y + xy[2].y) / 2) - FY1) / 40;
        check();
    }


    void Pazzer::check()
    {
        int i, j;

        if (xy[1].x < FX1)
        {
            xy[0].x += FX1 - xy[1].x;
            xy[1].x = xy[0].x + 2;
        }

        if (xy[1].y < FY1)
        {
            xy[0].y += FY1 - xy[1].y;
            xy[1].y = xy[0].y + 12;
        }

        if (xy[2].x > FX2)
        {
            xy[0].x += FX2 - xy[2].x;
            xy[2].x = xy[0].x + 38;
        }

        if (xy[2].y > FY2)
        {
            xy[0].y += FY2 - xy[2].y;
            xy[2].y = xy[0].y + 49;
        }

        switch (direction)
        {
            case geometry::Direction::UP:
            {
                int my_x = (((xy[1].x - 215) / 80) * 80 + 255);
                if ((((xy[1].y - 40) / 40) % 2 == 0) &&
                    ((((xy[1].x > my_x) && (xy[1].x < my_x + 39))) || (((xy[2].x > my_x) || (xy[2].x > my_x + 39)))))
                {
                    xy[0].y += speed;
                    if (xy[1].x - my_x < -5) xy[0].x += -4;
                    if (my_x + 39 - xy[2].x < -5) xy[0].x += 4;
                }
                if (((area[xy[3].x][xy[3].y - 1].type == WALL) || (area[xy[3].x][xy[3].y - 1].type == BOMB)) &&
                    (xy[2].y < (FY1 + 40 * (xy[3].y + 1))))
                {
                    xy[0].y -= -speed;
                }
                break;
            }

            case geometry::Direction::DOWN:
            {
                int my_x = (((xy[1].x - 215) / 80) * 80 + 255);
                if ((((xy[2].y - 40) / 40) % 2 == 0) &&
                    ((((xy[1].x > my_x) && (xy[1].x < my_x + 39))) || (((xy[2].x > my_x) || (xy[2].x > my_x + 39)))))
                {
                    xy[0].y += -speed;
                    if (xy[1].x - my_x < -5) xy[0].x += -4;
                    if (my_x + 39 - xy[2].x < -5) xy[0].x += 4;
                }
                if (((area[xy[3].x][xy[3].y + 1].type == WALL) || (area[xy[3].x][xy[3].y + 1].type == BOMB)) &&
                    (xy[2].y > (FY1 + 40 * (xy[3].y + 1))))
                {
                    xy[0].y -= speed;
                }
                break;
            }

            case geometry::Direction::RIGHT:
            {
                int my_y = (40 + 80 * (xy[1].y / 80));
                if ((((xy[2].x - 175) / 40) % 2 == 0) &&
                    ((((xy[1].y < my_y + 39) && (xy[1].y > my_y))) || (((xy[2].y < my_y + 39) && (xy[2].y > my_y)))))
                {
                    xy[0].x += -speed;
                    if (xy[1].y - my_y < -5) xy[0].y += -4;
                    if (my_y + 39 - xy[2].y < -5) xy[0].y += 4;
                }
                if (xy[3].x != 18)
                    if (((area[xy[3].x + 1][xy[3].y].type == WALL) || (area[xy[3].x + 1][xy[3].y].type == BOMB)) &&
                        (xy[2].x > (FX1 + 40 * (xy[3].x + 1))))
                    {
                        xy[0].x -= speed;
                    }
                break;
            }

            case geometry::Direction::LEFT:
            {
                int my_y = (40 + 80 * (xy[1].y / 80));
                if ((((xy[1].x - 175) / 40) % 2 == 0) &&
                    ((((xy[1].y < my_y + 39) && (xy[1].y > my_y))) || (((xy[2].y < my_y + 39) && (xy[2].y > my_y)))))
                {
                    xy[0].x += speed;
                    if (xy[1].y - my_y < -5) xy[0].y += -4;
                    if (my_y + 39 - xy[2].y < -5) xy[0].y += 4;
                }
                if (((area[xy[3].x - 1][xy[3].y].type == WALL) || (area[xy[3].x - 1][xy[3].y].type == BOMB)) &&
                    (xy[2].x < (FX1 + 40 * (xy[3].x + 1))))
                {
                    xy[0].x -= -speed;
                }
                break;
            }

            default:
                break;
        }

        if (pac)
        {
            pacXY.x = xy[3].x;
            pacXY.y = xy[3].y;
            if (SDL_GetTicks() - time > 10000)
            {
                pacXY.x = -1;
                pacXY.y = -1;
                pac = 0;
                pro_pac = 0;
                speed -= 3;
            }
        }
        else
        {
            if (area[xy[3].x][xy[3].y].type == EXPL && SDL_GetTicks() - time > 4500)
            {
                if (area[xy[3].x][xy[3].y].atk <= 0 && abs(area[xy[3].x][xy[3].y].atk) != id)
                {
                    life /= 2;
                    message.time = SDL_GetTicks();
                    sprintf(message.text, "-%d", life);
                    message.xy.x = xy[0].x;
                    message.xy.y = xy[0].y - 20;
                    dmg = 1;
                    time = area[xy[3].x][xy[3].y].time;
                }
                else if (area[xy[3].x][xy[3].y].atk > 0)
                {
                    life -= area[xy[3].x][xy[3].y].atk - def;
                    message.time = SDL_GetTicks();
                    sprintf(message.text, "-%d", area[xy[3].x][xy[3].y].atk - def);
                    message.xy.x = xy[0].x;
                    message.xy.y = xy[0].y - 20;
                    dmg = 1;
                    time = area[xy[3].x][xy[3].y].time;
                }
            }
        }

        if (SDL_GetTicks() - time > 4500 && dmg == 1)
            dmg = 0;

        if (life < 0)
            life = 0;
        else if (area[xy[3].x][xy[3].y].type == BS_BOMB)
        {
            area[xy[3].x][xy[3].y].type = FREE;
            mun++;
            mun_max++;
            message.time = SDL_GetTicks();
            sprintf(message.text, "%d", mun_max);
            message.xy.x = xy[0].x;
            message.xy.y = xy[0].y - 20;
        }
        else if (area[xy[3].x][xy[3].y].type == BS_FIRE)
        {
            area[xy[3].x][xy[3].y].type = FREE;
            power++;
            message.time = SDL_GetTicks();
            sprintf(message.text, "%d", power);
            message.xy.x = xy[0].x;
            message.xy.y = xy[0].y - 20;
        }
        else if (area[xy[3].x][xy[3].y].type == BS_PAC)
        {
            area[xy[3].x][xy[3].y].type = FREE;
            time = SDL_GetTicks();
            if (!pac)
            {
                pac = 1;
                speed += 3;
            }
        }
        else if (area[xy[3].x][xy[3].y].type == BS_ROLL)
        {
            area[xy[3].x][xy[3].y].type = FREE;
            if (speed < 9) speed++;
            message.time = SDL_GetTicks();
            sprintf(message.text, "%d", speed);
            message.xy.x = xy[0].x;
            message.xy.y = xy[0].y - 20;
        }
        else if (area[xy[3].x][xy[3].y].type == BS_ATK)
        {
            area[xy[3].x][xy[3].y].type = FREE;
            atk += 9;
            message.time = SDL_GetTicks();
            sprintf(message.text, "%d", atk);
            message.xy.x = xy[0].x;
            message.xy.y = xy[0].y - 20;
        }
        else if (area[xy[3].x][xy[3].y].type == BS_DEF)
        {
            area[xy[3].x][xy[3].y].type = FREE;
            def += 4;
            message.time = SDL_GetTicks();
            sprintf(message.text, "%d", def);
            message.xy.x = xy[0].x;
            message.xy.y = xy[0].y - 20;
        }
        else if (area[xy[3].x][xy[3].y].type == BS_SKY)
        {
            area[xy[3].x][xy[3].y].type = FREE;
            for (i = 0; i < 19; i++)
            {
                for (j = 0; j < 17; j++)
                {
                    if ((!(j % 2 == 1 && i % 2 == 1)) && (rand() % 100 < 7) && area[i][j].type == FREE)
                    {
                        area[i][j].type = SKY;
                        area[i][j].time = SDL_GetTicks();
                        area[i][j].power = 20;
                        area[i][j].atk = -id;
                    }
                }
            }
        }
        else if (pacXY.x == xy[3].x && pacXY.y == xy[3].y && !pac)
        {
            life = 0;
        }
        if (life < 1)
        {
            dead = SDL_GetTicks();
            count = 0;
        }
    }
}