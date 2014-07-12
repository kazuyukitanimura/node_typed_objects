//
//  main.cpp
//  hashly
//
//  Created by Kazuyuki Tanimura on 7/4/14.
//  Copyright (c) 2014 Kazuyuki Tanimura. All rights reserved.
//

#include <iostream>
#include "hashly.h"
#include <ctime>
#include <vector>
#include <unordered_map>
#include <google/dense_hash_map>

#define START begin = std::clock()
#define END(name) duration = 1000 * 1000 * 1000 / size * double(std::clock() - begin) / CLOCKS_PER_SEC; std::cout << (name) << ": " << duration << " ns/op\n"

typedef std::unordered_map<std::string, double > MapType;
MapType unordered_map;

int main(int argc, const char * argv[]) {
  // regression test...
  std::cout << "Hashly performance test!\n";
  Hashly* h = new Hashly(0.0);
  uint32_t size = 10000;
  clock_t begin;
  double duration;
  std::vector<std::string> keys(size);

  for (uint32_t i = size; i--;) {
    keys[i] = std::to_string(i);
  }

  START;
  for (uint32_t i = size; i--;) {
    h->set(keys[i], (double)i);
  }
  END("set");

  START;
  for (uint32_t i = size; i--;) {
    if (h->get(keys[i]) != (double)i) {
      std::cout << "actual: " << h->get(keys[i]) << ", expected: " << (double)i << "\n";
    }
  }
  END("get");

  START;
  for (uint32_t i = size; i--;) {
    h->del(keys[i]);
  }
  END("del");

  delete h;

  std::cout << "\nstd::unordered_map performance test!\n";
  START;
  for (uint32_t i = size; i--;) {
    unordered_map.insert(std::pair<std::string, double >(keys[i], (double)i));
  }
  END("set");

  START;
  for (uint32_t i = size; i--;) {
    auto itr = unordered_map.find(keys[i]);
    if (itr->second != (double)i) {
      std::cout << "actual: " << itr->second << ", expected: " << (double)i << "\n";
    }
  }
  END("get");

  START;
  for (uint32_t i = size; i--;) {
    unordered_map.erase(unordered_map.find(keys[i]));
  }
  END("del");

  std::cout << "\nnanahan_map performance test!\n";

  return 0;
}

