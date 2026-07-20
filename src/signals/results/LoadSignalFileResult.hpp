/**
 * @file
 *
 * LoadSignalFileResult specification
 *
 * @author Oliver Dixon
 * @date 2026-07-17
 */

#ifndef ECHOMAP_LOADSIGNALFILERESULT_HPP
#define ECHOMAP_LOADSIGNALFILERESULT_HPP

#include <memory>
#include <ranges>
#include <vector>

#include "../../objects/IDAllocator.hpp"

namespace echomap
{

class Signal;

/**
 * @todo Document.
 */
class LoadSignalFileResult
{
public:
    explicit LoadSignalFileResult(
            id_type project_id,
            std::vector<std::unique_ptr<Signal>> loaded_signal
    );

    [[nodiscard]] auto take_signals() && noexcept
    {
        return std::move(loaded_signals) | std::views::as_rvalue;
    }

    [[nodiscard]] auto observe_signals() const noexcept
    {
        return loaded_signals | std::views::transform([](const std::unique_ptr<Signal>& signal) {
                   return signal.get();
               });
    }

    [[nodiscard]] id_type get_project_id() const noexcept;

private:
    id_type project_id; // TODO put these in all tasks.
    std::vector<std::unique_ptr<Signal>> loaded_signals;
};

} // namespace echomap

#endif // ECHOMAP_LOADSIGNALFILERESULT_HPP
