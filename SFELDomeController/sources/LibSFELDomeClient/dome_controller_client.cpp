

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

OperationResult DomeControllerClient::findHome(controller::DomeError &res)
{
    return this->executeCommand(DomeServerCommand::REQ_FIND_HOME, res);
}

OperationResult DomeControllerClient::incTarget(int deg, controller::DomeError &res)
{
    RequestData req_data = this->prepareRequestData(deg);
    return this->executeCommand(DomeServerCommand::REQ_INC_TARGET, req_data, res);
}

OperationResult DomeControllerClient::setTarget(int deg, controller::DomeError &res)
{
    RequestData req_data = this->prepareRequestData(deg);
    return this->executeCommand(DomeServerCommand::REQ_SET_TARGET, req_data, res);
}

OperationResult DomeControllerClient::getPos(int &deg, controller::DomeError &res)
{
    return this->executeCommand(DomeServerCommand::REQ_GET_POS, res, deg);
}

OperationResult DomeControllerClient::getTarget(int &deg, controller::DomeError &res)
{
    return this->executeCommand(DomeServerCommand::REQ_GET_TARGET, res, deg);
}

OperationResult DomeControllerClient::setEnMovement(bool en, controller::DomeError &res)
{
    RequestData req_data = this->prepareRequestData(en);
    return this->executeCommand(DomeServerCommand::REQ_EN_MOVEMENT, req_data, res);
}

OperationResult DomeControllerClient::stop(controller::DomeError &res)
{
    return this->executeCommand(DomeServerCommand::REQ_STOP, res);
}

}} // END NAMESPACES.
// =====================================================================================================================
