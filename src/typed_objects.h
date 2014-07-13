#ifndef TYPEDOBJECT_H
#define TYPEDOBJECT_H

#include <node.h>
#include <google/dense_hash_map>

class TypedObject : public node::ObjectWrap {
  public:
    static void Init(v8::Handle<v8::Object> exports);

  private:
    TypedObject();
    ~TypedObject();
    v8::Local<v8::Value> defaultValue;
    google::dense_hash_map<const char*, double> dense_hash_map;
    static v8::Handle<v8::Value> New(const v8::Arguments &args);
    static v8::Handle<v8::Value> Get(const v8::Arguments &args);
    static v8::Handle<v8::Value> Set(const v8::Arguments &args);
    static v8::Handle<v8::Value> Del(const v8::Arguments &args);
};

#endif
