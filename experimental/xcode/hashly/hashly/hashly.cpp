//
//  hashly.cpp
//  hashly
//
//  Created by Kazuyuki Tanimura on 7/4/14.
//  Copyright (c) 2014 Kazuyuki Tanimura. All rights reserved.
//

#include "hashly.h"
#include "xxhash.h"

//TODO change to struct
Item::Item(std::string &key, double val, uint32_t hash) {
  this->key = key;
  this->val = val;
  this->hash = hash;
}
Item::~Item() {}

Bucket::Bucket(double defaultValue) : _defaultValue(defaultValue){
  items = ArrayMalloc(Item, BUCKET_SIZE);
}
Bucket::~Bucket() {
  free(items);
}

double Bucket::find(uint32_t hash, uint8_t count) {
  for (uint8_t i = count; i--;) {
    Item item = items[i];
    if (item.hash == hash) { // or compare the key
      return item.val;
    }
  }
  return _defaultValue;
}

bool Bucket::insert(Item* newItem, uint8_t count) {
  Item item = items[count];
  item.key = newItem->key; // copy only pointer
  item.val = newItem->val;
  item.hash = newItem->hash;
  return true;
}

bool Bucket::insert(std::string &key, double val, uint32_t hash, uint8_t count) {
  for (uint8_t i = count; i--;) {
    Item item = items[i];
    if (item.hash == hash) { // or compare the key
      items[i].val = val;
      return false;
    }
  }
  Item item = items[count];
  item.key = key;
  item.val = val;
  item.hash = hash;
  return true;
}

bool Bucket::remove(uint32_t hash, uint8_t count) {
  for (uint8_t i = count; i--;) {
    Item item = items[i];
    if (item.hash == hash) { // or compare the key
      items[i] = items[count - 1];
      return true;
    }
  }
  return false;
}

Node::Node(double defaultValue, Bucket* oldBucket = NULL) {
  count = 0;
  bucket = (oldBucket == NULL) ? new Bucket(defaultValue): oldBucket;
}
Node::~Node() {
  bucket->~Bucket();
}

Hashly::Hashly(double defaultValue) : _defaultValue(defaultValue) {
  arrayedTree =  ArrayMalloc(Node, (1 << 10) - 1);
  arrayedTree[0] = *(new Node(defaultValue));
  minHeight = 0;
}
Hashly::~Hashly() {
  free(arrayedTree);
}

void Hashly::_updateMinHeight(bool decrement) {
  minHeight -= decrement;
  uint32_t upper = (1 << (minHeight + 1)) - 1;
  uint32_t lower = (1 << minHeight) - 1;
  bool res = true;
  for (uint32_t i = lower; i < upper && res; i++) {
    res &= (arrayedTree[i].bucket == NULL);
  }
  minHeight += res;
}

double Hashly::get(std::string &key) {
  Hash;
  I;
  for (uint8_t bit = minHeight; bit < BIT; bit++) {
    LocalNode;
    LocalBucket;
    if (bucket != NULL) {
       return bucket->find(hash, node.count);
    }
    NextI;
  }
  return _defaultValue;
}

bool Hashly::set(std::string &key, double val) {
  Hash;
  I;
  for (uint8_t bit = minHeight; bit < BIT; bit++) {
    LocalNode;
    LocalBucket;
    if (bucket != NULL) {
      uint8_t count = node.count;
      if (count < BUCKET_SIZE) {
        node.count += bucket->insert(key, val, hash, count); // increment if append returns true
        return true;
      } else { // it is full, so add a bucket
        node.count = 0;
        node.bucket = NULL;
        uint32_t i_shift = i << 1;
        Node leftChild = arrayedTree[i_shift | 1] = *new Node(_defaultValue, bucket);
        Node rightChild = arrayedTree[i_shift + 2] = *new Node(_defaultValue);
        Bucket* rBucket = rightChild.bucket;
        uint8_t rCount = rightChild.count;
        uint32_t mask = 0x80000000 >> bit;
        for (uint8_t j = count; j--;) {
          Item item = bucket->items[j];
          if (item.hash & mask) {
            rBucket->insert(&item, rCount++);
            bucket[j] = bucket[--count];
          }
        }
        leftChild.count = count;
        rightChild.count = rCount;
        _updateMinHeight(false);
      }
    }
    NextI;
  }
  return false; // you never reach here, fake IDE
}

bool Hashly::has(std::string &key) {
  return get(key) != _defaultValue;
};

bool Hashly::del(std::string &key) {
  Hash;
  I;
  for (uint8_t bit = minHeight; bit < BIT; bit++) {
    LocalNode;
    LocalBucket;
    if (bucket != NULL) {
      bool res = bucket->remove(hash, node.count);
      if (res && ! (--node.count) && i) { // if it becomes empty after deletion
        uint32_t left = i & 1; // left is always an odd number
        uint32_t i_1 = i - 1;
        Node sibling = arrayedTree[i_1 + left * 2];
        uint32_t parent = i_1 >> 1;
        if (sibling.bucket != NULL) { // if sibling does not have children
          arrayedTree[parent] = sibling;
          _updateMinHeight(true);
        }
      }
      return res;
    }
    NextI;
  }
  return false; // you never reach here, fake IDE
}