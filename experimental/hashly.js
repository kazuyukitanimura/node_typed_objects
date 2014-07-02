var xxhash = require('xxhash').hash;

var BUCKET_SIZE = 32;
var SEED = 12345;
var BIT = 32;

var hashF = function(key) {
  return xxhash(new Buffer('' + key), SEED);
};

var Item = function(key, val, hash) {
  this.key = key;
  this.val = val;
  this.hash = hash;
};

var Bucket = function(defaultValue) { // members are Item
  this.defaultValue = defaultValue;
  Array.call(this, BUCKET_SIZE);
}
Bucket.prototype = Object.create(Array.prototype);

Bucket.prototype.append = function(newItem, count) {
  var newHash = newItem.hash;
  for (var i = 0; i < count; i++) {
    var item = this[i];
    if (item.hash === newHash) { // or compare the key
      this[i] = newItem;
      return false;
    }
  }
  this[count] = newItem;
  return true;
}

Bucket.prototype.find = function(hash, count) {
  for (var i = 0; i < count; i++) {
    var item = this[i];
    if (item.hash === hash) { // or compare the key
      return item.val;
    }
  }
  return this.defaultValue;
}

Bucket.prototype.remove = function(hash, count) {
  for (var i = 0; i < count; i++) {
    var item = this[i];
    if (item.hash === hash) { // or compare the key
      this[i] = this[count - 1];
      return true;
    }
  }
  return false;
}

var Node = function(defaultValue, oldPtr) {
  this.count = 0;
  this.ptr = oldPtr || new Bucket(defaultValue);
};

var Hashly = module.exports = function(defaultValue) {
  this.arrayedTree = [new Node(defaultValue)]; // http://en.wikipedia.org/wiki/Binary_tree
  this.minHeight = 0; // TODO
  this.defaultValue = defaultValue;
};

Hashly.prototype.get = function(key) {
  var hash = hashF(key);
  var i = 0;
  for (var bit = 0; bit < BIT; bit++) {
    var arrayedTree = this.arrayedTree;
    var node = arrayedTree[i];
    var ptr = node.ptr;
    if (ptr) {
      return ptr.find(hash, node.count);
    }
    var mask = 0x80000000 >> bit;
    var right = !! (hash & mask);
    i = (i << 1) + 1 + right; // child node
  }
};

Hashly.prototype.set = function(key, val) {
  var hash = hashF(key);
  var i = 0;
  for (var bit = 0; bit < BIT; bit++) {
    var arrayedTree = this.arrayedTree;
    var node = arrayedTree[i];
    var ptr = node.ptr;
    var mask = 0x80000000 >> bit;
    if (ptr) {
      var count = node.count;
      var newItem = new Item(key, val, hash);
      if (count < BUCKET_SIZE) {
        if (ptr.append(newItem, count)) {
          node.count++;
        }
        return true;
      } else { // it is full, so add a bucket
        node.count = 0;
        node.ptr = null;
        var defaultValue = this.defaultValue;
        var leftChild = arrayedTree[(i << 1) + 1] = new Node(defaultValue, ptr);
        var rightChild = arrayedTree[(i << 1) + 2] = new Node(defaultValue);
        var rPtr = rightChild.ptr;
        var rCount = rightChild.count;
        for (var j = 0; j < count; j++) {
          var item = ptr[j];
          if (item.hash & mask) {
            rPtr.append(item, rCount++);
            ptr[j--] = ptr[--count];
          }
        }
        leftChild.count = count;
        rightChild.count = rCount;
      }
    }
    var right = !! (hash & mask);
    i = (i << 1) + 1 + right; // child node
  }
};

Hashly.prototype.has = function(key) {
  return this.get(key) !== this.defaultValue;
};

Hashly.prototype.del = function(key) {
  var hash = hashF(key);
  var i = 0;
  for (var bit = 0; bit < BIT; bit++) {
    var arrayedTree = this.arrayedTree;
    var node = arrayedTree[i];
    var ptr = node.ptr;
    if (ptr) {
      var res = ptr.remove(hash, node.count);
      if (res && ! (--node.count)) { // if it becomes empty after deletion
        var left = i & 1; // left is always an odd number
        var sibling = arrayedTree[i + 1 - left * 2];
        var parent = (i - 1) >> 1;
        if (sibling.ptr) { // if sibling does not have children
          arrayedTree[parent] = sibling;
        }
      }
      return res;
    }
    var mask = 0x80000000 >> bit;
    var right = !! (hash & mask);
    i = (i << 1) + 1 + right; // child node
  }
};
