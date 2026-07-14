/**
 * @file
 *
 * DFTResult specification
 *
 * @author Oliver Dixon
 * @date 2026-07-14
 */

#ifndef ECHOMAP_DFTRESULT_HPP
#define ECHOMAP_DFTRESULT_HPP

#include "../objects/FrequencySpectrum.hpp"
#include "../objects/Signal.hpp"
#include "IResult.hpp"

namespace echomap
{

/**
 *
 */
class DFTResult : public IResult
{
public:
    explicit DFTResult(
            Signal::id_type source_id,
            std::unique_ptr<FrequencySpectrum> spectrum
    );

    void despatch(IResultHandler& handler) override;

    [[nodiscard]] std::unique_ptr<FrequencySpectrum> take_spectrum() noexcept;
    [[nodiscard]] Signal::id_type get_source_id() const noexcept;

private:
    const Signal::id_type source_id;
    std::unique_ptr<FrequencySpectrum> spectrum;
};

} // namespace echomap

#endif // ECHOMAP_DFTRESULT_HPP
