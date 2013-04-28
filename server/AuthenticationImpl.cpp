#include "AuthenticationImpl.h"
#include "AuthMsg.pb.h"
#include "SocketHandler.h"

void AuthenticationImpl::registerUser(const sdc::User &u, const string &pw,
  const Ice::Current&) {
  cout << "Registering " << u.ID << " with pw " << pw << endl;

  AuthMsg msg;
  msg.set_action(AuthMsg::REGISTER);
  msg.set_id(u.ID);
  msg.set_pw(pw);
  AuthReply reply = SocketHandler::send_auth(msg);
  cout << "Got reply " << reply.DebugString() << endl;
}

sdc::SessionIPrx AuthenticationImpl::login(const sdc::User &user, const string &pw,
  const Ice::Identity &id, const Ice::Current &cur) {
  AuthMsg msg;
  msg.set_action(AuthMsg::LOGIN);
  msg.set_id(user.ID);
  msg.set_pw(pw);
  AuthReply reply = SocketHandler::send_auth(msg);
  cout << "Got reply " << reply.DebugString() << endl;

  if(reply.status() == AuthReply::SUCCESS) {
    Ice::ObjectPrx proxy = cur.con->createProxy(id);
    sdc::ChatClientCallbackIPrx callback = sdc::ChatClientCallbackIPrx::checkedCast(proxy);
    if(callback->echo("42") == "42") {
      Ice::ObjectPrx prx = this->chat->createAdapter(new SessionImpl(user), cur.con);
      cout << "Login successful" << endl;
      return sdc::SessionIPrx::checkedCast(prx);
    }
  }
  cout << "login failed" << endl;
  throw sdc::AuthenticationException();
}
