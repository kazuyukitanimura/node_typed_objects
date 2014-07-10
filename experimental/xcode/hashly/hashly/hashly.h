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
unsigned int rdtsc() {
  unsigned int lo, hi;
  __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
  return hi ^ lo;
}

#define BUCKET_SIZE 32
#define BIT 32
#define ArrayMalloc(type, size) ((type *) malloc((size) * sizeof(type)))
#define Hash uint32_t hash = XXH32(key.c_str(), (int) key.length(), seed)
#define I uint32_t i = (1 << minHeight) - 1 + (hash >> (BIT - minHeight))
#define NextI i = (i << 1) + 1 + ((hash >> bit) & 1)
#define LocalNode Node node = arrayedTree[i]
#define LocalBucket Bucket* bucket = node.bucket


class Item {
  friend class Hashly;
  friend class Bucket;
private:
  Item(std::string &key, double val, uint32_t hash);
  ~Item();
  std::string key;
  double val;
  uint32_t hash;
};

class Bucket {
  friend class Hashly;
  friend class Node;
private:
  Bucket(double defaultValue);
  ~Bucket();
  double find(uint32_t hash, uint8_t count);
  bool insert(Item* newItem, uint8_t count);
  bool insert(std::string &key, double val, uint32_t hash, uint8_t count);
  bool remove(uint32_t hash, uint8_t count);
  double _defaultValue;
  Item* items;
};

class Node {
  friend class Hashly;
private:
  Node(double defaultValue, Bucket* oldBucket);
  ~Node();
  Bucket* bucket;
  uint8_t count;
};

class Hashly {
public:
  Hashly(double defaultValue);
  ~Hashly();
  double get(std::string &key);
  bool set(std::string &key, double val);
  bool has(std::string &key);
  bool del(std::string &key);
private:
  Node* arrayedTree;
  uint8_t minHeight;
  double _defaultValue;
  uint32_t seed = rdtsc();
  inline void _updateMinHeight(bool decrement);
};


#endif
