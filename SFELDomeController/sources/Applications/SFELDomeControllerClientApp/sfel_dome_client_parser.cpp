/***********************************************************************************************************************
 *   AMELAS_SFELMountController: [...].
 *
 *   Copyright (C) 2023 ROA Team (Royal Institute and Observatory of the Spanish Navy)
 *                      < Ángel Vera Herrera, avera@roa.es - angeldelaveracruz@gmail.com >
 *                      < Jesús Relinque Madroñal >
 *                      AVS AMELAS Team
 *                      <>
 *
 *   This file is part of AMELAS_SFELMountController.
 *
 *   Licensed under [...]
 **********************************************************************************************************************/



// PROJECT INCLUDES
// =====================================================================================================================
#include "sfel_dome_client_parser.h"
// =====================================================================================================================

// ZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/InternalHelpers/string_helpers.h>
// =====================================================================================================================

// AMELAS INTERFACE INCLUDES
// =====================================================================================================================
#include <LibSFELDomeClient/dome_controller_client.h>
// =====================================================================================================================

// ---------------------------------------------------------------------------------------------------------------------
using zmqutils::reqrep::CommandType;
using zmqutils::reqrep::CommandReply;
using zmqutils::reqrep::ServerCommand;
using zmqutils::reqrep::CommandClientBase;
using zmqutils::reqrep::RequestData;
using zmqutils::reqrep::OperationResult;
using zmqutils::serializer::BinarySerializer;
using sfeldome::communication::DomeControllerClient;
using sfeldome::communication::DomeServerCommand;
using sfeldome::controller::AltAzPos;
using sfeldome::controller::AltAzVel;
using sfeldome::controller::AltAzAdj;
using sfeldome::controller::DomeError;
// ---------------------------------------------------------------------------------------------------------------------

namespace sfeldome{
namespace communication {

SFELDomeClientParser::SFELDomeClientParser(DomeControllerClient &client) : client_(client)
{}

OperationResult SFELDomeClientParser::parseCommand(const std::string &command)
{
    auto tokens = zmqutils::internal_helpers::strings::split<std::vector<std::string>>(command, " ", false);

    if (tokens.empty())
    {
        std::cerr << "Not a valid command." << std::endl;
        return OperationResult::UNKNOWN_COMMAND;
    }

    CommandType command_id;

    try
    {
        command_id = static_cast<CommandType>(std::stoi(tokens.front()));
    }
    catch (...)
    {
        std::cerr << "Not a valid command." << std::endl;
        return OperationResult::UNKNOWN_COMMAND;
    }

    if (!this->client_.validateCommand(command_id))
    {
        std::cerr << "Not implemented command." << std::endl;
        return OperationResult::NOT_IMPLEMENTED;
    }

    std::vector<std::string> params;
    std::move(tokens.begin() + 1, tokens.end(), std::back_inserter(params));

    return this->executeCommand(command_id, params);
}

OperationResult SFELDomeClientParser::executeCommand(
    zmqutils::reqrep::CommandType command_id, const std::vector<std::string> &params)
{
    OperationResult res = OperationResult::INVALID_RESULT;
    DomeError error = DomeError::INVALID_ERROR;
    std::string log;

    std::cout << "Sending " << this->client_.serverCommandToString(command_id) << " command." << std::endl;

    if (command_id == static_cast<CommandType>(ServerCommand::REQ_CONNECT))
    {
        res = this->client_.doConnect();
    }
    else if (command_id == static_cast<CommandType>(ServerCommand::REQ_DISCONNECT))
    {
        res = this->client_.doDisconnect();
    }
    else if (command_id == static_cast<CommandType>(ServerCommand::REQ_ALIVE))
    {
        res = this->client_.doAlive();
    }
    else if (command_id == static_cast<CommandType>(ServerCommand::REQ_GET_SERVER_TIME))
    {
        std::string datetime;
        res = this->client_.doGetServerTime(datetime);
        std::cout << "Server time is: " << datetime << std::endl;
    }
    else if (command_id == static_cast<CommandType>(ServerCommand::REQ_PING))
    {
        std::cout << "Sending REQ_PING command." << std::endl;
        std::chrono::milliseconds elapsed_time;
        res = this->client_.doPing(elapsed_time);

        if(res == OperationResult::COMMAND_OK)
            std::cout << "PING command executed succesfully. ";

        else
            std::cout << "PING command failed. ";

        std::cout << "Elapsed time is: " << std::to_string(elapsed_time.count()) << " ms." << std::endl;
    }
    else if (command_id == static_cast<CommandType>(DomeServerCommand::REQ_OPEN_SERIAL_PORT))
    {
        if (params.size() == 1)
        {
            res = this->client_.openSerialPort(params[0], error);
        }
        else
            res = OperationResult::BAD_PARAMETERS;

    }
    else
    {
        std::cout << "Not implemented command" << std::endl;
        res = OperationResult::NOT_IMPLEMENTED;
    }

    if(res==OperationResult::COMMAND_OK)
        std::cout << this->client_.serverCommandToString(command_id)
                  << " command executed succesfully. " << std::endl;
    else
        std::cout << this->client_.serverCommandToString(command_id) << " command failed."
                  << "Result code is: " << static_cast<int>(res)
                  << "(" << this->client_.operationResultToString(res) << ")" <<  std::endl;

    return res;

}

bool SFELDomeClientParser::parseBool(const std::vector<std::string> &params, bool &flag)
{
    if (params.size() != 1)
        return false;

    std::string token = params.front();

    std::transform(token.begin(), token.end(), token.begin(), ::tolower);

    if (token == "0" || token == "false")
    {
        flag = false;
        return true;
    }
    else if (token == "1" || token == "true")
    {
        flag = true;
        return true;
    }

    return false;
}

bool SFELDomeClientParser::parseBool(const std::string& param, bool &flag)
{
    std::string aux = param;
    std::transform(aux.begin(), aux.end(), aux.begin(), ::tolower);

    if (aux == "0" || aux == "false")
    {
        flag = false;
        return true;
    }
    else if (aux == "1" || aux == "true")
    {
        flag = true;
        return true;
    }

    return false;
}

bool SFELDomeClientParser::parseAltAz(const std::vector<std::string> &params, AltAzPos &pos)
{
    if (params.size() != 2)
        return false;

    bool valid_params = true;

    try
    {
        pos.az = std::stod(params[0]);
        pos.el = std::stod(params[1]);
    }
    catch (...)
    {
        valid_params = false;
    }

    return valid_params;

}

bool SFELDomeClientParser::parseAltAzPosVel(const std::vector<std::string> &params,
                                                                 AltAzPos &pos, AltAzVel &vel)
{
    if (params.size() != 2 && params.size() != 4)
        return false;

    bool valid_params = true;

    try
    {
        pos.az = std::stod(params[0]);
        pos.el = std::stod(params[1]);

        if (params.size() == 4)
        {
            vel.az = std::stod(params[2]);
            vel.el = std::stod(params[3]);
        }
        else
            vel = {0., 0.};

    }
    catch (...)
    {
        valid_params = false;
    }

    return valid_params;

}

}
}

