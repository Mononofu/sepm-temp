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
    ("help,h", "produce help message")
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

    ViewRefresher refresher;
    FileWatcher watcher("./ui", boost::bind(&ViewRefresher::fileChanged, &refresher, _1, _2, _3));

    // create or modify a file in ./ui to see the changes getting picked up
    FileWatcher demo("./ui", [](string name, bool isDir, FileWatcher::FileEvent e) {
      switch(e) {
        case FileWatcher::CREATE:
          cout << "created ";
          break;
        case FileWatcher::MODIFY:
          cout << "modified ";
          break;
        case FileWatcher::DELETE:
          cout << "deleted ";
          break;
        default:
          cout << "unknown event: ";
      }
      cout << (isDir ? "dir " : "file ");
      cout << name << endl;
    });

    ExitHandler::i()->setHandler([](int) {
      // called when SIGINT (eg by Ctrl+C) is received
      // do cleanup

      // bad - cout not guaranteed to work, since not reentrant
      // this is just to show the handler is working
      cout << " Got signal .. terminating" << endl;
    });

    PluginManager manager;

    manager.listPlugins();

    Chat chat("selinux.inso.tuwien.ac.at", "1337", "ca.crt");
    cout << "Reply from server: " << chat.echo("Hello world") << endl;

    sdc::Security sec;
    // sdc::ByteSeq pubkey, privkey;
    // sec.genRSAKey(pubkey, privkey);
    // sec.savePrivKey(privkey, "privkey");
    // sec.savePubKey(pubkey, "pubkey");

    auto myPubKey = sec.readPubKey("pubkey");
    auto myPrivKey = sec.readPrivKey("privkey");


    // string dump_key(pubkey.begin(), pubkey.end());
    // cout << dump_key << endl;

    sdc::User u;
    u.ID = "mononofu2@selinux.inso.tuwien.ac.at";
    u.publicKey = myPubKey;

    // chat.registerUser(u, "secret");

    cout << "try login" << endl;
    sdc::SessionIPrx session = chat.login(u, "secret");

    auto user = session->retrieveUser("mononofu2@selinux.inso.tuwien.ac.at");
    string dump_key2(user.publicKey.begin(), user.publicKey.end());
    cout << dump_key2 << endl;

    auto decryptedKey = sec.decryptRSA(myPrivKey, user.publicKey);
    string dump_key3(decryptedKey.begin(), decryptedKey.end());
    cout << dump_key3 << endl;

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
