/**
 * @file
 *
 * Object implementation
 *
 * @author Oliver Dixon
 * @date 2026-08-02
 */

#include "Object.hpp"

#include <doctest/doctest.h>

namespace
{

using namespace echomap;

struct TestObject : Object<TestObject>
{
    explicit TestObject(
            const std::string_view name
    ) :
        Object(name)
    {
    }

    TestObject(
            const TestObject& other
    ) :
        Object(CopyTag{},
               other)
    {
    }

    TestObject(
            const TestObject& other,
            const std::string_view new_name
    ) :
        Object(CopyTag{},
               other,
               new_name)
    {
    }

    TestObject(
            TestObject&& other
    ) noexcept :
        Object(std::move(other))
    {
    }

    TestObject& operator=(
            TestObject&& other
    ) noexcept
    {
        if (this == &other)
            return *this;

        move_identity_from(std::move(other));
        return *this;
    }
};

} // namespace

namespace echomap
{

TEST_SUITE(
        "objects/Object"
)
{
    TEST_CASE("Objects receive unique valid runtime IDs")
    {
        const TestObject first{"First"};
        const TestObject second{"Second"};

        CHECK(first.is_valid());
        CHECK(second.is_valid());
        CHECK(first.get_id() != second.get_id());
    }

    TEST_CASE("Copy construction allocates a fresh ID and derives a copy name")
    {
        const TestObject original{"Original"};

        const TestObject first_copy{original}; // NOLINT(*-unnecessary-copy-initialization)
        const TestObject second_copy{original}; // NOLINT(*-unnecessary-copy-initialization)

        CHECK(first_copy.get_id() != original.get_id());
        CHECK(second_copy.get_id() != original.get_id());
        CHECK(first_copy.get_id() != second_copy.get_id());

        CHECK(std::string(first_copy.get_name()) == "Original (1)");
        CHECK(std::string(second_copy.get_name()) == "Original (2)");
    }

    TEST_CASE("Move construction transfers identity and invalidates the source")
    {
        TestObject original{"Moved"};
        const auto original_id = original.get_id();

        const TestObject moved{std::move(original)};

        CHECK(moved.get_id() == original_id);
        CHECK_FALSE(original.is_valid()); // NOLINT(*-cplusplus.Move)
    }

    TEST_CASE("Move assignment transfers identity and invalidates the source")
    {
        TestObject source{"Source"};
        TestObject destination{"Destination"};

        const auto source_id = source.get_id();

        destination = std::move(source);

        CHECK(destination.get_id() == source_id);
        CHECK_FALSE(source.is_valid()); // NOLINT(*-cplusplus.Move)
    }
}

} // namespace echomap
