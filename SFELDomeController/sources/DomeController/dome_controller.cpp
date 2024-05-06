

// PROJECT INCLUDES
// =====================================================================================================================
#include "LibSFELDomeController/DomeController/dome_controller.h"
// =====================================================================================================================

#include <serial/serial.h>

// SFEL DOME NAMESPACES
// =====================================================================================================================
namespace sfeldome{
namespace controller{
// =====================================================================================================================

DomeController::DomeController() :
    home_pos_({-1,-1})
{}

DomeError DomeController::setHomePosition(const AltAzPos &pos)
{
    // Auxiliar result.
    DomeError error = DomeError::SUCCESS;

    // Check the provided values if necessary.
    // [...]

    // Store the value
    this->home_pos_ = pos;

    // Do things in the hardware (PLC) or FPGA.
    // this->doPLCSetHomePosition(isdhfkljsdhilfhlisd)
    // WARNING: Remember use async if the tasks are computationally demanding.
    // [...]

    // Log.
    std::string cmd_str = ControllerErrorStr[static_cast<size_t>(error)];
    std::cout << std::string(100, '-') << std::endl;
    std::cout<<"<SFEL DOME CONTROLLER>"<<std::endl;
    std::cout<<"-> SET_HOME_POSITION"<<std::endl;
    std::cout<<"Time: "<<zmqutils::utils::currentISO8601Date()<<std::endl;
    std::cout<<"Az: "<<pos.az<<std::endl;
    std::cout<<"El: "<<pos.el<<std::endl;
    std::cout<<"Error: "<<static_cast<int>(error)<<" ("<<cmd_str<<")"<<std::endl;
    std::cout << std::string(100, '-') << std::endl;

    return error;
}

DomeError DomeController::getHomePosition(AltAzPos &pos)
{
    pos = this->home_pos_;

    std::cout << std::string(100, '-') << std::endl;
    std::cout<<"<SFEL DOME CONTROLLER>"<<std::endl;
    std::cout<<"-> GET_HOME_POSITION"<<std::endl;
    std::cout<<"Time: "<<zmqutils::utils::currentISO8601Date()<<std::endl;
    std::cout << std::string(100, '-') << std::endl;

    return DomeError::SUCCESS;
}

DomeError DomeController::openSerialPort(const std::string& serial_port)
{
    // ABRIR SERIAL PORT

    std::cout << std::string(100, '-') << std::endl;
    std::cout<<"<SFEL DOME CONTROLLER>"<<std::endl;
    std::cout<<"-> OPEN_SERIAL_PORT"<<std::endl;
    std::cout<<"Serial Port: "<<serial_port<<std::endl;
    std::cout<<"Time: "<<zmqutils::utils::currentISO8601Date()<<std::endl;
    std::cout << std::string(100, '-') << std::endl;

    return DomeError::CONTROLLER_COMM_ERROR;
}

}} // END NAMESPACES.
// =====================================================================================================================
