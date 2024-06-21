

// PROJECT INCLUDES
// =====================================================================================================================
#include "LibSFELDomeController/dome_controller.h"
// =====================================================================================================================

// C++ INCLUDES
// =====================================================================================================================
#include <cstring>
// =====================================================================================================================

// SFEL DOME NAMESPACES
// =====================================================================================================================
namespace sfeldome{
namespace controller{
// =====================================================================================================================


DomeController::DomeController(const std::string &serial_port) :
    serial_(serial_port, 115200)
{
    try
    {
        serial::Timeout timeout = serial::Timeout::simpleTimeout(2000);
        this->serial_.setTimeout(timeout);
        //this->serial_.open();
        this->serial_.close();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception at serial port opening. Reason: " << e.what() << std::endl;
        throw std::runtime_error("[LibSFELDomeController] - Cannot open serial port at DomeController.");
    }
}

DomeError DomeController::findHome()
{
    std::vector<std::string> params_out;
    return this->sendCommand("BC", {}, params_out);
}

DomeError DomeController::incTarget(int deg)
{
    std::vector<std::string> params_out;
    return this->sendCommand("IC", {std::to_string(deg)}, params_out);
}

DomeError DomeController::setTarget(int deg)
{
    std::vector<std::string> params_out;
    return this->sendCommand("EC", {std::to_string(deg)}, params_out);
}

DomeError DomeController::getPos(int &deg)
{
    std::vector<std::string> params_out;
    auto error = this->sendCommand("PP", {}, params_out);
    if (DomeError::SUCCESS == error)
    {
        try
        {
            int answer = std::stoi(params_out.at(0));
            deg = answer;
        }
        catch(...)
        {
            error = DomeError::CONTROLLER_COMM_ERROR;
        }
    }
    return error;
}

DomeError DomeController::getTarget(int &deg)
{
    std::vector<std::string> params_out;
    auto error = this->sendCommand("PC", {}, params_out);
    if (DomeError::SUCCESS == error)
    {
        try
        {
            int answer = std::stoi(params_out.at(0));
            deg = answer;
        }
        catch(...)
        {
            error = DomeError::CONTROLLER_COMM_ERROR;
        }
    }
    return error;
}

DomeError DomeController::setEnMovement(bool en)
{
    std::string command = en ? "HM" : "DM";
    std::vector<std::string> params_out;
    return this->sendCommand(command, {}, params_out);
}

DomeError DomeController::stop()
{
    std::vector<std::string> params_out;
    return this->sendCommand("ST", {}, params_out);
}

DomeError DomeController::sendCommand(const std::string &command, const std::vector<std::string> &params_in,
                                      std::vector<std::string> &params_out)
{
    DomeError result = DomeError::CONTROLLER_COMM_ERROR;

    try
    {
        this->serial_.open();
        std::string full_command = command;
        for (const auto& param : params_in)
        {
            full_command += " " + param;
        }
        this->serial_.write(full_command);
        this->serial_.close();
        bool available = this->serial_.waitReadable();
        if (available)
        {
            std::string answer = this->serial_.readline();
            char *tok = std::strtok(answer.data(), " ");
            while(tok)
            {
                params_out.push_back(tok);
                tok = std::strtok(nullptr, " ");
            }

            result = DomeError::SUCCESS;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error at command execution. Reason: " << e.what() << std::endl;
    }

    return result;
}

}} // END NAMESPACES.
// =====================================================================================================================
