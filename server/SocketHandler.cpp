#include "SocketHandler.h"
#include <string>

using namespace std;

zmqpp::context *SocketHandler::context;
boost::thread_specific_ptr<zmqpp::socket> SocketHandler::auth_req_sock;
boost::thread_specific_ptr<zmqpp::socket> SocketHandler::msg_sock;

void SocketHandler::create_sockets() {
  auth_req_sock.reset(new zmqpp::socket(*context, zmqpp::socket_type::req));
  auth_req_sock->set(zmqpp::socket_option::linger, 10);
  auth_req_sock->connect("ipc://sepm_auth_req");

  msg_sock.reset(new zmqpp::socket(*context, zmqpp::socket_type::pub));
  msg_sock->set(zmqpp::socket_option::linger, 10);
  msg_sock->connect("ipc://sepm_msg");
}

void SocketHandler::destroy_sockets() {
  auth_req_sock.release();
  msg_sock.release();
}

AuthReply SocketHandler::send_auth(AuthMsg msg) {
  string out;
  msg.SerializeToString(&out);
  auth_req_sock->send(out);

  string in;
  auth_req_sock->receive(in);
  AuthReply reply;
  reply.ParseFromString(in);
  return reply;
}

void SocketHandler::send_msg(MessageEvent msg) {
  string out;
  msg.SerializeToString(&out);
  msg_sock->send(out);
}
