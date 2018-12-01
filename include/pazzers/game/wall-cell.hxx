
#ifndef PAZZERS_GAME_WALLCELL_HXX
#define PAZZERS_GAME_WALLCELL_HXX

#include <pazzers/game/cell.hxx>
#include <pazzers/resources/image.hxx>

namespace pazzers
{
    namespace game
    {
        /**
         * \brief A single wall-cell in a game.
         * */
        class WallCell: public Cell
        {
        public:
            /**
             * \brief Construct a new wall-cell for the specified field, at the specified position.
             * \param field The specified field.
             * \param position The specified position.
             * */
            WallCell(Field* field, XY position);

            /**
             * \brief Draws the cell on the field.
             * */
            void static_draw() override;

            /**
             * \brief Default implementation.
             * */
            virtual ~WallCell() = default;

        private:
            /**
             * \brief The wall-cell image.
             * */
            const resources::Image& wall_cell_image;
        };
    }
}

#endif
