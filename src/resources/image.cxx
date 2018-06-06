#include <pazzers/resources/image.hxx>
#include <SDL/SDL_image.h>

namespace pazzers
{
    namespace resources
    {
        Image::Image(int width, int height):
            Image([width, height] () -> SDL_Surface*
                {
                    Uint32 r_mask, g_mask, b_mask, a_mask;

                    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                        r_mask = 0xff000000;
                        g_mask = 0x00ff0000;
                        b_mask = 0x0000ff00;
                        a_mask = 0x000000ff;
                    #else
                        r_mask = 0x000000ff;
                        g_mask = 0x0000ff00;
                        b_mask = 0x00ff0000;
                        a_mask = 0xff000000;
                    #endif

                    SDL_Surface* surface = SDL_CreateRGBSurface(
                            SDL_SWSURFACE,
                            width, height, 32,
                            r_mask, g_mask, b_mask, a_mask
                        );

                    return surface;
                } ())
        {

        }

        Image::Image(SDL_Surface* surface):
            surface(surface),
            full_view_rectangle(new geometry::Rectangle(0, 0, surface->w, surface->h))
        {
            SDL_UnlockSurface(surface);
        }

        Image::Image(const std::string& file_name):
            Image([file_name] () -> SDL_Surface*
                {
                    SDL_Surface* loaded = IMG_Load(file_name.c_str());

                    if (loaded == nullptr)
                        // TODO throw an exeption.
                        return nullptr;

                    SDL_Surface* optimized = SDL_DisplayFormat(loaded);
                    SDL_FreeSurface(loaded);
                    SDL_SetColorKey(optimized, SDL_SRCCOLORKEY, SDL_MapRGB(optimized->format, 0xFF, 0x00, 0xFF));
                    return optimized;
                } ())
        {

        }

        Image::~Image()
        {
            delete full_view_rectangle;
            SDL_FreeSurface(surface);
        }

        int Image::get_width() const
        {
            return surface->w;
        }

        int Image::get_height() const
        {
            return surface->h;
        }

        void Image::apply(const Image& image, const geometry::Rectangle& view, int x, int y)
        {
            SDL_Rect view_rect = {0, 0, 0, 0};
            view_rect.x = (Sint16) view.x;
            view_rect.y = (Sint16) view.y;
            view_rect.w = (Uint16) view.width;
            view_rect.h = (Uint16) view.height;

            SDL_Rect position_rect = {0, 0, 0, 0};
            position_rect.x = (Sint16) x;
            position_rect.y = (Sint16) y;

            SDL_BlitSurface(image.surface, &view_rect, this->surface, &position_rect);
        }

        void Image::apply(const Image& image, int x, int y)
        {
            apply(image, *image.full_view_rectangle, x, y);
        }

        Image* Image::clone() const
        {
            SDL_LockSurface(surface);
            SDL_Surface* cloned_surface = SDL_CreateRGBSurfaceFrom(
                surface->pixels,
                surface->w,
                surface->h,
                surface->format->BitsPerPixel,
                surface->pitch,
                surface->format->Rmask,
                surface->format->Gmask,
                surface->format->Bmask,
                surface->format->Amask
                );
            SDL_UnlockSurface(surface);

            return new Image(cloned_surface);
        }

        void Image::filter(const std::function<void(int, int, std::uint8_t*)>& f)
        {
            // Help resources:
            //     - http://sdl.beuc.net/sdl.wiki/Pixel_Access

            SDL_LockSurface(surface);
            std::function<std::uint32_t(std::uint8_t*)> read;
            std::function<void(std::uint8_t*, std::uint32_t)> write;
            const int bytes_per_pixel = surface->format->BytesPerPixel;
            auto raw_pixels = (Uint8*) surface->pixels;

            switch (surface->format->BytesPerPixel)
            {
                case 3:
                {
                    read = [] (auto raw)
                        {
                            if (SDL_BYTEORDER != SDL_BIG_ENDIAN)
                                return raw[0] << 16 | raw[1] << 8 | raw[2];
                            else
                                return raw[0] | raw[1] << 8 | raw[2] << 16;
                        };

                    write = [] (auto raw, auto data)
                        {
                            if(SDL_BYTEORDER != SDL_BIG_ENDIAN)
                            {
                                raw[0] = (data >> 16) & 0xff;
                                raw[1] = (data >> 8) & 0xff;
                                raw[2] = data & 0xff;
                            }
                            else
                            {
                                raw[0] = data & 0xff;
                                raw[1] = (data >> 8) & 0xff;
                                raw[2] = (data >> 16) & 0xff;
                            }
                        };

                    break;
                }

                default:
                    // TODO Should return an error.
                    return;
            }

            for (int y = 0, h = surface->h; y < h; ++y)
            {
                for (int x = 0, w = surface->w; x < w; ++x)
                {
                    std::uint8_t* raw = raw_pixels + y * surface->pitch + x * bytes_per_pixel;
                    std::uint32_t data = read(raw);
                    f(x, y, (std::uint8_t*) &data);
                    write(raw, data);
                }
            }

            SDL_UnlockSurface(surface);
        }
    }
}
