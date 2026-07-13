/**
 * @file
 *
 * DownsampleResult specification
 *
 * @author Oliver Dixon
 * @date 2026-07-13
 */

#ifndef ECHOMAP_DOWNSAMPLERESULT_HPP
#define ECHOMAP_DOWNSAMPLERESULT_HPP

#include <memory>

#include "../objects/Signal.hpp"
#include "IResult.hpp"

namespace echomap
{

/**
 * Represents an IResult for a downsampled Signal produced by a DownsampleTask job.
 */
class DownsampleResult : public IResult
{
public:
    explicit DownsampleResult(
            Signal::id_type source_id,
            std::unique_ptr<Signal> downsampled
    );

    void despatch(IResultHandler& handler) override;

    [[nodiscard]] std::unique_ptr<Signal> take_downsampled() noexcept;
    [[nodiscard]] Signal::id_type get_source_id() const noexcept;

private:
    const Signal::id_type source_id;
    std::unique_ptr<Signal> downsampled;
};

} // namespace echomap

#endif // ECHOMAP_DOWNSAMPLERESULT_HPP
