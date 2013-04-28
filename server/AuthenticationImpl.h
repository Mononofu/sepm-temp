#include "SecureDistributedChat.h"
#include "SessionImpl.h"
#include "Ice.h"
using namespace std;

class AuthenticationImpl : public virtual sdc::AuthenticationI {
public:
  AuthenticationImpl(IceChat *c) : chat(c) { }
  string echo(const string &s, const Ice::Current&) { return s; }
  void registerUser(const sdc::User &, const string &, const Ice::Current&);
  sdc::SessionIPrx login(const sdc::User &user, const string &pw,
    const Ice::Identity&, const Ice::Current&);
private:
  IceChat *chat;
};
