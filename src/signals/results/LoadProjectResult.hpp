/**
 * @file
 * @brief Project-loading result specification
 * @author Oliver Dixon
 * @date 2026-07-10
 */

#ifndef ECHOMAP_LOADPROJECTRESULT_HPP
#define ECHOMAP_LOADPROJECTRESULT_HPP

#include <memory>

namespace echomap
{

#ifdef __EMSCRIPTEN__
class PartialProject;
#else
class Project;
#endif // __EMSCRIPTEN__

/**
 * Denotes a loaded Project completed by a LoadProjectTask job.
 */
class LoadProjectResult
{
public:
#ifdef __EMSCRIPTEN__
    using ProjectT = PartialProject;
#else
    using ProjectT = Project;
#endif // __EMSCRIPTEN__

    explicit LoadProjectResult(std::unique_ptr<ProjectT> loaded_project);
    ~LoadProjectResult() noexcept;

    LoadProjectResult(const LoadProjectResult&) = delete;
    LoadProjectResult& operator=(const LoadProjectResult&) = delete;

    LoadProjectResult(LoadProjectResult&&) noexcept;
    LoadProjectResult& operator=(LoadProjectResult&&) noexcept;

    [[nodiscard]] std::unique_ptr<ProjectT> take_project() && noexcept;
    [[nodiscard]] const ProjectT * observe_project() const noexcept;

private:
    std::unique_ptr<ProjectT> loaded_project;
};

} // namespace echomap

#endif // ECHOMAP_LOADPROJECTRESULT_HPP
