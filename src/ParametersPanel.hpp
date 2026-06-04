//
// Created by owd on 04/06/2026.
//

#ifndef WEBCFD_PARAMETERSPANEL_HPP
#define WEBCFD_PARAMETERSPANEL_HPP

#include "IPanel.hpp"

namespace WebCFD
{

class ParametersPanel final : public IPanel
{
public:
    void draw() override;
};

} // WebCFD

#endif //WEBCFD_PARAMETERSPANEL_HPP
