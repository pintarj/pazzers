
#ifndef PAZZERS_GEOMETRY_RECT_HXX
#define PAZZERS_GEOMETRY_RECT_HXX

namespace pazzers
{
    namespace geometry
    {
        /**
         * \brief A class that represent a rectangle in a 2D space.
         * */
        class Rectangle
        {
        public:
            /**
             * \brief The x coordinate of the rectangle.
             * */
            const int x;

            /**
             * \brief The y coordinate of the rectangle.
             * */
            const int y;

            /**
             * \brief The width of the rectangle.
             * */
            const int width;

            /**
             * \brief The height of the rectangle.
             * */
            const int height;

            /**
             * \brief Creates a rectangle with the specified coordinates and dimensions.
             * \param image The image that will have this view associated.
             * \param x The x coordinate of the rectangle.
             * \param y The y coordinate of the rectangle.
             * \param width The width of the rectangle.
             * \param width The height of the rectangle.
             * */
            Rectangle(int x, int y, int width, int height);

            /**
             * \brief Default implementation
             * */
            virtual ~Rectangle() = default;
        };
    }
}

#endif
