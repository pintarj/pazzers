
#ifndef PAZZERS_IMAGE_HXX
#define PAZZERS_IMAGE_HXX

#include <string>
#include <SDL/SDL_video.h>

namespace pazzers
{
    class ImageView;

    class Image
    {
        public:
            ImageView* const fullView;
            SDL_Surface* const surface;

            Image(SDL_Surface* surface);
            Image(const std::string& fileName);
            virtual ~Image();
            std::uint16_t getWidth() const;
            std::uint16_t getHeight() const;
            void apply(const ImageView& view, std::uint16_t x, std::uint16_t y);
            void apply(const Image& image, std::uint16_t x, std::uint16_t y);
    };

    class ImageView
    {
        private:
            SDL_Rect rect;

        public:
            const Image& image;

            ImageView(const Image& image, std::uint16_t x, std::uint16_t y, std::uint16_t width, std::uint16_t height);
            virtual ~ImageView();
            std::uint16_t getWidth() const;
            std::uint16_t getHeight() const;
            void apply(const ImageView& view, std::uint16_t x, std::uint16_t y);
            void apply(const Image& image, std::uint16_t x, std::uint16_t y);
    };
}

#endif
