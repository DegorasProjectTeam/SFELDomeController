

/** ********************************************************************************************************************
 * @file dome_controller.h
 * @brief This file contains the declaration of the DomeController class.
 * @author Degoras Project Team
 * @copyright EUPL License
***********************************************************************************************************************/

// =====================================================================================================================
#pragma once
// =====================================================================================================================

// C++ INCLUDES
// =====================================================================================================================
// =====================================================================================================================

// PROJECT INCLUDES
// =====================================================================================================================
#include "LibSFELDomeController/sfeldomecontroller_global.h"
#include "LibSFELDomeController/dome_controller_data.h"
// =====================================================================================================================

// SERIAL INCLUDES
// =====================================================================================================================
#include <serial/serial.h>
// =====================================================================================================================

// SFEL DOME NAMESPACES
// =====================================================================================================================
namespace sfeldome{
namespace controller{
// =====================================================================================================================

class LIBDOMECONTROLLER_EXPORT DomeController
{
public:

    DomeController();

    DomeError findHome();
    DomeError incTarget(int deg);
    DomeError setTarget(int deg);
    DomeError getPos(int &deg);
    DomeError getTarget(int &deg);
    DomeError setEnMovement(bool en);
    DomeError stop();

private:

    DomeError sendCommand(const std::string &command, const std::vector<std::string> &params_in,
                          std::vector<std::string> &params_out);

    serial::Serial serial_;

};


using FindHomeCallback = std::function<DomeError()>;
using FindHomeCallbackInArgs = std::tuple<>;
using FindHomeCallbackOutArgs = std::tuple<>;

using IncTargetCallback = std::function<DomeError(int)>;
using IncTargetCallbackInArgs = std::tuple<int>;
using IncTargetCallbackOutArgs = std::tuple<>;

using SetTargetCallback = std::function<DomeError(int)>;
using SetTargetCallbackInArgs = std::tuple<int>;
using SetTargetCallbackOutArgs = std::tuple<>;

using GetTargetCallback = std::function<DomeError(int&)>;
using GetTargetCallbackInArgs = std::tuple<>;
using GetTargetCallbackOutArgs = std::tuple<int>;

using GetPosCallback = std::function<DomeError(int&)>;
using GetPosCallbackInArgs = std::tuple<>;
using GetPosCallbackOutArgs = std::tuple<int>;

using SetEnMovementCallback = std::function<DomeError(bool)>;
using SetEnMovementCallbackInArgs = std::tuple<bool>;
using SetEnMovementCallbackOutArgs = std::tuple<>;

using StopCallback = std::function<DomeError()>;
using StopCallbackInArgs = std::tuple<>;
using StopCallbackOutArgs = std::tuple<>;

}} // END NAMESPACES.
// =====================================================================================================================
