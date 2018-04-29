#include <pazzers/pazzer.hxx>

namespace pazzers
{
    static std::vector<const PazzerDescriptor*> descriptors;

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

    void Pazzer::status()
    {
        char str[10];

        ApplySurfaceSDL(17, 50 + id * 190, status_img, window->surface, NULL);
        ApplySurfaceSDL(67, 70 + id * 190, obj, window->surface, dead == -2 ? &obj_clip[5][0] : &obj_clip[0][0]);
        life_clip.w = life;
        ApplySurfaceSDL(54, 130 + id * 190, life_img, window->surface, &life_clip);

        sprintf(str, "%d", life);
        text = TTF_RenderText_Solid(font1, (const char*) str, black);
        ApplySurfaceSDL(27, id * 190 + 130, text, window->surface, NULL);
        SDL_FreeSurface(text);

        sprintf(str, "%d", atk);
        text = TTF_RenderText_Solid(font1, (const char*) str, black);
        ApplySurfaceSDL(atk > 9 ? 106 : 109, id * 190 + 156, text, window->surface, NULL);
        SDL_FreeSurface(text);

        sprintf(str, "%d", def);
        text = TTF_RenderText_Solid(font1, (const char*) str, black);
        ApplySurfaceSDL(def > 9 ? 106 : 109, id * 190 + 183, text, window->surface, NULL);
        SDL_FreeSurface(text);

        sprintf(str, "%d", mun_max);
        text = TTF_RenderText_Solid(font2, (const char*) str, black);
        ApplySurfaceSDL(mun_max > 9 ? 45 : 47, id * 190 + 153, text, window->surface, NULL);
        SDL_FreeSurface(text);

        sprintf(str, "%d", power);
        text = TTF_RenderText_Solid(font2, (const char*) str, black);
        ApplySurfaceSDL(power > 9 ? 45 : 47, id * 190 + 170, text, window->surface, NULL);
        SDL_FreeSurface(text);

        sprintf(str, "%d", speed);
        text = TTF_RenderText_Solid(font2, (const char*) str, black);
        ApplySurfaceSDL(speed > 9 ? 45 : 47, id * 190 + 187, text, window->surface, NULL);
        SDL_FreeSurface(text);

    }


    void Pazzer::make_fun(int phase)
    {
        phase -= dead;
        if (phase < 1500)
        {
            ApplySurfaceSDL(xy[0].x, xy[0].y, obj, window->surface, &obj_clip[4][(phase / 250) % 2]);
        }
        else if (phase < 1750)
        {
            ApplySurfaceSDL(xy[0].x, xy[0].y, obj, window->surface, &obj_clip[4][2]);
        }
        else if (phase < 1900)
        {
            ApplySurfaceSDL(xy[0].x, xy[0].y, obj, window->surface, &obj_clip[5][2]);
        }
        else if (xy[0].y > -50)
        {
            ApplySurfaceSDL(xy[0].x, xy[0].y, obj, window->surface, &obj_clip[5][(phase / 100) % 2]);
            xy[0].y -= count / 2;
            if (phase > 2000) count += 1;
        }
        else dead = -2;
    }


    void Pazzer::initialize(const char* name, int id_)
    {
        Uint8 i, j;

        obj = LoadImageSDL(name);
        sht = LoadImageSDL(name);
        NegativeSDL(sht);
        dir = DOWN;
        mun = 2;
        pac = 0;
        mun_max = mun;
        power = 4;
        atk = 26;
        def = 1;
        life = 100;
        id = id_;
        count = 0;
        time = SDL_GetTicks() - 4000;
        xy[0].x = initxy[id][0];
        xy[0].y = initxy[id][1];
        joy.x = 0;
        joy.y = 0;
        message.time = -1;
        dead = -1;
        dmg = 0;
        speed = 120 / FPS;
        mov = false;
        alter = false;
        up = button[id][0];
        down = button[id][1];
        left = button[id][2];
        right = button[id][3];
        drop = button[id][4];
        for (i = 0; i < 10; i++) cheat[i] = 0;
        for (i = 0; i < 6; i++)
        {
            for (j = 0; j < 3; j++)
            {
                obj_clip[i][j].x = j * 40;
                obj_clip[i][j].y = i * 50;
                obj_clip[i][j].w = 40;
                obj_clip[i][j].h = 50;
            }
        }
        life_clip.x = 2;
        life_clip.y = 0;
        life_clip.h = 19;
    }


