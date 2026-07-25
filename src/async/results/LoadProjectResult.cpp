/**
 * @file
 * @brief Project-loading result implementation
 * @author Oliver Dixon
 * @date 2026-07-10
 */

#include "LoadProjectResult.hpp"

#include "../../objects/Project.hpp"

#ifdef __EMSCRIPTEN__
#include "../../objects/web/PartialProject.hpp"
#endif

namespace echomap
{

LoadProjectResult::LoadProjectResult(
        std::unique_ptr<ProjectT> loaded_project
) :
    loaded_project(std::move(loaded_project))
{
}

LoadProjectResult::~LoadProjectResult() noexcept = default;

LoadProjectResult::LoadProjectResult(LoadProjectResult&&) noexcept = default;

LoadProjectResult& LoadProjectResult::operator=(LoadProjectResult&&) noexcept = default;

std::unique_ptr<LoadProjectResult::ProjectT> LoadProjectResult::take_project() && noexcept
{
    return std::move(loaded_project);
}

const LoadProjectResult::ProjectT * LoadProjectResult::observe_project() const noexcept
{
    return loaded_project.get();
}

} // namespace echomap
