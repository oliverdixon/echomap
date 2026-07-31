/**
 * @file
 *
 * WebProjectFilePickerService specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_WEBPROJECTFILEPICKERSERVICE_HPP
#define ECHOMAP_WEBPROJECTFILEPICKERSERVICE_HPP

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "../IFilePickerService.hpp"

/**
 * Services the Project selected callback for Emscripten.
 *
 * @param path The file-system path (in the Wasm VFS) selected in the prompt.
 * @return Status code; see @ref echomap::WebProjectFilePickerService::Status, or -1 if an implementation-defined
 *  exception was caught.
 */
extern "C" int echomap_on_project_file_picked(const char* path) noexcept;

namespace echomap
{

/**
 * Provides a service implementation for uploading Project files on a WebAssembly target.
 *
 * This class uses static storage to persist object handles across FFI boundaries. Hence, the
 * WebProjectFilePickerService is not thread-safe, and cannot be copied or moved.
 */
class WebProjectFilePickerService : public IFilePickerService
{
public:
    WebProjectFilePickerService();
    ~WebProjectFilePickerService() noexcept override;

    WebProjectFilePickerService(const WebProjectFilePickerService&) = delete;
    WebProjectFilePickerService& operator=(const WebProjectFilePickerService&) = delete;
    WebProjectFilePickerService(WebProjectFilePickerService&&) = delete;
    WebProjectFilePickerService& operator=(WebProjectFilePickerService&&) = delete;

    void request_project_file(
            SuccessCallbackT success,
            CancelledCallbackT cancelled
    ) override;

private:
    friend int ::echomap_on_project_file_picked(const char* path) noexcept;

    /**
     * Status codes to indicate result to JS runtime.
     */
    enum class Status : std::uint8_t
    {
        Success = 0,       /**< No error reported. */
        NoBoundInstance,   /**< No VFSPickerService instance was bound into static storage. */
        NoPath,            /**< The path was not provided by the JS callback. */
        NoPendingRequest,  /**< There is no PendingRequest in @ref pending_request. */
        UnexpectedProject, /**< The JS callback indicated a PartialProject with a different ID to ours. */
        UnexpectedPath,    /**< The JS callback indicated a path different to the one we expected. */
    };

    /**
     * Completes the Project load workflow from a static context.
     *
     * If no path was provided, @ref cancelled_callback is invoked.
     *
     * @param path The file-system path (in the Wasm VFS) selected in the prompt.
     * @return Integral Status code.
     */
    static Status complete_project_file_pick(const char * path);

    SuccessCallbackT success_callback;
    CancelledCallbackT cancelled_callback;

    static WebProjectFilePickerService* instance;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_WEBPROJECTFILEPICKERSERVICE_HPP
