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
using JSONDeserialiser = JSONPartialDeserialiser;
#else
using JSONDeserialiser = JSONFullDeserialiser;
#endif // __EMSCRIPTEN__

}

#endif // ECHOMAP_JSONDESERIALISER_HPP
