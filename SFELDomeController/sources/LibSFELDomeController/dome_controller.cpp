

// PROJECT INCLUDES
// =====================================================================================================================
#include "LibSFELDomeController/dome_controller.h"
// =====================================================================================================================

#include <serial/serial.h>

// SFEL DOME NAMESPACES
// =====================================================================================================================
namespace sfeldome{
namespace controller{
// =====================================================================================================================

DomeController::DomeController() {}

DomeError DomeController::findHome()
{

    return DomeError::CONTROLLER_COMM_ERROR;
}

DomeError DomeController::incTarget(int deg)
{
    return DomeError::CONTROLLER_COMM_ERROR;
}

DomeError DomeController::setTarget(int deg)
{
    return DomeError::CONTROLLER_COMM_ERROR;
}

DomeError DomeController::getPos(int &deg)
{
    return DomeError::CONTROLLER_COMM_ERROR;
}

DomeError DomeController::setEnMovement(bool en)
{
    return DomeError::CONTROLLER_COMM_ERROR;
}

}} // END NAMESPACES.
// =====================================================================================================================
