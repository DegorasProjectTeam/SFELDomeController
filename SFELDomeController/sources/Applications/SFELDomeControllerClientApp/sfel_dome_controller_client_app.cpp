/***********************************************************************************************************************
 *   SFELDomeController: [...].
 *
 *   Copyright (C) 2023 ROA Team (Royal Institute and Observatory of the Spanish Navy)
 *                      < Ángel Vera Herrera, avera@roa.es - angeldelaveracruz@gmail.com >
 *                      < Jesús Relinque Madroñal >
 *                      AVS AMELAS Team
 *                      <>
 *
 *   This file is part of SFELDomeController.
 *
 *   Licensed under [...]
 **********************************************************************************************************************/

/** ********************************************************************************************************************
 * @example SFELDomeControllerClientApp.cpp
 *
 * @brief TODO
 *
 * TODO
 *
 * @author Degoras Project Team
 * @copyright EUPL License
 * @version 2309.5
***********************************************************************************************************************/

// C++ INCLUDES
// =====================================================================================================================
#include <iostream>
// =====================================================================================================================

// ZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/Modules/Utilities>
// =====================================================================================================================

// CLIENT PARSER
// =====================================================================================================================
#include "sfel_dome_client_parser.h"
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
using sfeldome::communication::SFELDomeClientParser;
using sfeldome::controller::AltAzPos;
using sfeldome::controller::AltAzVel;
using sfeldome::controller::AltAzAdj;
using sfeldome::controller::DomeError;
// ---------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------

void printSFELDomeCommands()
{
    // TODO Some commands not have correct parameters.
    std::cout<<"------------------------------------------------------"                  <<std::endl
             <<"-- Basic Commands --"                                                    <<std::endl
             <<">  0 - REQ_CONNECT"                                                      <<std::endl
             <<">  1 - REQ_DISCONNECT"                                                   <<std::endl
             <<">  2 - REQ_ALIVE"                                                        <<std::endl
             <<">  3 - REQ_GET_SERVER_TIME"                                              <<std::endl
             <<">  4 - REQ_PING"                                                         <<std::endl
             <<"------------------------------------------------------"                  <<std::endl
             <<"-- Specific Commands --"                                                 <<std::endl
             <<"> "<< static_cast<int>(DomeServerCommand::REQ_OPEN_SERIAL_PORT)
             << " - REQ_OPEN_SERIAL_PORT"                                                <<std::endl
             <<"-- Other --"                                                             <<std::endl
             <<"> Show the commands:       help"                                         <<std::endl
             <<"> Client exit:             exit"                                         <<std::endl
             <<"> Enable auto-alive:       auto_alive_en"                                <<std::endl
             <<"> Disable auto-alive:      auto_alive_ds"                                <<std::endl
             <<"> Enable auto-alive clbk:  auto_alive_clbk_en"                           <<std::endl
             <<"> Disable auto-alive clbk: auto_alive_clbk_ds"                           <<std::endl
             <<"------------------------------------------------------"                  <<std::endl;
}

// ---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Main entry point of the program `SFELDomeControllerClientApp`.
 */
int main(int argc, char** argv)
{
    // Std Namespaces.
    using namespace std::chrono_literals;

    // Configure the console.
    zmqutils::utils::ConsoleConfig& console_cfg = zmqutils::utils::ConsoleConfig::getInstance();
    console_cfg.configureConsole(true, false, true);

    // Client configuration variables.
    std::string server_endpoint = "tcp://192.168.209.51:9999";    // Server endpoint.
    std::string client_iface = "";                                // Client network interface.
    std::string client_name = "SFELDomeCommandLineClient";          // Client name.
    std::string client_version = "1.1.2";                         // Client version.
    std::string client_info = "SFELDome Controller Client v1.1.2";  // Client information.

    // Other configurations.
    bool enable_alive_callbacks = false;                    // Disable or enable the alive callbacks.
    std::chrono::milliseconds alive_timeout_ms = 2000ms;    // Timeout to consider a client dead.
    std::chrono::milliseconds alive_period_ms = 1000ms;     // Timeout to consider a client dead.

    // Get the server endpoint as argument parameter.
    if (argc > 1)
        server_endpoint = argv[1];

    // Instantiate the client.
    DomeControllerClient dome_client(server_endpoint, client_iface, client_name, client_version, client_info);

    // Prepare the auxiliar parser.
    SFELDomeClientParser client_parser(dome_client);

    // Configure the client.
    dome_client.setAliveCallbacksEnabled(enable_alive_callbacks);
    dome_client.setServerAliveTimeout(alive_timeout_ms);
    dome_client.setSendAlivePeriod(alive_period_ms);

    // Set the exit callback to the console handler for safety.
    console_cfg.setExitCallback(
        [&dome_client]()
        {
            std::cout << std::endl;
            std::cout << "Stopping the client..." << std::endl;
            dome_client.stopClient();
        });

    // -- OTHER FUNCTIONS
    // ---------------------------------------

    // Start the server.
    bool started = dome_client.startClient();

    // Check if the server starts ok.
    if(!started)
    {
        std::cout<<"Unable to start the client.";
        return 1;
    }

    // Strong to store each command.
    std::string command;

    // Infinite loop for getting commands.
    while(true)
    {
        // Get the command and parameters.
        std::cout<<"------------------------------------------------------"<<std::endl;
        std::cout<<"Write a command: ";
        std::getline(std::cin, command);

        // Check for other commands.
        if(command == "help")
        {
            printSFELDomeCommands();
            continue;
        }
        else if(command == "exit")
        {
            std::cout << "Stopping the client..." << std::endl;
            dome_client.stopClient();
            break;
        }
        else if(command == "auto_alive_en")
        {
            std::cout << "Enabling auto-alive..." << std::endl;
            dome_client.doDisconnect();
            dome_client.doConnect(true);
            continue;
        }
        else if(command == "auto_alive_ds")
        {
            std::cout << "Disabling auto-alive..." << std::endl;
            dome_client.disableAutoAlive();
            continue;
        }
        else if(command == "auto_alive_clbk_en")
        {
            std::cout << "Enabling auto-alive clbk..." << std::endl;
            dome_client.setAliveCallbacksEnabled(true);
            continue;
        }
        else if(command == "auto_alive_clbk_ds")
        {
            std::cout << "Disabling auto-alive clbk..." << std::endl;
            dome_client.setAliveCallbacksEnabled(false);
            continue;
        }

        // Break if we want to close the example program.
        if(console_cfg.closeStatus() || std::cin.eof())
        {
            console_cfg.waitForClose();
            break;
        }

        // Parse the command.
        client_parser.parseCommand(command);
    }

    // Final log.
    std::cout << "Client stoped. All ok!!" << std::endl;

    // Restore the console.
    console_cfg.restoreConsole();

    // Return.
	return 0;
}

// ---------------------------------------------------------------------------------------------------------------------
