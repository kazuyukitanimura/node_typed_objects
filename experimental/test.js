var Hash = require('./hashly');
var assert = require('assert');

var hash = new Hash();
var native = {};

function test_set(size) {
  for (var i = size; i--;) {
    hash.set(i, i);
  }
}

function test_get(size) {
  for (var i = size; i--;) {
    assert.deepEqual(hash.get(i), i);
  }
}

function test_n_set(size) {
  for (var i = size; i--;) {
    native[i] = i;
  }
}

function test_n_get(size) {
  for (var i = size; i--;) {
    native[i];
  }
}

start = Date.now();
test_set(10000);
end = Date.now();
console.log('h set', end - start)

start = Date.now();
test_get(10000);
end = Date.now();
console.log('h get', end - start)

start = Date.now();
test_n_set(10000);
end = Date.now();
console.log('n set', end - start)

start = Date.now();
test_n_get(10000);
end = Date.now();
console.log('n get', end - start)
