
#ifndef PAZZERS_SYSTEM_HXX
#define PAZZERS_SYSTEM_HXX

#include <pazzers/resources/image.hxx>

namespace pazzers
{
    namespace system
    {
        /**
         * \brief The image representing the SDL window.
         * */
        extern resources::Image* window;

        /**
         * \brief Initialize the game system.
         * \return True on success, false on failure.
         * */
        bool initialize();

        /**
         * \brief Release the used resources by the system.
         * */
        void destroy();
    }
}

#endif
