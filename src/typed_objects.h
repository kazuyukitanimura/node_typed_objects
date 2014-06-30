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
    static v8::Local<v8::Value> defaultValue;
    enum ValueTypes {
      Double,
      Int
    };
    ValueTypes defaultValueType;
    static v8::Handle<v8::Value> New(const v8::Arguments &args);
    static v8::Handle<v8::Value> Get(v8::Local<v8::String> name, const v8::AccessorInfo& info);
    bool Set(v8::Handle<v8::Value> key, v8::Handle<v8::Value> value, v8::PropertyAttribute attribs=v8::None);
    bool Delete(v8::Handle<v8::Value> key);
};

#endif
