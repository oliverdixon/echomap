/**
 * @file
 *
 * IProjectFilePicker specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_IPROJECTFILEPICKER_HPP
#define ECHOMAP_IPROJECTFILEPICKER_HPP

#include <sigc++/slot.h>

#include <filesystem>

namespace echomap
{

/**
 * @todo Document
 */
class IProjectFilePicker
{
public:
    using SuccessCallbackT = sigc::slot<void(const std::filesystem::path&)>;
    using CancelledCallbackT = sigc::slot<void()>;

    IProjectFilePicker() = default;
    virtual ~IProjectFilePicker() = default;

    IProjectFilePicker(const IProjectFilePicker&) = default;
    IProjectFilePicker& operator=(const IProjectFilePicker&) = default;
    IProjectFilePicker(IProjectFilePicker&&) = default;
    IProjectFilePicker& operator=(IProjectFilePicker&&) = default;

    virtual void request_project_file(
            SuccessCallbackT success,
            CancelledCallbackT cancelled
    ) = 0;
};

} // namespace echomap

#endif // ECHOMAP_IPROJECTFILEPICKER_HPP
