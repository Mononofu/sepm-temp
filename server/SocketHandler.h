#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H

#include "zmqpp/zmqpp.hpp"
#include "AuthMsg.pb.h"
#include "AuthReply.pb.h"
#include "MessageEvent.pb.h"
#include <boost/thread/tss.hpp>

class SocketHandler {
public:
  static void create_sockets();
  static void destroy_sockets();

  static AuthReply send_auth(AuthMsg msg);
  static void send_msg(MessageEvent msg);
  static zmqpp::context *context;
private:
  static boost::thread_specific_ptr<zmqpp::socket> auth_req_sock;
  static boost::thread_specific_ptr<zmqpp::socket> msg_sock;
};


#endif
