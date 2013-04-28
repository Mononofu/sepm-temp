#include "Auth.h"
#include "AuthMsg.pb.h"
#include "AuthReply.pb.h"
#include "MessageEvent.pb.h"
#include "UserCallback.h"

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

  msg_sock = new zmqpp::socket(*context, zmqpp::socket_type::sub);
  msg_sock->set(zmqpp::socket_option::linger, 10);
  msg_sock->subscribe("");
  msg_sock->bind("ipc://sepm_msg");

  zmqpp::poller poller;
  poller.add(*auth_rep_sock);
  poller.add(*msg_sock);

  try {
    while(running) {
      if(!poller.poll(100 /* ms */))
        continue;

      if(poller.has_input(*auth_rep_sock)) {
        string in;
        auth_rep_sock->receive(in);
        AuthMsg msg;
        msg.ParseFromString(in);
        cout << "Got msg: " << msg.DebugString() << endl;

        switch(msg.action()) {
          case AuthMsg::LOGIN:
            users[msg.id()] = msg.pubkey();
            break;
          case AuthMsg::LOGOUT:
            users.erase(msg.id());
            break;
          default:
            ;
        }

        string out;
        AuthReply rep;
        rep.set_status(AuthReply::SUCCESS);
        rep.SerializeToString(&out);
        auth_rep_sock->send(out);
      }

      if(poller.has_input(*msg_sock)) {
        string in;
        msg_sock->receive(in);
        MessageEvent msg;
        msg.ParseFromString(in);

        cout << "Got msg: " << msg.DebugString() << endl;

        switch(msg.action()) {
          case MessageEvent::SEND_MESSAGE:
            for(auto& kv : users) {
              auto callback = UserCallback::getCallback(kv.first);

              std::vector<Ice::Byte> msgV(msg.msg().begin(), msg.msg().end());
              sdc::User u;
              u.ID = msg.user();
              callback->appendMessageToChat(msgV, msg.chat(), u);
            }
            break;
          default:
            ;
        }
      }
    }
  } catch(zmqpp::zmq_internal_exception &e) {
    cout << "error: " << e.zmq_error() << endl;
  }
  delete auth_rep_sock;
}
