/**
 * @file
 *
 * SignalDFTPanel specification
 *
 * @author Oliver Dixon
 * @date 2026-07-14
 */

#ifndef ECHOMAP_SIGNALDFTPANEL_HPP
#define ECHOMAP_SIGNALDFTPANEL_HPP

#include <sigc++/scoped_connection.h>

#include <string>

#include "../objects/IDAllocator.hpp"
#include "../objects/factories/WindowFunctions.hpp"
#include "IPanel.hpp"

namespace echomap
{
class IProjectObserveService;

class Signal;
class FrequencySpectrum;
class WorkerResultDespatcher;
class DFTResult;
class Worker;
class IRenderInvalidateService;

/**
 * Provides an IPanel to display and interact with previews of Signal frequency spectra (i.e., Signal DFTs).
 */
class SignalDFTPanel final : public IPanel
{
public:
    /**
     * Create a new SignalDFTPanel to display DFTs of Signal waveforms in the frequency domain.
     *
     * The SignalDFTPanel consumes the DFTResult message.
     *
     * @param parent_worker The Worker to receive ITask commands over the command bus.
     * @param despatcher The despatcher to expose the result buses.
     * @param invalidation_service The service to invalidate render cycles.
     * @param observer_service Service for observing the active Project.
     */
    explicit SignalDFTPanel(
            Worker& parent_worker,
            WorkerResultDespatcher& despatcher,
            IRenderInvalidateService& invalidation_service,
            const IProjectObserveService& observer_service
    );

    ~SignalDFTPanel() noexcept override;

    SignalDFTPanel(const SignalDFTPanel&) = delete;
    SignalDFTPanel& operator=(const SignalDFTPanel&) = delete;
    SignalDFTPanel(SignalDFTPanel&&) = delete;
    SignalDFTPanel& operator=(SignalDFTPanel&&) = delete;

    void draw() noexcept override;

    [[nodiscard]] const char* get_imgui_name() const noexcept override;

    static const char* get_imgui_stable_name() noexcept;

private:
    struct CallbackData
    {
        const FrequencySpectrum* spectrum;
        int index_offset;
    };

    static ImPlotPoint get_indexed_frequency_bin(
            int index,
            void* user_data
    ) noexcept;

    void handle_completed_dft(DFTResult&& result);

    void draw_configuration_section() noexcept;
    void draw_configuration_window_function() noexcept;
    void draw_configuration_transform_size() noexcept;
    void draw_configuration_scale_type() noexcept;
    void draw_configuration_preview_actions() noexcept;

    void draw_preview_section(const Project& active_project) noexcept;
    void draw_preview_of_signal(std::shared_ptr<Signal> signal) noexcept;

    void reset_available_transform_sizes();
    void update_spectrum_bounds(const FrequencySpectrum& spectrum) noexcept;
    void update_spectrum_bounds() noexcept;
    void update_available_sizes(std::uint64_t maximum_sample_count);
    void reset_viewport_bounds() noexcept;

    const FrequencySpectrum* get_spectra(
            std::shared_ptr<Signal> signal,
            WindowFunctions::AllFunctions window_function,
            std::size_t transform_size
    );

    /**
     * The minimal bounding box required to fully contain the DFT spectrum plot.
     */
    ImPlotRect spectrum_bounds{
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::lowest(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::lowest(),
    };

    ImPlotRect viewport_bounds; /**< The user-controlled bounding box of the DFT plots. */

    std::string panel_name;
    ImPlotSpec plotting_spec_2d;
    Worker& parent_worker;
    IRenderInvalidateService& invalidation_service;
    const IProjectObserveService& observer_service;

    /**
     * A three-way key into the FrequencySpectrum cache.
     */
    struct CacheKey
    {
        id_type source_id;
        WindowFunctions::AllFunctions window_function;
        std::size_t transform_size;

        bool operator==(const CacheKey& key) const;
    };

    /**
     * A keyed value within the FrequencySpectrum cache, either denoting a pending, failed, or present DFT.
     */
    struct CacheValue
    {
        enum class State : std::uint8_t
        {
            NotRequested,
            Success,
            Pending,
            Failed,
        } status{State::NotRequested};

        std::unique_ptr<FrequencySpectrum> spectrum;
    };

    /**
     * The hash functor for CacheKey.
     */
    struct CacheKeyHash
    {
        [[nodiscard]] std::size_t operator()(const CacheKey& key) const noexcept;

    private:
        static std::size_t combine(
                std::size_t seed,
                std::size_t value
        ) noexcept;
    };

    std::unordered_map<CacheKey, CacheValue, CacheKeyHash> spectra_cache; /**< Cached DFT spectra. */
    bool use_log_scale = false; /**< Should the DFT be plotted with a linear or base-10 logarithmic freq. axis? */
    static constexpr unsigned int default_size_log = 7; /**< Base-2 log of the minimum transform size. */
    std::vector<std::string> available_sizes;           /**< Strings of all available transform sizes. */
    unsigned int selected_size_log = default_size_log;  /**< Base-2 log of selected transform size. */

    bool was_transform_size_combo_open = false;  /**< Was the Transform Size combo box open on the previous cycle? */
    bool was_window_function_combo_open = false; /**< Was the Window Function combo box open on the previous cycle? */
    WindowFunctions::AllFunctions selected_window = WindowFunctions::Constant{};

    std::vector<sigc::scoped_connection> connections;
};

} // namespace echomap

#endif // ECHOMAP_SIGNALDFTPANEL_HPP
