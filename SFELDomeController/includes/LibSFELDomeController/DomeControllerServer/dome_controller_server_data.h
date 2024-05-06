

/** ********************************************************************************************************************
 * @file dome_controller_server_data.h
 * @brief EXAMPLE FILE - This file contains common elements for the example AmelasServer module.
 * @author Degoras Project Team
 * @copyright EUPL License
***********************************************************************************************************************/

// =====================================================================================================================
#pragma once
// =====================================================================================================================

// ZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/Modules/CommandServer>
// =====================================================================================================================

// SFEL DOME NAMESPACES
// =====================================================================================================================
namespace sfeldome{
namespace communication{
// =====================================================================================================================

// Specific subclass commands (0 to 30 are reserved for the base server).
// WARNING: In our approach, the server commands must be always in order.
enum class DomeServerCommand : zmqutils::serverclient::CommandType
{
    REQ_SET_HOME_POSITION = 31,
    REQ_GET_HOME_POSITION = 32,
    REQ_OPEN_SERIAL_PORT  = 33,
    END_IMPL_COMMANDS     = 34,
    END_DOME_COMMANDS     = 35
};

// Specific subclass errors (0 to 30 are reserved for the base server).
enum class DomeOperationResult : zmqutils::serverclient::ResultType
{};

// Extend the base command strings with those of the subclass.
static constexpr auto DomeServerCommandStr = zmqutils::utils::joinArraysConstexpr(
zmqutils::serverclient::ServerCommandStr, std::array<const char*, 5>
{
    "REQ_SET_HOME_POSITION",
    "REQ_GET_HOME_POSITION",
    "OPEN_SERIAL_PORT",
    "END_IMPL_COMMANDS",
    "END_DOME_COMMANDS"
});

// Extend the base result strings with those of the subclass.
static constexpr auto DomeOperationResultStr = zmqutils::utils::joinArraysConstexpr(
zmqutils::serverclient::OperationResultStr, std::array<const char*, 2>
{
});

// Usefull const expressions.
constexpr std::int32_t kMinCmdId = static_cast<std::int32_t>(zmqutils::serverclient::ServerCommand::END_BASE_COMMANDS) + 1;
constexpr std::int32_t kMaxCmdId = static_cast<std::int32_t>(DomeServerCommand::END_DOME_COMMANDS) - 1;

}} // END NAMESPACES.
// =====================================================================================================================
