//
// Created by owd on 6/23/26.
//

#ifdef __EMSCRIPTEN__

#include "JSBridge.hpp"

#include <emscripten/em_macros.h>

#include "Logger.hpp"

namespace WebCFD
{

WebCFD * JSBridge::instance = nullptr;

void JSBridge::bind(
        WebCFD* const new_instance
) noexcept
{
    instance = new_instance;
}

void JSBridge::unbind() noexcept
{
    instance = nullptr;
}

int JSBridge::notify_wav_file_picked(
        const char * const path
) noexcept
{
    if (instance == nullptr) {
        LOG_ERROR("No WebCFD app instance is registered");
        return 1;
    }

    if (path == nullptr) {
        LOG_ERROR("File picker callback received no path.");
        return 2;
    }

    LOG_F_INFO("Received new file {}", path);
    // TODO process

    return 0;
}

} // namespace WebCFD

extern "C" EMSCRIPTEN_KEEPALIVE int webcfd_on_wav_file_picked(const char * const path)
{
    return WebCFD::JSBridge::notify_wav_file_picked(path);
}

#endif // __EMSCRIPTEN__
