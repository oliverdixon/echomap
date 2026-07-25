/**
 * @file
 *
 * JSONDeserialiser specification
 *
 * @author Oliver Dixon
 * @date 2026-07-25
 */

#ifndef ECHOMAP_JSONDESERIALISER_HPP
#define ECHOMAP_JSONDESERIALISER_HPP

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)
#include "web/JSONPartialDeserialiser.hpp"
#else
#include "native/JSONFullDeserialiser.hpp"
#endif

namespace echomap
{

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

/**
 * Provides a JSONDeserialiser for the web, where PartialProject objects are required.
 */
using JSONDeserialiser = JSONPartialDeserialiser;

#else

/**
 * Provides a JSONDeserialiser for native platforms, where Project objects are constructed in full.
 */
using JSONDeserialiser = JSONFullDeserialiser;

#endif // __EMSCRIPTEN__

}

#endif // ECHOMAP_JSONDESERIALISER_HPP
