#include <pazzers/resources/cache.hxx>
#include <map>

namespace pazzers
{
    namespace resources
    {
        namespace cache
        {
            static std::map<std::string, const Image*> images;

            const Image& get_image(const std::string& id)
            {
                auto iterator = images.find(id);

                if (iterator != images.end())
                    return *iterator->second;

                const Image* image = new Image(id);
                images[id] = image;
                return *image;
            }

            void free_all()
            {
                for (auto pair : images)
                    delete pair.second;
            }
        }
    }
}
