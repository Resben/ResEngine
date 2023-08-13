/**
 * \file
 * \author Ben Hawkins (34112619)
**/

#include "Types.h"

namespace AEngine
{
    class ResourceAPI
    {
    public:
			/**
			 * \brief Initialises a model loader library
			 * \param[in] loader ModelLoaderLibrary enum
             * \note Temporary implementation
			*/
        static void Initialise(ModelLoaderLibrary loader);
			/**
			 * \brief Get the current model loader library
			 * \return The current model loader library
			*/
        static ModelLoaderLibrary GetLibrary();

    private:
        static ModelLoaderLibrary s_api;
    };
}