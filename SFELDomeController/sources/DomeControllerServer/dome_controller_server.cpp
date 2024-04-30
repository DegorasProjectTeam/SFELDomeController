// PROJECT INCLUDES
// =====================================================================================================================
#include "LibSFELDomeController/DomeControllerServer/dome_controller_server.h"
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace sfeldome{
namespace communication{
// =====================================================================================================================

// ---------------------------------------------------------------------------------------------------------------------
using zmqutils::serverclient::ServerCommand;
using zmqutils::serverclient::OperationResult;
using zmqutils::serverclient::ResultType;
using zmqutils::serverclient::CommandRequest;
using zmqutils::serverclient::CommandReply;
using zmqutils::serverclient::HostInfo;
using zmqutils::serializer::BinarySerializer;
// ---------------------------------------------------------------------------------------------------------------------

void DomeControllerServer::processSetHomePosition(const CommandRequest& request, CommandReply& reply)
{
    // Auxiliar variables and containers.
    controller::DomeError ctrl_err;

    // Position struct.
    controller::AltAzPos pos;

    // Check the request parameters size.
    if (request.params_size == 0 || !request.params)
    {
        reply.server_result = OperationResult::EMPTY_PARAMS;
        return;
    }

    // Try to read the parameters data.
    try
    {
        BinarySerializer::fastDeserialization(request.params.get(), request.params_size, pos);
    }
    catch(...)
    {
        reply.server_result = OperationResult::BAD_PARAMETERS;
        return;
    }

    // Now we will process the command in the controller.
    ctrl_err = this->invokeCallback<controller::SetHomePositionCallback>(request, reply, pos);

    // Serialize parameters if all ok.
    if(reply.server_result == OperationResult::COMMAND_OK)
        reply.params_size = BinarySerializer::fastSerialization(reply.params, ctrl_err);
}


}} // END NAMESPACES.
// =====================================================================================================================
