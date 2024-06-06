// PROJECT INCLUDES
// =====================================================================================================================
#include "LibSFELDomeController/dome_controller_server.h"
#include "LibSFELDomeController/dome_controller_server_data.h"
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace sfeldome{
namespace communication{
// =====================================================================================================================

// ---------------------------------------------------------------------------------------------------------------------
using zmqutils::reqrep::ServerCommand;
using zmqutils::reqrep::CommandType;
// ---------------------------------------------------------------------------------------------------------------------

DomeControllerServer::DomeControllerServer(unsigned int server_port,
                                           const std::string &server_iface,
                                           bool log_internal_callbacks) :
    DebugClbkCommandServerBase(server_port,
                               server_iface,
                               log_internal_callbacks)
{
    this->registerCommandToStrLookup(DomeServerCommandStr);
}

DomeControllerServer::DomeControllerServer(unsigned int server_port,
                                           const std::string &server_iface,
                                           const std::string &server_name,
                                           const std::string &server_version,
                                           const std::string &server_info,
                                           bool log_internal_callbacks) :
    DebugClbkCommandServerBase(server_port,
                               server_iface,
                               server_name,
                               server_version,
                               server_info,
                               log_internal_callbacks)
{
    this->registerCommandToStrLookup(DomeServerCommandStr);
}

bool DomeControllerServer::validateCustomRequest(const zmqutils::reqrep::CommandRequest &request) const
{
    zmqutils::reqrep::CommandType cmd = static_cast<zmqutils::reqrep::CommandType>(request.command);
    return cmd >= kMinCmdId && cmd <= kMaxCmdId;
}


}} // END NAMESPACES.
// =====================================================================================================================
