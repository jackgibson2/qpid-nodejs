/* This code is PUBLIC DOMAIN, and is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND. See the accompanying 
 * LICENSE file.
 */
#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Message.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Session.h>
#include <iostream>
#include <v8.h>
#include <node.h>

using namespace node;
using namespace v8;
using namespace qpid::messaging;

class HelloWorld: ObjectWrap
{
private:
  int m_count;
public:

  static Persistent<FunctionTemplate> s_ct;
  static void Init(v8::Handle<Object> target)
  {
    v8::HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);

    s_ct = Persistent<FunctionTemplate>::New(t);
    s_ct->InstanceTemplate()->SetInternalFieldCount(1);
    s_ct->SetClassName(String::NewSymbol("HelloWorld"));

    NODE_SET_PROTOTYPE_METHOD(s_ct, "hello", Hello);

    target->Set(String::NewSymbol("HelloWorld"),
                s_ct->GetFunction());
  }

  HelloWorld() :
    m_count(0)
  {
  }

  ~HelloWorld()
  {
  }

  static v8::Handle<Value> New(const Arguments& args)
  {
    v8::HandleScope scope;
    HelloWorld* hw = new HelloWorld();
    hw->Wrap(args.This());
    return args.This();
  }

  static v8::Handle<Value> Hello(const Arguments& args)
  {
    HandleScope scope;
    HelloWorld* hw = ObjectWrap::Unwrap<HelloWorld>(args.This());
    hw->m_count++;


    std::string broker = "localhost:5672";
    std::string address = "amq.topic";
    std::string connectionOptions = "";
    std::string content;
    Connection connection(broker, connectionOptions);
    connection.open();
    Session session = connection.createSession();

    Receiver receiver = session.createReceiver(address);
    Sender sender = session.createSender(address);

    sender.send(qpid::messaging::Message("Hello world!"));

    qpid::messaging::Message message = receiver.fetch(Duration::SECOND * 1);
    content =  message.getContent();
    session.acknowledge();

    connection.close();


    Local<String> result = v8::String::New(content.data());

    return scope.Close(result);
  }

};

Persistent<FunctionTemplate> HelloWorld::s_ct;

extern "C" {
  static void init (v8::Handle<Object> target)
  {
    HelloWorld::Init(target);
  }

  NODE_MODULE(helloworld, init);
}