    void Pazzer::handle(int msg, int type)
    {
        int i;

        if (alter == true && event.type == SDL_JOYAXISMOTION)
        {
            if (event.jaxis.axis == 0) joy.x = event.jaxis.value;
            if (event.jaxis.axis == 1) joy.y = event.jaxis.value;
            if (abs(joy.x) > 8000 || abs(joy.y) > 8000)
            {
                mov = true;
                if (abs(joy.y) < abs(joy.x))
                {
                    if (joy.x > 0)
                        dir = RIGHT;
                    else
                        dir = LEFT;
                }
                else
                {
                    if (joy.y > 0)
                        dir = DOWN;
                    else
                        dir = UP;
                }
                if (cheat[0] != dir)
                {
                    for (i = 9; i > 0; i--) cheat[i] = cheat[i - 1];
                    cheat[i] = dir;
                }
            }
            else mov = false;
        }
        else if (alter == true && event.type == SDL_JOYBUTTONDOWN)
        {
            if ((event.jbutton.button) && (mun > 0))
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
        }
        else if (alter == false)
        {
            switch (type)
            {
                case SDL_KEYDOWN:
                {
                    if (msg == SDLK_ESCAPE) exit(0);
                    if (msg == SDLK_t) SDL_SaveBMP(window->surface, "shot.bmp");
                    if (msg == up)
                    {
                        dir = UP;
                        mov = true;
                    }
                    if (msg == down)
                    {
                        dir = DOWN;
                        mov = true;
                    }
                    if (msg == left)
                    {
                        dir = LEFT;
                        mov = true;
                    }
                    if (msg == right)
                    {
                        dir = RIGHT;
                        mov = true;
                    }
                    if (msg == drop)
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
                    for (i = 9; i > 0; i--) cheat[i] = cheat[i - 1];
                    cheat[i] = dir;
                    break;
                }
                case SDL_KEYUP:
                {
                    if (((dir == UP) && (msg == up)) ||
                        ((dir == DOWN) && (msg == down)) ||
                        ((dir == LEFT) && (msg == left)) ||
                        ((dir == RIGHT) && (msg == right))
                        )
                        mov = false;
                    break;
                }
            }
        }
    }


    int Pazzer::alive(Uint8 i)
    {
        if (mov == false) return 0;
        if (i < 4)
        { return 0; }
        else if (i < 12)
        { return 1; }
        else if (i < 16)
        { return 0; }
        else if (i < 23)
        { return 2; }
    }


    void Pazzer::show()
    {
        if (pac)
            ApplySurfaceSDL(xy[0].x, xy[0].y, pacman, window->surface, &obj_clip[dir][alive(++count)]);
        else
            ApplySurfaceSDL(xy[0].x, xy[0].y, dmg ? (
                SDL_GetTicks() % 200 > 100 ? obj : sht) : obj, window->surface, &obj_clip[dir][alive(++count)]);
        count %= 22;
        if (message.time != -1)
        {
            show_num(&message.xy.x, &message.xy.y, message.text, message.time);
            if (SDL_GetTicks() - message.time > 1000) message.time = -1;
        }
    }


    void Pazzer::move()
    {
        if (mov == true)
            switch (dir)
            {
                case UP:
                    xy[0].y += -speed;
                    break;
                case DOWN:
                    xy[0].y += speed;
                    break;
                case LEFT:
                    xy[0].x += -speed;
                    break;
                case RIGHT:
                    xy[0].x += speed;
                    break;
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
        switch (dir)
        {
            case UP:
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
            case DOWN:
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
            case RIGHT:
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
            case LEFT:
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
        if (SDL_GetTicks() - time > 4500 && dmg == 1) dmg = 0;
        if (life < 0) life = 0;
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
        for (i = 0; (cheat[i] == i % 2) && (i < 10); i++);
        if (i == 10)
        {
            area[xy[3].x][xy[3].y].type = BS_SKY;
            cheat[0] = 8;
        }
        for (i = 0; (cheat[i] == (i % 2) + 2) && (i < 10); i++);
        if (i == 10)
        {
            life = 75;
            cheat[0] = 8;
        }
    }
}