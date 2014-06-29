#include <node.h>
#include <node_buffer.h>
#include <stdlib.h>

#include "xxhash.h"
#include "typed_objects.h"

using namespace v8;

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

  TypedObject* obj = new TypedObject();
  obj->Wrap(args.This());

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
  constructor = Persistent<FunctionTemplate>::New(tpl);
#else
  constructor = Persistent<FunctionTemplate>::New(isolate, tpl);
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

  NODE_MODULE(typed_objects, init);
}
