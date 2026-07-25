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
        Worker* const worker
) :
    ITask(std::format("LoadProjectTask: {}", path.c_str())),
    project_file_path(std::move(path)),
    worker(worker)
{
}

WorkerResult LoadProjectTask::execute_work()
{
    return LoadProjectResult(deserialiser.deserialise_project(project_file_path, worker));
}

} // namespace echomap
