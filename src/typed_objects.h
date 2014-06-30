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
    static v8::Handle<v8::Value> Get(const v8::Arguments &args);
    static v8::Handle<v8::Value> Set(const v8::Arguments &args);
    static v8::Handle<v8::Value> Has(const v8::Arguments &args);
    static v8::Handle<v8::Value> Del(const v8::Arguments &args);
};

#endif
