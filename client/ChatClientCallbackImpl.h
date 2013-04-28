#include "SecureDistributedChat.h"

class ChatClientCallbackImpl : public virtual sdc::ChatClientCallbackI {
public:
  void initChat(const sdc::StringSeq&, const string&, const sdc::ByteSeq&, const Ice::Current&) {}
  void addChatParticipant(const sdc::User&, const string&, const Ice::Current&) {}
  void removeChatParticipant(const sdc::User&, const string&, const Ice::Current&) {}
  void appendMessageToChat(const sdc::ByteSeq&, const string&, const sdc::User&, const Ice::Current&) {}
  std::string echo(const string &s, const Ice::Current&) { return s; }
};
