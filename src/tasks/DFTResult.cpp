/**
 * @file
 *
 * DFTResult implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-14
 */

#include "DFTResult.hpp"

#include "IResultHandler.hpp"

namespace echomap
{

DFTResult::DFTResult(
        const Signal::id_type source_id,
        std::unique_ptr<FrequencySpectrum> spectrum
) :
    IResult(std::format(
            "DFTResult: {}",
            spectrum->get_name()
    )),
    source_id(source_id),
    spectrum(std::move(spectrum))
{
    assert(this->spectrum != nullptr);
}

void DFTResult::despatch(
        IResultHandler& handler
)
{
    handler.handle(*this);
}

std::unique_ptr<FrequencySpectrum> DFTResult::take_spectrum() noexcept
{
    return std::move(spectrum);
}

Signal::id_type DFTResult::get_source_id() const noexcept
{
    return source_id;
}

} // namespace echomap
