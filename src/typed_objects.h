#ifndef TYPEDOBJECT_H
#define TYPEDOBJECT_H

#include <node.h>

class TypedObject : public node::ObjectWrap {
  public:
    static void Init(v8::Handle<v8::Object> exports);

  private:
    TypedObject();
    ~TypedObject();
    v8::Local<v8::Value> defaultValue;
    static v8::Handle<v8::Value> New(const v8::Arguments &args);
    static v8::Handle<v8::Value> Get(const v8::Arguments &args);
    static v8::Handle<v8::Value> Set(const v8::Arguments &args);
    static v8::Handle<v8::Value> Has(const v8::Arguments &args);
    static v8::Handle<v8::Value> Del(const v8::Arguments &args);
};

#endif
