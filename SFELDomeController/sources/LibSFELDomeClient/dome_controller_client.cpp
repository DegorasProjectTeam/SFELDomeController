

// PROJECT INCLUDES
// =====================================================================================================================
#include "LibSFELDomeClient/dome_controller_client.h"
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace sfeldome{
namespace communication{
// =====================================================================================================================

// ---------------------------------------------------------------------------------------------------------------------
using zmqutils::reqrep::ServerCommand;
using zmqutils::reqrep::OperationResult;
using zmqutils::reqrep::ResultType;
using zmqutils::reqrep::CommandType;
using zmqutils::reqrep::CommandReply;
using zmqutils::reqrep::RequestData;
using zmqutils::serializer::BinarySerializer;
// ---------------------------------------------------------------------------------------------------------------------


DomeControllerClient::DomeControllerClient(const std::string &server_endpoint,
                                           const std::string &client_iface,
                                           const std::string &client_name,
                                           const std::string &client_version,
                                           const std::string &client_info,
                                           bool log_internal_callbacks) :
    DebugCommandClientBase(server_endpoint,
                           client_iface,
                           client_name,
                           client_version,
                           client_info,
                           log_internal_callbacks)
{
    this->registerCommandToStrLookup(DomeServerCommandStr);
}

DomeControllerClient::DomeControllerClient(const std::string &server_endpoint,
                                           const std::string &client_iface,
                                           bool log_internal_callbacks) :
    DebugCommandClientBase(server_endpoint,
                           client_iface,
                           log_internal_callbacks)
{
    this->registerCommandToStrLookup(DomeServerCommandStr);
}

OperationResult DomeControllerClient::getHomePosition(controller::AltAzPos &pos, controller::DomeError &res)
{
    return this->executeCommand(DomeServerCommand::REQ_GET_HOME_POSITION, res, pos);
}

OperationResult DomeControllerClient::setHomePosition(const controller::AltAzPos &pos,
                                                      controller::DomeError &res)
{
    RequestData request = this->prepareRequestData(pos);
    return this->executeCommand(DomeServerCommand::REQ_SET_HOME_POSITION, request, res);
}

OperationResult DomeControllerClient::openSerialPort(const std::string& serial_port, controller::DomeError &res)
{
    RequestData request = this->prepareRequestData(serial_port);
    return this->executeCommand(DomeServerCommand::REQ_OPEN_SERIAL_PORT, request, res);
}


}} // END NAMESPACES.
// =====================================================================================================================
