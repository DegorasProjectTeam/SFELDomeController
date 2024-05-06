// =====================================================================================================================
#pragma once
// =====================================================================================================================

// C++ INCLUDES
// =====================================================================================================================
#include <string>
// =====================================================================================================================

// LIBZMQUTILS INCLUDES
// =====================================================================================================================
#include <LibZMQUtils/Modules/CommandClient>
#include <LibZMQUtils/Modules/Utilities>
// =====================================================================================================================

// INTERFACE INCLUDES
// =====================================================================================================================
#include <LibSFELDomeController/DomeController/dome_controller_data.h>
#include <LibSFELDomeController/DomeControllerServer/dome_controller_server_data.h>
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace sfeldome{
namespace communication{
// =====================================================================================================================

class DomeControllerClient : public zmqutils::serverclient::CommandClientBase
{
public:

    DomeControllerClient(const std::string& server_endpoint,
                         const std::string& client_name = "",
                         const std::string interf_name = "");

    template<typename CmdId>
    bool validateCommand(CmdId command) const
    {
        return (CommandClientBase::validateCommand(command) ||
                (static_cast<std::int32_t>(command) >= kMinCmdId && static_cast<std::int32_t>(command) <= kMaxCmdId));
    }

    zmqutils::serverclient::OperationResult getHomePosition(controller::AltAzPos &pos,
                                                            controller::DomeError &res);

    zmqutils::serverclient::OperationResult setHomePosition(const controller::AltAzPos &pos,
                                                            controller::DomeError &res);

    zmqutils::serverclient::OperationResult openSerialPort(const std::string&,
                                                           controller::DomeError &res);


private:

    virtual void onClientStart() override final;

    virtual void onClientStop() override final;

    virtual void onWaitingReply() override final;

    virtual void onDeadServer() override final;

    virtual void onConnected() override final;

    virtual void onDisconnected() override final;

    virtual void onInvalidMsgReceived(const zmqutils::serverclient::CommandReply&) override final;

    virtual void onReplyReceived(const zmqutils::serverclient::CommandReply& reply) override final;

    virtual void onSendingCommand(const zmqutils::serverclient::RequestData&) override final;

    virtual void onClientError(const zmq::error_t&, const std::string& ext_info) override final;
};

}} // END NAMESPACES.
// =====================================================================================================================
