/***********************************************************************************************************************
 *   LibZMQUtils (ZeroMQ High-Level Utilities C++ Library).                                                            *
 *
 *   ExamplesLibZMQUtils related project.                                                                            *
 *                                                                                                        *
 *   A modern open-source C++ library with high-level utilities based on the well-known ZeroMQ open-source universal   *
 *   messaging library. Includes custom command based server-client and publisher-subscriber with automatic binary     *
 *   serialization capabilities, specially designed for system infraestructure. Developed as a free software under the *
 *   context of Degoras Project for the Spanish Navy Observatory SLR station (SFEL) in San Fernando and, of course,    *
 *   for any other station that wants to use it!                                                                       *
 *                                                                                                                     *
 *   Copyright (C) 2024 Degoras Project Team                                                                           *
 *                      < Ángel Vera Herrera, avera@roa.es - angeldelaveracruz@gmail.com >                             *
 *                      < Jesús Relinque Madroñal >                                                                    *                                                            *
 *                                                                                                                     *
 *   This file is part of LibZMQUtils.                                                                                 *
 *                                                                                                                     *
 *   Licensed under the European Union Public License (EUPL), Version 1.2 or subsequent versions of the EUPL license   *
 *   as soon they will be approved by the European Commission (IDABC).                                                 *
 *                                                                                                                     *
 *   This project is free software: you can redistribute it and/or modify it under the terms of the EUPL license as    *
 *   published by the IDABC, either Version 1.2 or, at your option, any later version.                                 *
 *                                                                                                                     *
 *   This project is distributed in the hope that it will be useful. Unless required by applicable law or agreed to in *
 *   writing, it is distributed on an "AS IS" basis, WITHOUT ANY WARRANTY OR CONDITIONS OF ANY KIND; without even the  *
 *   implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the EUPL license to check specific   *
 *   language governing permissions and limitations and more details.                                                  *
 *                                                                                                                     *
 *   You should use this project in compliance with the EUPL license. You should have received a copy of the license   *
 *   along with this project. If not, see the license at < https://eupl.eu/ >.                                         *
 **********************************************************************************************************************/

/** ********************************************************************************************************************
 * @file dome_controller_server.h
 * @brief EXAMPLE FILE - This file contains the declaration of the AmelasServer example class.
 * @author Degoras Project Team
 * @copyright EUPL License
***********************************************************************************************************************/

// =====================================================================================================================
#pragma once
// =====================================================================================================================

// C++ INCLUDES
// =====================================================================================================================
// =====================================================================================================================

// LIBZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/Modules/CommandServerClient>
#include <LibZMQUtils/Modules/Utilities>
// =====================================================================================================================

// PROJECT INCLUDES
// =====================================================================================================================
// =====================================================================================================================

// SFEL DOME NAMESPACES
// =====================================================================================================================
namespace sfeldome{
namespace communication{
// =====================================================================================================================

// Example of creating a command server from the base.
class DomeControllerServer : public zmqutils::reqrep::DebugClbkCommandServerBase
{
public:

    DomeControllerServer(unsigned server_port,
                         const std::string& server_iface = "*",
                         bool log_internal_callbacks = true);

    DomeControllerServer(unsigned server_port,
                         const std::string& server_iface = "*",
                         const std::string& server_name = "",
                         const std::string& server_version = "",
                         const std::string& server_info = "",
                         bool log_internal_callbacks = true);

private:

    // -----------------------------------------------------------------------------------------------------------------
    using CommandServerBase::registerReqProcFunc;
    using ClbkCommandServerBase::registerCallback;
    // -----------------------------------------------------------------------------------------------------------------

    // CommandServerBase interface
    bool validateCustomRequest(const zmqutils::reqrep::CommandRequest &request) const;
};

}} // END NAMESPACES.
// =====================================================================================================================
