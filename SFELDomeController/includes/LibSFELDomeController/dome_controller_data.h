

/** ********************************************************************************************************************
 * @file dome_controller_data.h
 * @brief This file contains the declaration of common elements for the DomeController module.
 * @author Degoras Project Team
 * @author AVS AMELAS Team
 * @copyright EUPL License
***********************************************************************************************************************/

// =====================================================================================================================
#pragma once
// =====================================================================================================================

// C++ INCLUDES
// =====================================================================================================================
#include <cstdint>
// =====================================================================================================================

// LIBZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/Modules/Utilities>
// =====================================================================================================================

// PROJECT INCLUDES
// =====================================================================================================================
#include "LibSFELDomeController/sfeldomecontroller_global.h"
// =====================================================================================================================

// SFEL DOME NAMESPACES
// =====================================================================================================================
namespace sfeldome{
namespace controller{
// =====================================================================================================================

enum class DomeError : std::int32_t
{
    INVALID_ERROR = -1,
    SUCCESS = 0,
    CONTROLLER_COMM_ERROR = 1,
    WARNING_NOT_REAL_POS = 2,
    ERROR_NOT_REAL_POS = 3,
    ERROR_INVALID_COMMAND = 4,
    ERROR_DISABLED_MOVE = 5
};

static constexpr std::array<const char*, 2>  ControllerErrorStr
{
    "SUCCESS - Controller process success",
    "CONTROLLER_COMM_ERROR - The controller can not communicate with microcontroller."
};

struct LIBDOMECONTROLLER_EXPORT AltAzPos final : public zmqutils::serializer::Serializable
{
     AltAzPos(double az, double el);

     AltAzPos();

     AltAzPos(const AltAzPos& pos) = default;

     AltAzPos& operator =(const AltAzPos& pos) = default;

     size_t serialize(zmqutils::serializer::BinarySerializer& serializer) const final;

     void deserialize(zmqutils::serializer::BinarySerializer& serializer) final;

     size_t serializedSize() const final;

    ~AltAzPos() final;

    double az;
    double el;
};

using AltAzVel = AltAzPos;
using AltAzAdj = AltAzPos;

// =====================================================================================================================

}} // END NAMESPACES.
// =====================================================================================================================
