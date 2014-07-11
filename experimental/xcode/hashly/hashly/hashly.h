//
//  hashly.h
//  hashly
//
//  Created by Kazuyuki Tanimura on 7/4/14.
//  Copyright (c) 2014 Kazuyuki Tanimura. All rights reserved.
//

#ifndef hashly_hashly_h
#define hashly_hashly_h

#include <stdint.h>
#include <stdlib.h>
#include <string>

// http://stackoverflow.com/questions/7617587/is-there-an-alternative-to-using-time-to-seed-a-random-number-generation
inline unsigned int rdtsc() {
  unsigned int lo, hi;
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return hi ^ lo;
}

#define BUCKET_SIZE 32
#define BIT 32
#define ArrayMalloc(type, size) ((type *) malloc((size) * sizeof(type)))
#define Hash uint32_t hash = XXH32(key.c_str(), (int) key.length(), seed)
#define I uint32_t i = (1 << minHeight) - 1 + (minHeight && (hash >> (BIT - minHeight)))
#define HashlyFor for (uint8_t bit = minHeight; bit < BIT; bit++)
#define NextI i = (i << 1) + 1 + ((hash >> (BIT - 1 - bit)) & 1)
#define LocalBucket Bucket* bucket = arrayedTree[i]
#define IfBucket if (bucket != NULL)


struct Item {
  std::string key;
  double val;
  uint32_t hash;
};

class Bucket {
  friend class Hashly;
private:
  Bucket(double defaultValue);
  ~Bucket();
  double find(uint32_t hash);
  bool insert(Item* newItem);
  bool insert(const std::string &key, double val, uint32_t hash);
  bool remove(uint32_t hash);
  double _defaultValue;
  Item* items;
  uint8_t count;
};

class Hashly {
public:
  Hashly(double defaultValue);
  ~Hashly();
  double get(const std::string &key);
  bool set(const std::string &key, double val);
  bool has(const std::string &key);
  bool del(const std::string &key);
private:
  Bucket** arrayedTree;
  uint8_t minHeight;
  double _defaultValue;
  uint32_t seed = rdtsc();
  inline void _updateMinHeight(bool decrement);
  void _free(uint32_t height);
};


#endif
