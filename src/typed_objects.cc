#include <node.h>
#include <node_buffer.h>
#include <stdlib.h>

#include "xxhash.h"
#include "typed_objects.h"

using namespace v8;

Handle<Value> TypedObject::Get(const Arguments& args) {
#if NODE_MAJOR_VERSION == 0 && NODE_MINOR_VERSION < 11
  HandleScope scope;
#else
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
#endif

  if (args.Length() != 1) {
    return ThrowException(Exception::Error(
#if NODE_MAJOR_VERSION == 0 && NODE_MINOR_VERSION < 11
      String::New("TypedObject#Get takes exactly one argument.")
#else
      String::NewFromUtf8(isolate, "TypedObject#Get takes exactly one argument.")
#endif
    ));
  }

  TypedObject* obj = ObjectWrap::Unwrap<TypedObject>(args.This());
  return scope.Close(obj->defaultValue);
}

Handle<Value> TypedObject::Set(const Arguments& args) {
#if NODE_MAJOR_VERSION == 0 && NODE_MINOR_VERSION < 11
  HandleScope scope;
#else
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
#endif

  if (args.Length() != 2) {
    return ThrowException(Exception::Error(
#if NODE_MAJOR_VERSION == 0 && NODE_MINOR_VERSION < 11
      String::New("TypedObject#Set takes exactly two arguments.")
#else
      String::NewFromUtf8(isolate, "TypedObject#Set takes exactly two arguments.")
#endif
    ));
  }

  TypedObject* obj = ObjectWrap::Unwrap<TypedObject>(args.This());
  String::AsciiValue keyAscii(args[0]->ToString());
  unsigned int hash = ::XXH32(*keyAscii, keyAscii.length(), obj->seed);
}

Handle<Value> TypedObject::Has(const Arguments& args) {
#if NODE_MAJOR_VERSION == 0 && NODE_MINOR_VERSION < 11
  HandleScope scope;
#else
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
#endif

}

Handle<Value> TypedObject::Del(const Arguments& args) {
#if NODE_MAJOR_VERSION == 0 && NODE_MINOR_VERSION < 11
  HandleScope scope;
#else
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
#endif

}

Handle<Value> TypedObject::New(const Arguments& args) {
#if NODE_MAJOR_VERSION == 0 && NODE_MINOR_VERSION < 11
  HandleScope scope;
#else
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
#endif

  if (!args.IsConstructCall()) {
    return ThrowException(Exception::Error(
#if NODE_MAJOR_VERSION == 0 && NODE_MINOR_VERSION < 11
      String::New("Use `new` to create instances of this object.")
#else
      String::NewFromUtf8(isolate, "Use `new` to create instances of this object.")
#endif
    ));
  }

  uint32_t argsLen = args.Length();
  if (argsLen < 1) {
    return ThrowException(Exception::Error(
#if NODE_MAJOR_VERSION == 0 && NODE_MINOR_VERSION < 11
      String::New("TypedObject takes at least one argument.")
#else
      String::NewFromUtf8(isolate, "TypedObject takes at least one argument.")
#endif
    ));
  }
  if (!args[0]->IsNumber()) { // it will also check !args[1]->IsInt32() && !args[1]->IsUint32()
    return ThrowException(Exception::Error(
#if NODE_MAJOR_VERSION == 0 && NODE_MINOR_VERSION < 11
      String::New("Unsupported default value.")
#else
      String::NewFromUtf8(isolate, "Unsupported default value.")
#endif
    ));
  }

  TypedObject* obj = new TypedObject();
  obj->seed = rdtsc();
  obj->defaultValue = args[0];
  obj->defaultValueType = (args[0]->IsInt32() || args[0]->IsUint32())? Int: Double;
  obj->Wrap(args.This());

  if (argsLen > 1 && args[1]->IsObject()) {
    Local<Object> object = args[1]->ToObject();
    Local<Array> keys = object->GetOwnPropertyNames();
    for (uint32_t i = keys->Length(); i--;) {
      Local<String> key = keys->Get(i)->ToString();
      //obj->Set(key, object->Get(key));
    }
  }

  return args.This();
}

void TypedObject::Init(Handle<Object> exports) {
#if NODE_MAJOR_VERSION == 0 && NODE_MINOR_VERSION < 11
  HandleScope scope;
#else
  Isolate* isolate = Isolate::GetCurrent();
#endif

  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  Local<String> name = String::NewSymbol("TypedObject");

#if NODE_MAJOR_VERSION == 0 && NODE_MINOR_VERSION < 11
  Persistent<FunctionTemplate> constructor = Persistent<FunctionTemplate>::New(tpl);
#else
  Persistent<FunctionTemplate> constructor = Persistent<FunctionTemplate>::New(isolate, tpl);
#endif
  constructor->InstanceTemplate()->SetInternalFieldCount(1);
  constructor->SetClassName(name);
  //constructor->PrototypeTemplate()->SetNamedPropertyHandler(Get, Set, Has, Del, For);
  NODE_SET_PROTOTYPE_METHOD(constructor, "get", Get);
  NODE_SET_PROTOTYPE_METHOD(constructor, "set", Set);
  NODE_SET_PROTOTYPE_METHOD(constructor, "has", Has);
  NODE_SET_PROTOTYPE_METHOD(constructor, "del", Del);

  exports->Set(name, constructor->GetFunction());
}

TypedObject::TypedObject() {}

TypedObject::~TypedObject() {}

extern "C" {
  void init(Handle<Object> exports) {
    TypedObject::Init(exports);
  }

  NODE_MODULE(typed_objects, init)
}
