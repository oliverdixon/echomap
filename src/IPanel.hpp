//
// Created by owd on 04/06/2026.
//

#ifndef WEBCFD_PANEL_HPP
#define WEBCFD_PANEL_HPP

#include <webgpu/webgpu_cpp.h>

namespace WebCFD
{

class IPanel
{
public:
    virtual ~IPanel() = default;

    virtual void draw() = 0;

    virtual void update_gpu(const wgpu::CommandEncoder &command_encoder)
    {
        std::ignore = command_encoder;
    }
};

} // WebCFD

#endif //WEBCFD_PANEL_HPP
