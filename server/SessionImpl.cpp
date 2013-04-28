#include "SessionImpl.h"
#include "AuthMsg.pb.h"
#include "SocketHandler.h"
#include "MessageEvent.pb.h"

SessionImpl::SessionImpl(sdc::User u) : user(u) {

}

void SessionImpl::logout(const Ice::Current&) {
  AuthMsg msg;
  msg.set_action(AuthMsg::LOGOUT);
  msg.set_id(user.ID);

  SocketHandler::send_auth(msg);
}

void SessionImpl::leaveChat(const string &chat, const Ice::Current&) {
  MessageEvent msg;
  msg.set_action(MessageEvent::LEAVE_CHAT);
  msg.set_user(user.ID);
  msg.set_chat(chat);

  SocketHandler::send_msg(msg);
}


void SessionImpl::invite(const sdc::User &other, const string &chat,
  const sdc::ByteSeq &key, const Ice::Current&) {
  MessageEvent msg;
  msg.set_action(MessageEvent::INVITE_USER);
  msg.set_user(user.ID);
  msg.set_chat(chat);

  msg.set_other(other.ID);

  std::string pubkey(key.begin(), key.end());
  msg.set_pubkey(pubkey);

  SocketHandler::send_msg(msg);
}

void SessionImpl::sendMessage(const sdc::ByteSeq &msg, const string &chat,
  const Ice::Current&) {
  MessageEvent msgE;
  msgE.set_action(MessageEvent::SEND_MESSAGE);
  msgE.set_user(user.ID);
  msgE.set_chat(chat);

  std::string msgstr(msg.begin(), msg.end());
  msgE.set_msg(msgstr);

  SocketHandler::send_msg(msgE);
}
