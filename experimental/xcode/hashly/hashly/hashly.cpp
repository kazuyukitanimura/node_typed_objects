//
//  hashly.cpp
//  hashly
//
//  Created by Kazuyuki Tanimura on 7/4/14.
//  Copyright (c) 2014 Kazuyuki Tanimura. All rights reserved.
//

#include "hashly.h"
#include "xxhash.h"

Bucket::Bucket(double defaultValue) : _defaultValue(defaultValue) {
  items = new Item[BUCKET_SIZE];
  count = 0;
}
Bucket::~Bucket() {
  delete[] items;
}

double Bucket::find(uint32_t hash) {
  for (uint8_t i = count; i--;) {
    Item item = items[i];
    if (item.hash == hash) { // or compare the key
      return item.val;
    }
  }
  return _defaultValue;
}

bool Bucket::insert(Item* newItem) {
  items[count] = *newItem; // Is this ok?
  count++;
  return true;
}

bool Bucket::insert(std::string &key, double val, uint32_t hash) {
  for (uint8_t i = count; i--;) {
    Item item = items[i];
    if (item.hash == hash) { // or compare the key
      items[i].val = val;
      return false;
    }
  }
  Item item = items[count];
  /**
   * We do not do this in a copy constructor in order to avoid allocating
   * extra memory for the right hand side values (i.e., key, val, hash)
   */
  item.key = key;
  item.val = val;
  item.hash = hash;
  count++;
  return true;
}

bool Bucket::remove(uint32_t hash) {
  for (uint8_t i = count; i--;) {
    Item item = items[i];
    if (item.hash == hash) { // or compare the key
      items[i] = items[count - 1];
      count--;
      return true;
    }
  }
  return false;
}

Hashly::Hashly(double defaultValue) : _defaultValue(defaultValue) {
  /**
   * Do not use the new operator for arrayedTree (i.e., new Bucket*[size])
   * since we do not want to call the default constructor of Bucket.
   * Maybe we just should use std::vector here, but we want to make sure
   * that we can exploit the cache locality by allocating raw memory
   */
  arrayedTree =  ArrayMalloc(Bucket*, (1 << 10) - 1); // TODO realloc
  arrayedTree[0] = new Bucket(defaultValue);
  minHeight = 0;
}
Hashly::~Hashly() {
  _free(minHeight);
  free(arrayedTree);
}

/**
 * Recursively free the buckets in arrayedTree
 */
void Hashly::_free(uint32_t height) {
  uint32_t upper = (1 << (height + 1)) - 1;
  uint32_t lower = (1 << height) - 1;
  for (uint32_t i = lower; i < upper; i++) {
    if (arrayedTree[i] == NULL) {
      _free(height + 1);
    } else {
      delete arrayedTree[i];
      //arrayedTree[i]->Bucket::~Bucket();
    }
  }
}

void Hashly::_updateMinHeight(bool decrement) {
  minHeight -= decrement;
  uint32_t upper = (1 << (minHeight + 1)) - 1;
  uint32_t lower = (1 << minHeight) - 1;
  bool res = true;
  for (uint32_t i = lower; i < upper && res; i++) {
    res &= (arrayedTree[i] == NULL);
  }
  minHeight += res;
}

double Hashly::get(std::string &key) {
  Hash;
  I;
  HashlyFor {
    LocalBucket;
    IfBucket {
       return bucket->find(hash);
    }
    NextI;
  }
  return _defaultValue;
}

bool Hashly::set(std::string &key, double val) {
  Hash;
  I;
  HashlyFor {
    LocalBucket;
    IfBucket {
      uint8_t count = bucket->count;
      if (count < BUCKET_SIZE) {
        bucket->insert(key, val, hash);
        return true;
      } else { // it is full, so add a bucket
        arrayedTree[i] = NULL;
        uint32_t i_shift = i << 1;
        Bucket* leftChild = arrayedTree[i_shift | 1] = bucket;
        Bucket* rightChild = arrayedTree[i_shift + 2] = new Bucket(_defaultValue);
        uint32_t mask = 0x80000000 >> bit;
        for (uint8_t j = count; j--;) {
          Item item = bucket->items[j];
          if (item.hash & mask) {
            rightChild->insert(&item);
            bucket[j] = bucket[--count];
          }
        }
        leftChild->count = count;
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
  HashlyFor {
    LocalBucket;
    IfBucket {
      bool res = bucket->remove(hash);
      if (res && ! (bucket->count) && i) { // if it becomes empty after deletion
        uint32_t left = i & 1; // left is always an odd number
        uint32_t i_1 = i - 1;
        Bucket* sibling = arrayedTree[i_1 + (left << 1)];
        uint32_t parent = i_1 >> 1;
        if (sibling != NULL) { // if sibling does not have children
          arrayedTree[parent] = sibling;
          delete bucket;
          _updateMinHeight(true);
        }
      }
      return res;
    }
    NextI;
  }
  return false; // you never reach here, fake IDE
}