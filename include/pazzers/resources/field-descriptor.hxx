
#ifndef PAZZERS_RESOURCES_FIELDDESCRIPTOR_HXX
#define PAZZERS_RESOURCES_FIELDDESCRIPTOR_HXX

#include <vector>
#include <pazzers/resources/cell-descriptor.hxx>

namespace pazzers
{
    namespace resources
    {
        /**
         * \brief Describes a game's field.
         * */
        class FieldDescriptor
        {
        public:
            /**
             * \brief The width of the field. Expressed in cells.
             * */
            const int width;

            /**
             * \brief The height of the field. Expressed in cells.
             * */
            const int height;

            /**
             * \brief The cells of the field. Ordered by rows: the first "n" cells represents the first row, the
             * second "n" cells represent the second row, where "n" is the fields "width".
             * */
            const std::vector<const CellDescriptor*> cells;

            /**
             * \brief Create a field descriptor.
             * */
            FieldDescriptor(int width, int height, std::vector<const CellDescriptor*>&& cells);

            /**
             * \brief Default implementation.
             * */
            virtual ~FieldDescriptor() = default;
        };
    }
}

#endif
