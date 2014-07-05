//
//  hashly.cpp
//  hashly
//
//  Created by Kazuyuki Tanimura on 7/4/14.
//  Copyright (c) 2014 Kazuyuki Tanimura. All rights reserved.
//

#include "hashly.h"
#include "xxhash.h"

Item::Item(std::string key, double val, uint32_t hash) {
  this->key = key;
  this->val = val;
  this->hash = hash;
}
Item::~Item() {}

Bucket::Bucket(double defaultValue) {
  this->defaultValue = defaultValue;
}
Bucket::~Bucket() {}

double Bucket::find(uint32_t hash, uint8_t count) {
  for (uint8_t i = count; i--;) {
    Item item = this->items[i];
    if (item.hash == hash) { // or compare the key
      return item.val;
    }
  }
  return this->defaultValue;
}

bool Bucket::insert(Item* newItem, uint8_t count) {
  uint32_t newHash = newItem->hash;
  for (uint8_t i = count; i--;) {
    Item item = this->items[i];
    if (item.hash == newHash) { // or compare the key
      this->items[i] = *newItem;
      return false;
    }
  }
  this->items[count] = *newItem;
  return true;
}

bool Bucket::remove(uint32_t hash, uint8_t count) {
  for (uint8_t i = count; i--;) {
    Item item = this->items[i];
    if (item.hash == hash) { // or compare the key
      this->items[i] = this->items[count - 1];
      return true;
    }
  }
  return false;
}

Node::Node(double defaultValue, Bucket* oldBucket = NULL) {
  this->count = 0;
  this->bucket = (oldBucket == NULL) ? new Bucket(defaultValue): oldBucket;
}
Node::~Node() {}

Hashly::Hashly(double defaultValue) {
  this->arrayedTree[0] = *(new Node(defaultValue));
  this->minHeight = 0;
  this->defaultValue = defaultValue;
}
Hashly::~Hashly() {}

double Hashly::operator[](std::string &key) {
  uint32_t hash = XXH32(key.c_str(), (int) key.length(), this->seed);
  uint8_t minHeight = this->minHeight;
  uint8_t i = (1 << minHeight) - 1 + (hash >> (BIT - minHeight));
  for (uint8_t bit = minHeight; bit < BIT; bit++) {
    Node node = this->arrayedTree[i];
    Bucket* bucket = node.bucket;
    if (bucket) {
       return bucket->find(hash, node.count);
    }
    uint8_t right = (hash >> bit) & 1;
    i = (i << 1) + 1 + right; // child node
  }
  return this->defaultValue;
}
