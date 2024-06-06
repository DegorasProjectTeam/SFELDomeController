

/** ********************************************************************************************************************
 * @file dome_controller_server_data.h
 * @brief EXAMPLE FILE - This file contains common elements for the example AmelasServer module.
 * @author Degoras Project Team
 * @copyright EUPL License
***********************************************************************************************************************/

// =====================================================================================================================
#pragma once
// =====================================================================================================================

// LIBZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/Modules/CommandServerClient>
// =====================================================================================================================

// SFEL DOME NAMESPACES
// =====================================================================================================================
namespace sfeldome{
namespace communication{
// =====================================================================================================================

// Specific subclass commands (0 to 30 are reserved for the base server).
// WARNING: In our approach, the server commands must be always in order.
enum class DomeServerCommand : zmqutils::reqrep::CommandType
{
    REQ_FIND_HOME = 31,
    REQ_INC_TARGET,
    REQ_SET_TARGET,
    REQ_GET_POS,
    REQ_EN_MOVEMENT,
    END_IMPL_COMMANDS,
    END_DOME_COMMANDS
};

// Specific subclass errors (0 to 30 are reserved for the base server).
enum class DomeOperationResult : zmqutils::reqrep::ResultType
{};

// Extend the base command strings with those of the subclass.
static constexpr auto DomeServerCommandStr = zmqutils::utils::joinArraysConstexpr(
zmqutils::reqrep::ServerCommandStr, std::array<const char*, 7>
{
    "REQ_FIND_HOME",
    "REQ_INC_TARGET",
    "REQ_SET_TARGET",
    "REQ_GET_POS",
    "REQ_EN_MOVEMENT",
    "END_IMPL_COMMANDS",
    "END_DOME_COMMANDS"
});

// Extend the base result strings with those of the subclass.
static constexpr auto DomeOperationResultStr = zmqutils::utils::joinArraysConstexpr(
zmqutils::reqrep::OperationResultStr, std::array<const char*, 2>
{
});

// Usefull const expressions.
constexpr std::int32_t kMinCmdId = static_cast<std::int32_t>(zmqutils::reqrep::ServerCommand::END_BASE_COMMANDS) + 1;
constexpr std::int32_t kMaxCmdId = static_cast<std::int32_t>(DomeServerCommand::END_DOME_COMMANDS) - 1;

}} // END NAMESPACES.
// =====================================================================================================================
