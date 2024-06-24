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
 * @example AmelasControllerServerApp.cpp
 *
 * @brief TODO
 *
 * TODO
 *
 * @author Degoras Project Team
 * @author AVS AMELAS Team
 * @copyright EUPL License
 * @version 2309.5
***********************************************************************************************************************/

// C++ INCLUDES
// =====================================================================================================================
#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif
#include <iostream>
#include <limits>
// =====================================================================================================================

// ZMQUTILS INCLUDES
// =====================================================================================================================
#include "LibZMQUtils/Modules/Utilities"
// =====================================================================================================================

// AMELAS INTERFACE INCLUDES
// =====================================================================================================================
#include <LibSFELDomeController/dome_controller_server.h>
#include <LibSFELDomeController/dome_controller.h>
#include <LibSFELDomeController/dome_controller_server_data.h>
// =====================================================================================================================





/**
 * @brief Main entry point of the program AmelasControllerServerApp.
 *
 * TODO
 *
 * WARNING This now is a simple example. This must be replaced by the real server program.
 *
 */
int main(int argc, char**argv)
{

    // MAIN SERVER APP
    // GLOBAL CONFIG
    // AMELAS SERVER
    // ETC
    // WARNING: WINDOWS PROCESS MODE

    std::string serial_port = "/dev/ttyACM0";

    if (argc > 1)
        serial_port = argv[1];


    // Nampesaces.
    using sfeldome::communication::DomeControllerServer;
    using sfeldome::communication::DomeServerCommand;
    using sfeldome::controller::DomeController;

    // Configure the console.
    zmqutils::utils::ConsoleConfig& console_cfg = zmqutils::utils::ConsoleConfig::getInstance();
    console_cfg.configureConsole(true, true, true);

    // Configuration variables.
    unsigned port = 9000;
    bool client_status_check = true;

    // Instantiate the Amelas controller.
    DomeController dome_controller(serial_port);

    // Instantiate the server.
    DomeControllerServer dome_server(port, "*", "DomeControllerServer", "1.1.1", "SFEL Dome Controller Server", true);

    // Disable or enables the client status checking.
    dome_server.setClientStatusCheck(client_status_check);

    // TODO: Remove this
    dome_server.setClientAliveTimeout(std::chrono::minutes(10));

    // ---------------------------------------

    // Set the controller callbacks in the server.
    dome_server.registerCbAndReqProcFunc<sfeldome::controller::FindHomeCallback,
                                         sfeldome::controller::FindHomeCallbackInArgs,
                                         sfeldome::controller::FindHomeCallbackOutArgs>(
        DomeServerCommand::REQ_FIND_HOME,
        &dome_controller,
        &DomeController::findHome);

    dome_server.registerCbAndReqProcFunc<sfeldome::controller::IncTargetCallback,
                                         sfeldome::controller::IncTargetCallbackInArgs,
                                         sfeldome::controller::IncTargetCallbackOutArgs>(
        DomeServerCommand::REQ_INC_TARGET,
        &dome_controller,
        &DomeController::incTarget);

    dome_server.registerCbAndReqProcFunc<sfeldome::controller::SetTargetCallback,
                                         sfeldome::controller::SetTargetCallbackInArgs,
                                         sfeldome::controller::SetTargetCallbackOutArgs>(
        DomeServerCommand::REQ_SET_TARGET,
        &dome_controller,
        &DomeController::setTarget);

    dome_server.registerCbAndReqProcFunc<sfeldome::controller::GetTargetCallback,
                                         sfeldome::controller::GetTargetCallbackInArgs,
                                         sfeldome::controller::GetTargetCallbackOutArgs>(
        DomeServerCommand::REQ_GET_TARGET,
        &dome_controller,
        &DomeController::getTarget);

    dome_server.registerCbAndReqProcFunc<sfeldome::controller::GetPosCallback,
                                         sfeldome::controller::GetPosCallbackInArgs,
                                         sfeldome::controller::GetPosCallbackOutArgs>(
        DomeServerCommand::REQ_GET_POS,
        &dome_controller,
        &DomeController::getPos);

    dome_server.registerCbAndReqProcFunc<sfeldome::controller::SetEnMovementCallback,
                                         sfeldome::controller::SetEnMovementCallbackInArgs,
                                         sfeldome::controller::SetEnMovementCallbackOutArgs>(
        DomeServerCommand::REQ_EN_MOVEMENT,
        &dome_controller,
        &DomeController::setEnMovement);

    dome_server.registerCbAndReqProcFunc<sfeldome::controller::StopCallback,
                                         sfeldome::controller::StopCallbackInArgs,
                                         sfeldome::controller::StopCallbackOutArgs>(
        DomeServerCommand::REQ_STOP,
        &dome_controller,
        &DomeController::stop);


    // -- OTHER FUNCTIONS
    // ---------------------------------------

    // Start the server.
    bool started = dome_server.startServer();

    // Check if the server starts ok.
    if(!started)
    {
        // Log.
        std::cout << "Server start failed!! Press Enter to exit!" << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.clear();
        return 1;
    }

    // Wait for closing as an infinite loop until ctrl-c.
    console_cfg.waitForClose();

    // Log.
    std::cout << "Stopping the server..." << std::endl;

    // Stop the server.
    dome_server.stopServer();

    // Final log.
    std::cout << "Server stoped. All ok!!" << std::endl;

    // Restore the console.
    console_cfg.restoreConsole();

    // Return.
	return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
