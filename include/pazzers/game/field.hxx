
#ifndef PAZZERS_GAME_FIELD_HXX
#define PAZZERS_GAME_FIELD_HXX

namespace pazzers
{
    namespace game
    {
        class Field;
    }
}

#include <pazzers/pazzer.hxx>
#include <pazzers/game/cell.hxx>
#include <pazzers/geometry/xy.hxx>
#include <pazzers/resources/image.hxx>
#include <vector>

namespace pazzers
{
    namespace game
    {
        /**
         * \brief Represent a field for a single game.
         * */
        class Field
        {
            friend Cell;

        public:
            /**
             * \brief The width of the field.
             * */
            const int width;

            /**
             * \brief The height of the field.
             * */
            const int height;

            /**
             * \brief The number of total cells.
             * */
            const int total_cells;

            /**
             * \brief Create a new field, with the specified players.
             * \param origin The origin position of the field on the screen.
             * \param players The specified players.
             * */
            Field(XY origin, const std::vector<Pazzer*>& players);

            /**
             * \brief Release the used resources.
             * */
            virtual ~Field();

            /**
             * \brief Return the cell at a specified position.
             * \param position The specific position.
             * \return The cell at the specified position, or nullptr if the cell does not exists (also if position
             *     is not a valid one).
             */
            Cell* get_cell(const XY& position);

            /**
             * \brief Returns the image that represent the current field.
             * */
            resources::Image& get_image();

            /**
             * \brief Draws The field.
             * */
            void draw();

        private:
            /**
             * \brief The origin position of the field on the screen.
             */
            XY origin;

            /**
             * \brief The array that contains all the field's cells.
             *
             * Cells are contained row by row.
             * */
            Cell** const all_cells;

            /**
             * \brief The two-dimensional array that contains all the field's cells.
             * */
            Cell*** const cells;

            /**
             * \brief The image that represent the field.
             * */
            resources::Image* image;
        };
    }
}

#endif
