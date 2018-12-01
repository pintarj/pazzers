
#ifndef PAZZERS_GAME_GRASSCELL_HXX
#define PAZZERS_GAME_GRASSCELL_HXX

#include <pazzers/game/cell.hxx>
#include <pazzers/resources/image.hxx>

namespace pazzers
{
    namespace game
    {
        /**
         * \brief A single grass cell in a game.
         * */
        class GrassCell: public Cell
        {
        public:
            /**
             * \brief Construct a new grass-cell for the specified field, at the specified position.
             * \param field The specified field.
             * \param position The specified position.
             * */
            GrassCell(Field* field, XY position);

            /**
             * \brief Draws the cell on the field.
             * */
            void static_draw() override;

            /**
             * \brief Default implementation.
             * */
            virtual ~GrassCell() = default;

        private:
            /**
             * \brief The grass-cell image.
             * */
            const resources::Image& grass_cell_image;
        };
    }
}

#endif
