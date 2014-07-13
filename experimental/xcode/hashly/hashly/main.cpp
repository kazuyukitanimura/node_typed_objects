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

std::unordered_map<const char*, double > unordered_map;
google::dense_hash_map<const char*, double> dense_hash_map;

int main(int argc, const char * argv[]) {
  // regression test...
  std::cout << "Hashly performance test!\n";
  Hashly* h = new Hashly(0.0);
  uint32_t size = 50000;
  clock_t begin;
  double duration;
  std::vector<std::string> keys(size);

  for (uint32_t i = size; i--;) {
    keys[i] = std::to_string(i);
  }

  START;
  for (uint32_t i = size; i--;) {
    h->set(keys[i].c_str(), (int)keys[i].length(), (double)i);
  }
  END("set");

  START;
  for (uint32_t i = size; i--;) {
    if (h->get(keys[i].c_str(), (int)keys[i].length()) != (double)i) {
      std::cout << "actual: " << h->get(keys[i].c_str(), (int)keys[i].length()) << ", expected: " << (double)i << "\n";
    }
  }
  END("get");

  START;
  for (uint32_t i = size; i--;) {
    h->del(keys[i].c_str(), (int)keys[i].length());
  }
  END("del");

  delete h;

  std::cout << "\nstd::unordered_map performance test!\n";
  START;
  for (uint32_t i = size; i--;) {
    unordered_map[keys[i].c_str()] = (double)i;
  }
  END("set");

  START;
  for (uint32_t i = size; i--;) {
    if (unordered_map[keys[i].c_str()] != (double)i) {
      std::cout << "actual: " << unordered_map[keys[i].c_str()] << ", expected: " << (double)i << "\n";
    }
  }
  END("get");

  START;
  for (uint32_t i = size; i--;) {
    unordered_map.erase(keys[i].c_str());
  }
  END("del");

  std::cout << "\ngoogle::dense_hash_map performance test!\n";
  dense_hash_map.set_empty_key("");
  dense_hash_map.set_deleted_key("d");
  START;
  for (uint32_t i = size; i--;) {
    dense_hash_map[keys[i].c_str()] = (double)i;
  }
  END("set");

  START;
  for (uint32_t i = size; i--;) {
    if (dense_hash_map[keys[i].c_str()] != (double)i) {
      std::cout << "actual: " << dense_hash_map[keys[i].c_str()] << ", expected: " << (double)i << "\n";
    }
  }
  END("get");

  START;
  for (uint32_t i = size; i--;) {
    dense_hash_map.erase(keys[i].c_str());
  }
  END("del");

  return 0;
}

