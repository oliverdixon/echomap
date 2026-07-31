/**
 * @file
 * @brief Project-loading task implementation
 * @author Oliver Dixon
 * @date 2026-07-10
 */

#include "LoadProjectTask.hpp"

#include "../results/LoadProjectResult.hpp"

#ifdef __EMSCRIPTEN__
#include "../../objects/web/PartialProject.hpp"
#else
#include "../../objects/Project.hpp"
#endif // __EMSCRIPTEN__

namespace echomap
{

JSONDeserialiser LoadProjectTask::deserialiser{};

LoadProjectTask::LoadProjectTask(
        std::filesystem::path path,
        Worker& worker
) :
    ITask(std::format("LoadProjectTask: {}", path.c_str())),
    project_file_path(std::move(path)),
    worker(worker)
{
}

LoadProjectTask::LoadProjectTask(
        LoadProjectTask&& other
) noexcept :
    ITask(std::move(other)),
    project_file_path(std::move(other.project_file_path)),
    worker(other.worker)
{
}

LoadProjectTask& LoadProjectTask::operator=(
        LoadProjectTask&& other
) noexcept
{
    if (this == &other)
        return *this;

    move_task_identity_from(std::move(other));

    project_file_path = std::move(other.project_file_path);
    worker = other.worker;

    return *this;
}

WorkerResult LoadProjectTask::execute_work()
{
    return LoadProjectResult(deserialiser.deserialise_project(project_file_path, worker));
}

} // namespace echomap
