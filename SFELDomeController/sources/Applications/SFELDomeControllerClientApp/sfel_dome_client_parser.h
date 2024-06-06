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

/** ********************************************************************************************************************
 * @file sfel_dome_client_parser.h
 * @brief
 * @author Degoras Project Team
 * @copyright EUPL License
***********************************************************************************************************************/

// =====================================================================================================================
#pragma once
// =====================================================================================================================

// ZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/Modules/Utilities>
// =====================================================================================================================

// AMELAS INTERFACE INCLUDES
// =====================================================================================================================
#include <LibSFELDomeClient/dome_controller_client.h>
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace sfeldome {
namespace communication {
// =====================================================================================================================

class DomeControllerClient;

class SFELDomeClientParser
{
public:

    SFELDomeClientParser(DomeControllerClient &client);

    zmqutils::reqrep::OperationResult parseCommand(const std::string &command);

    zmqutils::reqrep::OperationResult executeCommand(zmqutils::reqrep::CommandType command_id,
                                                     const std::vector<std::string> &params);
private:

    bool parseBool(const std::vector<std::string> &params, bool& flag);
    bool parseBool(const std::string& params, bool& flag);
    bool parseAltAz(const std::vector<std::string> &params, sfeldome::controller::AltAzPos &pos);
    bool parseAltAzPosVel(const std::vector<std::string> &params, sfeldome::controller::AltAzPos &pos,
                          sfeldome::controller::AltAzVel &vel);


    DomeControllerClient &client_;
};


}} // END NAMESPACES.
// =====================================================================================================================
