#include "Auth.h"
#include "AuthMsg.pb.h"
#include "AuthReply.pb.h"

using namespace std;

Auth::Auth(zmqpp::context *ctx) : running(true), context(ctx) {
  my_thread = new boost::thread(*this);
}

Auth::~Auth() {
  running = false;
}

void Auth::operator()() {
  cout << "starting Auth thread" << endl;
  auth_rep_sock = new zmqpp::socket(*context, zmqpp::socket_type::rep);
  auth_rep_sock->set(zmqpp::socket_option::linger, 10);
  auth_rep_sock->bind("ipc://sepm_auth_req");

  try {
    while(running) {
      string in;
      auth_rep_sock->receive(in);
      AuthMsg msg;
      msg.ParseFromString(in);
      cout << "Got msg: " << msg.DebugString() << endl;

      string out;
      AuthReply rep;
      rep.set_status(AuthReply::SUCCESS);
      rep.SerializeToString(&out);
      auth_rep_sock->send(out);
    }
  } catch(zmqpp::zmq_internal_exception &e) {
    cout << "error: " << e.zmq_error() << endl;
  }
  delete auth_rep_sock;
}
