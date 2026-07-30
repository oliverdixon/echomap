/**
 * @file
 *
 * IFilePickerService specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_IFILEPICKERSERVICE_HPP
#define ECHOMAP_IFILEPICKERSERVICE_HPP

#include <sigc++/slot.h>

#include <filesystem>

namespace echomap
{

/**
 * @todo Document
 */
class IFilePickerService
{
public:
    using SuccessCallbackT = sigc::slot<void(const std::filesystem::path&)>;
    using CancelledCallbackT = sigc::slot<void()>;

    IFilePickerService() = default;
    virtual ~IFilePickerService() = default;

    IFilePickerService(const IFilePickerService&) = default;
    IFilePickerService& operator=(const IFilePickerService&) = default;
    IFilePickerService(IFilePickerService&&) = default;
    IFilePickerService& operator=(IFilePickerService&&) = default;

    virtual void request_project_file(
            SuccessCallbackT success,
            CancelledCallbackT cancelled
    ) = 0;
};

} // namespace echomap

#endif // ECHOMAP_IFILEPICKERSERVICE_HPP
