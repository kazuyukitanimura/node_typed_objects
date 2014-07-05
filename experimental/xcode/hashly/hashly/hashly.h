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

const uint8_t BUCKET_SIZE = 32;
const uint8_t BIT = 32;

class Item {
public:
  Item(std::string key, double val, uint32_t hash);
  ~Item();
  std::string key;
  double val;
  uint32_t hash;
};

class Bucket {
public:
  Bucket(double defaultValue);
  ~Bucket();
  double find(uint32_t hash, uint8_t count);
  bool insert(Item* newItem, uint8_t count);
  bool remove(uint32_t hash, uint8_t count);
private:
  double defaultValue;
  Item items[BUCKET_SIZE];
};

class Node {
public:
  Node(double defaultValue, Bucket* oldBucket);
  ~Node();
  Bucket* bucket;
  uint8_t count;
};

class Hashly {
public:
  Hashly(double defaultValue);
  ~Hashly();
  double operator[](std::string &key);
private:
  Node arrayedTree[(1 << 10) - 1];
  uint8_t minHeight;
  double defaultValue;
  uint32_t seed = rdtsc();
};


#endif
