#include <pazzers/resources/cell-descriptor.hxx>

namespace pazzers
{
    namespace resources
    {
        CellDescriptor::CellDescriptor(const std::string& image_path, bool crossable, bool spawnable):
            image_path(image_path),
            crossable(crossable),
            spawnable(spawnable)
        {

        }
    }
}
