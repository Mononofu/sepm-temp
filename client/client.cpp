#include <iostream>
#include <boost/program_options.hpp>
#include <boost/bind.hpp>
#include "FileWatcher.h"
#include "ViewRefresher.h"
#include "ExitHandler.h"
#include "PluginManager.h"
#include "zmqpp/zmqpp.hpp"
#include <QtConcurrentRun>
#include "Chat.h"
#include <vector>
#include "Security.h"

namespace po = boost::program_options;
using namespace std;

class MessageReader {
public:
  MessageReader(zmqpp::socket *s) {
    QtConcurrent::run(this, &MessageReader::read, s);
  }

  void read(zmqpp::socket *source) {
    while(true) {
      string msg;
      source->receive(msg);
      cout << "> " << msg << endl;
    }
  }
};

int main(int argc, char** argv) {
  po::options_description desc("Allowed options");
  desc.add_options()
    ("register,r", "register a new account")
    ("help,h", "produce help message")
    ("user,u", po::value<string>(), "choose username")
    ("password,p", po::value<string>(), "choose password")
    ("server,s", po::value<string>(), "choose server")
    ("dump,d", "dump key from server")
    ("retrieve", po::value<string>(), "retrieve a given user from server")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help")) {
    cout << desc << "\n";
    return 1;
  }

  cout << "Hello from client" << endl;


  try {

    ExitHandler::i()->setHandler([](int) {
      // called when SIGINT (eg by Ctrl+C) is received
      // do cleanup

      // bad - cout not guaranteed to work, since not reentrant
      // this is just to show the handler is working
      cout << " Got signal .. terminating" << endl;
    });


    string server = vm.count("server") ? vm["server"].as<string>() : "selinux.inso.tuwien.ac.at";
    Chat chat(server, "1337", "ca.crt");
    cout << "Reply from server: " << chat.echo("Hello world") << endl;

    sdc::Security sec;
    // sdc::ByteSeq pubkey, privkey;
    // sec.genRSAKey(pubkey, privkey);
    // sec.savePrivKey(privkey, "privkey");
    // sec.savePubKey(pubkey, "pubkey");

    auto myPubKey = sec.readPubKey("pubkey");
    auto myPrivKey = sec.readPrivKey("privkey");


    if(vm.count("retrieve")) {
      string target = vm["retrieve"].as<string>();
      cout << "retrieving user " << target << endl;
      auto retrievedUser = chat.inter->retrieveUser(target);
      cout << retrievedUser.ID << endl;
      string dump_key2(retrievedUser.publicKey.begin(), retrievedUser.publicKey.end());
      cout << dump_key2 << endl;
    }

    // string dump_key(pubkey.begin(), pubkey.end());
    // cout << dump_key << endl;

    string username = vm.count("username") ? vm["username"].as<string>() : "mononofu";
    string password = vm.count("password") ? vm["password"].as<string>() : "secret";

    sdc::User u;
    u.ID = username + "@selinux.inso.tuwien.ac.at";
    u.publicKey = myPubKey;

    if(vm.count("register")) {
      cout << "registering" << endl;
      chat.registerUser(u, password);
    }

    cout << "try login" << endl;
    sdc::SessionIPrx session = chat.login(u, password);

    if(vm.count("dump")) {
      cout << "duming key" << endl;
      auto user = session->retrieveUser(u.ID);
      string dump_key2(user.publicKey.begin(), user.publicKey.end());
      cout << dump_key2 << endl;
    }


    string chatID = session->initChat();

    string in;
    while(in != "exit") {
      cin >> in;
      std::vector<Ice::Byte> msg(in.begin(), in.end());
      session->sendMessage(msg, chatID);
    }

    session->logout();

  } catch(PluginException ex) {
    cerr << ex.what() << endl;
  }



  return 0;
}
