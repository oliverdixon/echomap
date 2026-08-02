/**
 * @file
 *
 * IDAllocator implementation
 *
 * @author Oliver Dixon
 * @date 2026-08-02
 */

#include "IDAllocator.hpp"

#include <doctest/doctest.h>

namespace
{

struct TagA {};
struct TagB {};

}

namespace echomap
{

TEST_SUITE("objects/IDAllocator")
{
    TEST_CASE("IDAllocator allocates monotonically per tag type")
    {
        CHECK_EQ(IDAllocator<TagA>::allocate(), 0);
        CHECK_EQ(IDAllocator<TagB>::allocate(), 0);
        CHECK_EQ(IDAllocator<TagA>::allocate(), 1);
    }
}

}
