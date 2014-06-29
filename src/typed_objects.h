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
    v8::Local<v8::Value> Get(v8::Handle<v8::Value> key);
    bool Set(v8::Handle<v8::Value> key, v8::Handle<v8::Value> value, v8::PropertyAttribute attribs=v8::None);
    bool Delete(v8::Handle<v8::Value> key);

  private:
    TypedObject();
    ~TypedObject();
    unsigned int seed;
    v8::Local<v8::Number> defaultValue;
    enum ValueTypes {
      Double,
      Int
    };
    ValueTypes defaultValueType;
    static v8::Handle<v8::Value> New(const v8::Arguments &args);
};

#endif
