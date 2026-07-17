/**
 * @file
 *
 * EmscriptenExtra specification
 *
 * @author Oliver Dixon
 * @date 2026-07-17
 */

#ifndef ECHOMAP_EMSCRIPTENEXTRA_HPP
#define ECHOMAP_EMSCRIPTENEXTRA_HPP

#ifdef __EMSCRIPTEN__

#include <type_traits>

namespace echomap::web
{

/**
 * Constexpr absolute value function for pre-C++23 Emscripten compilers.
 *
 * @tparam T Scalar type.
 * @param x Scalar value.
 *
 * @return The absolute value of the given scalar value.
 */
template <
        class T,
        std::enable_if_t<std::is_arithmetic_v<T>>...>
static constexpr auto abs(
        T const& x
) noexcept
{
    return x < 0 ? -x : x;
}

} // namespace echomap::web

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_EMSCRIPTENEXTRA_HPP
