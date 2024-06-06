// =====================================================================================================================
#pragma once
// =====================================================================================================================

// C++ INCLUDES
// =====================================================================================================================
#include <string>
// =====================================================================================================================

// LIBZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/Modules/CommandServerClient>
#include <LibZMQUtils/Modules/Utilities>
// =====================================================================================================================

// INTERFACE INCLUDES
// =====================================================================================================================
#include <LibSFELDomeController/dome_controller_data.h>
#include <LibSFELDomeController/dome_controller_server_data.h>
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace sfeldome{
namespace communication{
// =====================================================================================================================

class DomeControllerClient : public zmqutils::reqrep::DebugCommandClientBase
{
public:

    DomeControllerClient(const std::string& server_endpoint,
                         const std::string& client_iface = "",
                         const std::string& client_name = "",
                         const std::string& client_version = "",
                         const std::string& client_info = "",
                         bool log_internal_callbacks = true);

    DomeControllerClient(const std::string& server_endpoint,
                         const std::string& client_iface = "",
                         bool log_internal_callbacks = true);

    template<typename CmdId>
    bool validateCommand(CmdId command) const
    {
        return (zmqutils::reqrep::CommandClientBase::isBaseCommand(command) ||
                (static_cast<std::int32_t>(command) >= kMinCmdId && static_cast<std::int32_t>(command) <= kMaxCmdId));
    }

    zmqutils::reqrep::OperationResult getHomePosition(controller::AltAzPos &pos, controller::DomeError &res);

    zmqutils::reqrep::OperationResult setHomePosition(const controller::AltAzPos &pos, controller::DomeError &res);

    zmqutils::reqrep::OperationResult openSerialPort(const std::string&, controller::DomeError &res);
};

}} // END NAMESPACES.
// =====================================================================================================================
