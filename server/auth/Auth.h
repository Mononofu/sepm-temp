#ifndef AUTH_H
#define AUTH_H

#include "zmqpp/zmqpp.hpp"
#include <boost/thread.hpp>
#include <map>

using namespace std;

class Auth {
public:
  Auth(zmqpp::context *ctx);
  ~Auth();
  void operator()();
private:
  bool running;
  zmqpp::context *context;
  boost::thread *my_thread;
  zmqpp::socket *auth_rep_sock;
  zmqpp::socket *msg_sock;
  map<string, string> users; // username <-> pubkey
};

#endif
