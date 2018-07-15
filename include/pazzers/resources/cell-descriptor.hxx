
#ifndef PAZZERS_RESOURCES_CELLDESCRIPTOR_HXX
#define PAZZERS_RESOURCES_CELLDESCRIPTOR_HXX

#include <string>

namespace pazzers
{
    namespace resources
    {
        /**
         * \brief Describes a cell.
         * */
        class CellDescriptor
        {
        public:
            /**
             * \brief The path to the image that describes the cell.
             * */
            const std::string image_path;

            /**
             * \brief Tells if something can cross this cell.
             * */
            const bool crossable;

            /**
             * brief Tells if a pazzer can be spawned on this cell.
             * */
            const bool spawnable;

            /**
             * \brief Create a cell descriptor.
             * \param image_path The path to the image that describes the cell.
             * \param crossable Tells if something can cross this cell.
             * \param spawnable Tells if a pazzer can be spawned on this cell.
             * */
            CellDescriptor(const std::string& image_path, bool crossable, bool spawnable = false);

            /**
             * \brief Default implementation.
             * */
            virtual ~CellDescriptor() = default;
        };
    }
}

#endif
