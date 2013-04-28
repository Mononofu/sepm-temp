#include "SecureDistributedChat.h"
using namespace std;

class SessionImpl : public virtual sdc::SessionI {
public:
  SessionImpl(sdc::User);
  void logout(const Ice::Current&);
  sdc::User retrieveUser(const string&, const Ice::Current&) { return sdc::User(); }
  std::string initChat(const Ice::Current&) { return ""; }
  void leaveChat(const string &chat, const Ice::Current&);
  void invite(const sdc::User &other, const string &chat, const sdc::ByteSeq &key, const Ice::Current&);
  void sendMessage(const sdc::ByteSeq &msg, const string &chat, const Ice::Current&);
  void deleteUser(const sdc::User&, const Ice::Current&) {}
  void saveLog(const string&, Ice::Long, const sdc::SecureContainer&, const Ice::Current&) {}
  sdc::Loglist retrieveLoglist(const Ice::Current&) { return sdc::Loglist(); }
  sdc::SecureContainer retrieveLog(const string&, Ice::Long, const Ice::Current&) { return sdc::SecureContainer();}
  void saveContactList(const sdc::SecureContainer&, const Ice::Current&) {}
  sdc::SecureContainer retrieveContactList(const Ice::Current&) { return sdc::SecureContainer(); }
private:
  bool loggedIn;
  sdc::User user;
};
