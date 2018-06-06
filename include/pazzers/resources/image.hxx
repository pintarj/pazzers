
#ifndef PAZZERS_RESOURCES_IMAGE_HXX
#define PAZZERS_RESOURCES_IMAGE_HXX

#include <functional>
#include <string>
#include <SDL/SDL_video.h>
#include <pazzers/geometry/rectangle.hxx>

namespace pazzers
{
    namespace resources
    {
        /**
         * \brief Class that represents an image.
         *
         * Each image has an associated "SDL_surface".
         * */
        class Image
        {
        public:
            /**
             * \brief A rectangle which view covers the whole image.
             * */
            const geometry::Rectangle* const full_view_rectangle;

            /**
             * \brief The SDL_surface associated with this image.
             * */
            SDL_Surface* const surface;

            /**
             * \brief Create an image of the specified size.
             * \param width The width of the image.
             * \param height The height of the image.
             *
             * The image content is undefined.
             * */
            Image(int width, int height);

            /**
             * \brief Create a new Image associating it with a specified SDL_surface.
             * \param surface The SDL_surface to associate with.
             * */
            Image(SDL_Surface* surface);

            /**
             * \brief Create a new Image using a specific image file referenced by a filename.
             * \param file_name The filename that reference the image file.
             * */
            Image(const std::string& file_name);

            /**
             * \brief Release the used resources.
             * */
            virtual ~Image();

            /**
             * \brief Returns the image's width.
             * \return The image's width.
             * */
            int get_width() const;

            /**
             * \brief Returns the image's height.
             * \return The image's height.
             * */
            int get_height() const;

            /**
             * \brief Applies a specified image, using a specified view rectangle, on this image
             *     on the specified offset.
             * \param image The image to apply.
             * \param view The rectangle view on the specified image.
             * \param x The x offset on this image.
             * \param y The y offset on this image.
             * */
            void apply(const Image& image, const geometry::Rectangle& view, int x, int y);

            /**
             * \brief Applies a specified image, using it's "full view", on this image on the specified offset.
             * \param image The image to apply.
             * \param x The x offset on this image.
             * \param y The y offset on this image.
             * */
            void apply(const Image& image, int x, int y);

            /**
             * \brief Create a clone of this image.
             * \return The clone.
             *
             * The clone should be then deleted with "delete".
             * */
            Image* clone() const;

            /**
             * \brief Performs a per-pixel filter.
             * \param f The function will accept the x and y coordinates and then the values of the current pixel.
             *     The new values of the pixel have to be stored directly in the third argument.
             * */
            void filter(const std::function<void(int, int, std::uint8_t*)>& f);
        };
    }
}

#endif
