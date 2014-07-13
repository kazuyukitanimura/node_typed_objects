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
  rCount = BIT;
}
Bucket::~Bucket() {
  delete[] items;
}

double Bucket::find(uint32_t hash) {
  if (hash & 1) {
    for (uint8_t i = rCount; i < BIT; i++) {
      if (items[i].hash == hash) { // or compare the key
        return items[i].val;
      }
    }
  } else {
    for (uint8_t i = count; i--;) {
      if (items[i].hash == hash) { // or compare the key
        return items[i].val;
      }
    }
  }
  return _defaultValue;
}

bool Bucket::insert(Item* newItem) {
  items[count].key = newItem->key;
  items[count].val = newItem->val;
  items[count].hash = newItem->hash;
  count++;
  return true;
}

bool Bucket::rInsert(Item* newItem) {
  rCount--;
  items[rCount].key = newItem->key;
  items[rCount].val = newItem->val;
  items[rCount].hash = newItem->hash;
  return true;
}

bool Bucket::insert(const char *key, double val, uint32_t hash) {
  if (hash & 1) {
    for (uint8_t i = rCount; i < BIT; i++) {
      if (items[i].hash == hash) { // or compare the key
        items[i].val = val;
        return false;
      }
    }
    /**
     * We do not do this in a copy constructor in order to avoid allocating
     * extra memory for the right hand side values (i.e., key, val, hash)
     */
    rCount--;
    items[rCount].key = key;
    items[rCount].val = val;
    items[rCount].hash = hash;
  } else {
    for (uint8_t i = count; i--;) {
      if (items[i].hash == hash) { // or compare the key
        items[i].val = val;
        return false;
      }
    }
    /**
     * We do not do this in a copy constructor in order to avoid allocating
     * extra memory for the right hand side values (i.e., key, val, hash)
     */
    items[count].key = key;
    items[count].val = val;
    items[count].hash = hash;
    count++;
  }
  return true;
}

bool Bucket::remove(uint32_t hash) {
  if (hash & 1) {
    for (uint8_t i = rCount; i < BIT; i++) {
      if (items[i].hash == hash) { // or compare the key
        items[i] = items[rCount++];
        return true;
      }
    }
  } else {
    for (uint8_t i = count; i--;) {
      if (items[i].hash == hash) { // or compare the key
        items[i] = items[--count];
        return true;
      }
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
  arrayedTree =  ArrayMalloc(Bucket*, (1 << 13) - 1); // TODO realloc
  arrayedTree[0] = new Bucket(defaultValue);
  minHeight = 0;
}
Hashly::~Hashly() {
  uint32_t upper = (1 << (minHeight + 1)) - 1;
  uint32_t lower = (1 << minHeight) - 1;
  for (uint32_t i = lower; i < upper; i++) {
    _free(i);
  }
  free(arrayedTree);
}

/**
 * Recursively free the buckets in arrayedTree
 */
void Hashly::_free(uint32_t i) {
  if (arrayedTree[i] == NULL) {
    _free((i << 1) + 1);
    _free((i << 1) + 2);
  } else {
    delete arrayedTree[i];
  }
}

void Hashly::_updateMinHeight(bool decrement) {
  minHeight -= decrement;
  uint32_t lower = (1 << minHeight) - 1;
  uint32_t upper = (lower << 1) | 1;
  bool res = true;
  for (uint32_t i = lower; i < upper && res; i++) {
    res = (arrayedTree[i] == NULL);
  }
  minHeight += res;
}

double Hashly::get(const char *key, int len) {
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

bool Hashly::set(const char *key, int len, double val) {
  Hash;
  I;
  HashlyFor {
    LocalBucket;
    IfBucket {
      uint8_t count = bucket->count;
      uint8_t rCount = bucket->rCount;
      if (count != rCount) {
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
            bucket->items[j] = bucket->items[--count];
          }
        }
        leftChild->count = count;
        for (uint8_t j = rCount; j < BIT; j++) {
          Item item = bucket->items[j];
          if (item.hash & mask) {
            rightChild->rInsert(&item);
            bucket->items[j] = bucket->items[rCount++];
          }
        }
        leftChild->rCount = rCount;
        _updateMinHeight(false);
      }
    }
    NextI;
  }
  return false; // you never reach here, fake IDE
}

bool Hashly::has(const char *key, int len) {
  return get(key, len) != _defaultValue;
};

bool Hashly::del(const char *key, int len) {
  Hash;
  I;
  HashlyFor {
    LocalBucket;
    IfBucket {
      bool res = false;
      res = bucket->remove(hash);
      if (res && ! (bucket->count) && (bucket->rCount == BIT) && i) { // if it becomes empty after deletion
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