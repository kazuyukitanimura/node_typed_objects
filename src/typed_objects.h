#ifndef TYPEDOBJECT_H
#define TYPEDOBJECT_H

#include <node.h>

// http://stackoverflow.com/questions/7617587/is-there-an-alternative-to-using-time-to-seed-a-random-number-generation
unsigned int rdtsc() {
  unsigned int lo, hi;
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return hi ^ lo;
}

class TypedObject : public node::ObjectWrap {
  public:
    static void Init(v8::Handle<v8::Object> exports);

  private:
    TypedObject();
    ~TypedObject();
    unsigned int seed;
    v8::Local<v8::Value> defaultValue;
    enum ValueTypes {
      Double,
      Int
    };
    ValueTypes defaultValueType;
    static v8::Handle<v8::Value> New(const v8::Arguments &args);
    static v8::Handle<v8::Value> Get(v8::Local<v8::String> key, const v8::AccessorInfo& info);
    static v8::Handle<v8::Value> Set(v8::Local<v8::String> key, v8::Local<v8::Value> value, const v8::AccessorInfo &info);
    static v8::Handle<v8::Integer> Has(v8::Local<v8::String> key, const v8::AccessorInfo& info);
    static v8::Handle<v8::Boolean> Del(v8::Local<v8::String> key, const v8::AccessorInfo& info);
    static v8::Handle<v8::Array> For(const v8::AccessorInfo& info);
};

#endif
