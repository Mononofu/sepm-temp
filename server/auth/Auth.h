#ifndef AUTH_H
#define AUTH_H

#include "zmqpp/zmqpp.hpp"
#include <boost/thread.hpp>

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
};

#endif
