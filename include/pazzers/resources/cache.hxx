
#ifndef PAZZERS_RESOURCES_CACHE_HXX
#define PAZZERS_RESOURCES_CACHE_HXX

#include <pazzers/resources/image.hxx>

namespace pazzers
{
    namespace resources
    {
        namespace cache
        {
            const Image& get_image(const std::string& id);
            void free_all();
        }
    }
}

#endif
