#include <pazzers/resources/field-descriptor.hxx>

namespace pazzers
{
    namespace resources
    {
        FieldDescriptor::FieldDescriptor(int width, int height, std::vector<const CellDescriptor*>&& cells):
            width(width),
            height(height),
            cells(std::move(cells))
        {

        }
    }
}
