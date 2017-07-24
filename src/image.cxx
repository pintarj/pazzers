#include <pazzers/image.hxx>
#include <SDL/SDL_image.h>

namespace pazzers
{
    Image::Image(SDL_Surface* surface):
        surface(surface),
        fullView(new ImageView(*this, 0, 0, (std::uint16_t) surface->w, (std::uint16_t) surface->h))
    {

    }

    Image::Image(const std::string& fileName):
        Image(IMG_Load(fileName.c_str()))
    {

    }

    Image::~Image()
    {
        SDL_FreeSurface(surface);
    }

    std::uint16_t Image::getWidth() const
    {
        return (std::uint16_t) surface->w;
    }

    std::uint16_t Image::getHeight() const
    {
        return (std::uint16_t) surface->h;
    }

    void Image::apply(const ImageView& view, std::uint16_t x, std::uint16_t y)
    {
        this->fullView->apply(view, x, y);
    }

    void Image::apply(const Image& image, std::uint16_t x, std::uint16_t y)
    {
        apply(*image.fullView, x, y);
    }

    ImageView::ImageView(const Image& image, std::uint16_t x, std::uint16_t y, std::uint16_t width, std::uint16_t height):
        image(image)
    {
        rect.x = x;
        rect.y = y;
        rect.w = width;
        rect.h = height;
    }

    ImageView::~ImageView()
    {

    }

    std::uint16_t ImageView::getWidth() const
    {
        return rect.w;
    }

    std::uint16_t ImageView::getHeight() const
    {
        return rect.h;
    }

    void ImageView::apply(const ImageView& view, std::uint16_t x, std::uint16_t y)
    {
        SDL_Rect position;
        position.x = rect.x + x;
        position.y = rect.y + y;
        SDL_BlitSurface(view.image.surface, (SDL_Rect*) &view.rect, this->image.surface, &position);
    }

    void ImageView::apply(const Image& image, std::uint16_t x, std::uint16_t y)
    {
        apply(*image.fullView, x, y);
    }
}
