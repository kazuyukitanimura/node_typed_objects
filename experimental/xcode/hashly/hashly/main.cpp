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

#define START begin = std::clock()
#define END(name) duration = 1000 * double(std::clock() - begin) / CLOCKS_PER_SEC; std::cout << (name) << ": " << duration << " milliseconds\n"


int main(int argc, const char * argv[]) {
  // regression test...
  std::cout << "Hello, World!\n";
  Hashly* h = new Hashly(0.0);
  uint32_t size = 10000;
  clock_t begin;
  double duration;

  START;
  for (uint32_t i = size; i--;) {
    h->set(std::to_string(i), (double)i);
  }
  END("set");

  START;
  for (uint32_t i = size; i--;) {
    if (h->get(std::to_string(i)) != (double)i) {
      std::cout << "actual: " << h->get(std::to_string(i)) << ", expected: " << (double)i << "\n";
    }
  }
  END("get");

  START;
  for (uint32_t i = size; i--;) {
    h->del(std::to_string(i));
  }
  END("del");

  delete h;

  return 0;
}

