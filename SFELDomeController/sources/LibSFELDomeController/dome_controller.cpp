

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
        this->serial_.flush();
        std::string full_command = command;
        for (const auto& param : params_in)
        {
            full_command += " " + param;
        }
        full_command += "\n";
        this->serial_.write(full_command);
        bool available = this->serial_.waitReadable();
        if (available)
        {
            std::string answer = this->serial_.readline();
            std::cout << "Received answer from serial: " << answer << std::endl;
            char *tok = std::strtok(answer.data(), " ");
            std::vector<std::string> response_tokens;
            while(tok)
            {
                response_tokens.push_back(tok);
                tok = std::strtok(nullptr, " ");
            }

            if (response_tokens.size() > 1)
            {
                if (response_tokens[0] == "R" && response_tokens[1] == command)
                {
                    result = DomeError::SUCCESS;
                    params_out = {std::make_move_iterator(response_tokens.begin() + 2),
                                  std::make_move_iterator(response_tokens.end())};
                }
                else if (response_tokens[0] == "E")
                {
                    if (response_tokens[1] == "0")
                        result = DomeError::ERROR_INVALID_COMMAND;
                    else if (response_tokens[1] == "1")
                        result = DomeError::ERROR_DISABLED_MOVE;
                    else if (response_tokens[1] == "2")
                        result = DomeError::ERROR_NOT_REAL_POS;
                }
                else if (response_tokens[0] == "W" && response_tokens[1] == "1")
                {
                    result = DomeError::WARNING_NOT_REAL_POS;
                }
            }
        }

    }
    catch (const std::exception &e)
    {
        std::cerr << "Error at command execution. Reason: " << e.what() << std::endl;
        this->serial_.close();
    }

    this->serial_.close();

    return result;
}

}} // END NAMESPACES.
// =====================================================================================================================
