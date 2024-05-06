

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

// ZMQUTILS INCLUDES
// =====================================================================================================================
// =====================================================================================================================

// PROJECT INCLUDES
// =====================================================================================================================
#include "LibSFELDomeController/Global/sfeldomecontroller_global.h"
#include "LibSFELDomeController/DomeController/dome_controller_data.h"
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

    DomeError setHomePosition(const AltAzPos& pos);

    DomeError getHomePosition(AltAzPos& pos);

    DomeError openSerialPort(const std::string& serial_port);


private:

    AltAzPos home_pos_;
};

// Callback function type aliases
using SetHomePositionFunction = std::function<DomeError(const AltAzPos&)>;
using GetHomePositionFunction = std::function<DomeError(AltAzPos&)>;
using OpenSerialPortFunction = std::function<DomeError(const std::string&)>;

// Callback function arguments type aliases
using SetHomePositionFunctionInArgs = std::tuple<AltAzPos>;
using SetHomePositionFunctionOutArgs = std::tuple<>;
//
using GetHomePositionFunctionInArgs = std::tuple<>;
using GetHomePositionFunctionOutArgs = std::tuple<AltAzPos>;
//
using OpenSerialPortFunctionInArgs = std::tuple<std::string>;
using OpenSerialPortFunctionOutArgs = std::tuple<>;

}} // END NAMESPACES.
// =====================================================================================================================
