#include <pazzers/image.hxx>
#include <SDL/SDL_image.h>

namespace pazzers
{
    Image::Image(SDL_Surface* surface):
        surface(surface),
        full_view(new ImageView(*this, 0, 0, (std::uint16_t) surface->w, (std::uint16_t) surface->h))
    {

    }

    Image::Image(const std::string& file_name):
        Image(IMG_Load(file_name.c_str()))
    {

    }

    Image::~Image()
    {
        SDL_FreeSurface(surface);
    }

    std::uint16_t Image::get_width() const
    {
        return (std::uint16_t) surface->w;
    }

    std::uint16_t Image::get_height() const
    {
        return (std::uint16_t) surface->h;
    }

    void Image::apply(const ImageView& view, std::uint16_t x, std::uint16_t y)
    {
        this->full_view->apply(view, x, y);
    }

    void Image::apply(const Image& image, std::uint16_t x, std::uint16_t y)
    {
        apply(*image.full_view, x, y);
    }

    ImageView::ImageView(const Image& image, std::uint16_t x, std::uint16_t y, std::uint16_t width, std::uint16_t height):
        image(image)
    {
        rect.x = x;
        rect.y = y;
        rect.w = width;
        rect.h = height;
    }

    ImageView::~ImageView() = default;

    std::uint16_t ImageView::get_width() const
    {
        return rect.w;
    }

    std::uint16_t ImageView::get_height() const
    {
        return rect.h;
    }

    void ImageView::apply(const ImageView& view, std::uint16_t x, std::uint16_t y)
    {
        SDL_Rect position = {};
        position.x = rect.x + x;
        position.y = rect.y + y;
        SDL_BlitSurface(view.image.surface, (SDL_Rect*) &view.rect, this->image.surface, &position);
    }

    void ImageView::apply(const Image& image, std::uint16_t x, std::uint16_t y)
    {
        apply(*image.full_view, x, y);
    }
}
