
#ifndef PAZZERS_GAME_CELL_HXX
#define PAZZERS_GAME_CELL_HXX

namespace pazzers
{
    namespace game
    {
        class Cell;
    }
}

#include <pazzers/game/field.hxx>
#include <pazzers/geometry/direction.hxx>
#include <pazzers/geometry/xy.hxx>

namespace pazzers
{
    namespace game
    {
        /**
         * \brief A single cell in a game.
         * */
        class Cell
        {
            friend Field;

        public:
            /**
             * \brief The position of the cell.
             * */
            const XY position;

            /**
             * \brief Default implementation.
             * */
            virtual ~Cell() = default;

            /**
             * \brief Get a cell's neighbour given the direction.
             * \param direction The direction of the neighbour cell.
             * \return The neighbour cell, or nullptr if there is no neighbour.
             *
             * If Direction::NONE is passed as direction, nullptr will be returned.
             * */
            Cell* get_neighbour(geometry::Direction direction);

            /**
             * \brief Draws the cell on the field.
             * */
            virtual void draw() = 0;

        protected:
            /**
             * \brief Create a new cell of specified type.
             * \param type The type of the cell.
             * \param position The position of the cell.
             * */
            Cell(Field* field, XY position);

            /**
             * \brief The field that owns this cell.
             * */
            Field* field;

        private:
            /**
             * \brief The neighbour cells of this cell.
             * */
            Cell* neighbours[5];

            /**
             * \brief Set a cell's neighbour given it's direction.
             * \param direction The direction of the neighbour.
             * \param neighbour The neighbour to set.
             * */
            void set_neighbour(geometry::Direction direction, Cell* neighbour);
        };
    }
}

#endif
