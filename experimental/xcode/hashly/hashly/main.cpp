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
#define END(name) duration = double(std::clock() - begin) / CLOCKS_PER_SEC; std::cout << (name) << ": " << duration << " seconds\n"


int main(int argc, const char * argv[]) {
  // dumb unit test...
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

  bool res = true;
  START;
  for (uint32_t i = size; i--;) {
    res &= (h->get(std::to_string(i)) == (double)i);
  }
  END("get");
  std::cout << "res: " << res << "\n";

  START;
  for (uint32_t i = size; i--;) {
    h->del(std::to_string(i));
  }
  END("del");

  return 0;
}

