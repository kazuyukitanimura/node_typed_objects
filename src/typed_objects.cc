#include <node.h>
#include <node_buffer.h>
#include <stdlib.h>

#include "xxhash.h"
#include "typed_objects.h"

using namespace v8;

Local<Value> TypedObject::Get(Handle<Value> key) {

}

bool TypedObject::Set(Handle<Value> key, Handle<Value> value, PropertyAttribute attribs) {

}

bool TypedObject::Delete(Handle<Value> key) {

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
  obj->defaultValue = args[0]->ToNumber();
  obj->defaultValueType = (args[0]->IsInt32() || args[0]->IsUint32())? Int: Double;
  obj->Wrap(args.This());

  if (argsLen > 1 && args[1]->IsObject()) {
    Local<Object> object = args[1]->ToObject();
    Local<Array> keys = object->GetOwnPropertyNames();
    for (uint32_t i = keys->Length(); i--;) {
      Local<Value> key = keys->Get(i);
      object->Get(key); // Call setter
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

  //NODE_SET_PROTOTYPE_METHOD(constructor, "update", Update);
  //NODE_SET_PROTOTYPE_METHOD(constructor, "digest", Digest);

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
