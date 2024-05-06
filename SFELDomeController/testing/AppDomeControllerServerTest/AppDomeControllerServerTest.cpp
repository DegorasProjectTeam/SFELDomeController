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

// EXTERNAL INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/Modules/Utilities>
#include <LibSFELDomeController/Modules/DomeControllerServer>
// =====================================================================================================================


#include <serial/serial.h>

/**
 * @brief Main entry point of the program AmelasControllerServerApp.
 *
 * TODO
 *
 * WARNING This now is a simple example. This must be replaced by the real server program.
 *
 */
int main(int, char**)
{

    std::vector<serial::PortInfo> devices_found = serial::list_ports();

    std::vector<serial::PortInfo>::iterator iter = devices_found.begin();

    while( iter != devices_found.end() )
    {
        serial::PortInfo device = *iter++;

        printf( "(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
               device.hardware_id.c_str() );
    }



    // Nampesaces.
    using sfeldome::communication::DomeControllerServer;
    using sfeldome::communication::DomeServerCommand;
    using sfeldome::controller::DomeController;
    // Callbacks
    using sfeldome::controller::SetHomePositionFunction;
    using sfeldome::controller::SetHomePositionFunctionInArgs;
    using sfeldome::controller::SetHomePositionFunctionOutArgs;
    using sfeldome::controller::GetHomePositionFunction;
    using sfeldome::controller::GetHomePositionFunctionInArgs;
    using sfeldome::controller::GetHomePositionFunctionOutArgs;
    using sfeldome::controller::OpenSerialPortFunction;
    using sfeldome::controller::OpenSerialPortFunctionInArgs;
    using sfeldome::controller::OpenSerialPortFunctionOutArgs;
    // -------------------------------------------------------------------

    // Configure the console.
    zmqutils::utils::ConsoleConfig& console_cfg = zmqutils::utils::ConsoleConfig::getInstance();
    console_cfg.configureConsole(true, true, false);

    // Configuration variables.
    unsigned port = 9999;
    bool client_status_check = true;

    // Instantiate the Dome controller.
    DomeController dome_controller;

    // Instantiate the server.
    DomeControllerServer dome_server(port);

    // Disable or enables the client status checking.
    dome_server.setClientStatusCheck(client_status_check);

    // -------------------------------------------------------------------

    // Set the controller callbacks in the server.

    dome_server.registerCallbackAndRequestProcFunc<SetHomePositionFunction,
                                                   SetHomePositionFunctionInArgs,
                                                   SetHomePositionFunctionOutArgs>
        (DomeServerCommand::REQ_SET_HOME_POSITION,
         &dome_controller,
         &DomeController::setHomePosition);

    dome_server.registerCallbackAndRequestProcFunc<GetHomePositionFunction,
                                                   GetHomePositionFunctionInArgs,
                                                   GetHomePositionFunctionOutArgs>
        (DomeServerCommand::REQ_GET_HOME_POSITION,
         &dome_controller,
         &DomeController::getHomePosition);

    dome_server.registerCallbackAndRequestProcFunc<OpenSerialPortFunction,
                                                   OpenSerialPortFunctionInArgs,
                                                   OpenSerialPortFunctionOutArgs>
        (DomeServerCommand::REQ_OPEN_SERIAL_PORT,
         &dome_controller,
         &DomeController::openSerialPort);

    // -------------------------------------------------------------------

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
