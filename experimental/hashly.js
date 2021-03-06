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
};
Bucket.prototype = Object.create(Array.prototype);

Bucket.prototype.append = function(newItem, count) {
  var newHash = newItem.hash;
  for (var i = count; i--;) {
    var item = this[i];
    if (item.hash === newHash) { // or compare the key
      this[i] = newItem;
      return false;
    }
  }
  this[count] = newItem;
  return true;
};

Bucket.prototype.find = function(hash, count) {
  for (var i = count; i--;) {
    var item = this[i];
    if (item.hash === hash) { // or compare the key
      return item.val;
    }
  }
  return this.defaultValue;
};

Bucket.prototype.remove = function(hash, count) {
  for (var i = count; i--;) {
    var item = this[i];
    if (item.hash === hash) { // or compare the key
      this[i] = this[count - 1];
      return true;
    }
  }
  return false;
};

var Node = function(defaultValue, oldPtr) {
  this.count = 0; // maybe we can have another counter to fill items from both top and bottom of the bucket. In order to reduce the number of linear probing, check the next highest bit and decide whether we should put this item from the bottom or top
  this.ptr = oldPtr || new Bucket(defaultValue);
};

var Hashly = module.exports = function(defaultValue) {
  this.arrayedTree = [new Node(defaultValue)]; // http://en.wikipedia.org/wiki/Binary_tree
  this.minHeight = 0; // real min height - 1
  //this.minHeightRequest = 0;
  this.defaultValue = defaultValue;
};

Hashly.prototype._updateMinHeight = function(decrement) {
  //if (!decrement) {
  //  if ((1 << this.minHeight) > (++this.minHeightRequest)) {
  //    return;
  //  }
  //  this.minHeightRequest = 0;
  //}
  this.minHeight -= decrement;
  var minHeight = this.minHeight;
  var upper = (1 << (minHeight + 1)) - 1;
  var lower = (1 << minHeight) - 1;
  var arrayedTree = this.arrayedTree;
  var res = true;
  for (var i = lower; i < upper && res; i++) {
    res &= !(arrayedTree[i].ptr);
  }
  this.minHeight += res;
};

Hashly.prototype.get = function(key) {
  var hash = hashF(key);
  var minHeight = this.minHeight;
  var i = (1 << minHeight) - 1 + (minHeight && (hash >>> (BIT - minHeight)));
  for (var bit = minHeight; bit < BIT; bit++) {
    var arrayedTree = this.arrayedTree;
    var node = arrayedTree[i];
    var ptr = node.ptr;
    if (ptr) {
      return ptr.find(hash, node.count);
    }
    var mask = 0x80000000 >>> bit;
    var right = !! (hash & mask);
    i = (i << 1) + 1 + right; // child node
  }
};

Hashly.prototype.set = function(key, val) {
  var hash = hashF(key);
  var minHeight = this.minHeight;
  var i = (1 << minHeight) - 1 + (minHeight && (hash >>> (BIT - minHeight)));
  for (var bit = minHeight; bit < BIT; bit++) {
    var arrayedTree = this.arrayedTree;
    var node = arrayedTree[i];
    var ptr = node.ptr;
    var mask = 0x80000000 >>> bit;
    if (ptr) {
      var count = node.count;
      if (count < BUCKET_SIZE) {
        node.count += ptr.append(new Item(key, val, hash), count); // increment if append returns true
        return true;
      } else { // it is full, so add a bucket
        node.count = 0;
        node.ptr = null;
        var defaultValue = this.defaultValue;
        var i_shift = i << 1;
        var leftChild = arrayedTree[i_shift | 1] = new Node(defaultValue, ptr);
        var rightChild = arrayedTree[i_shift + 2] = new Node(defaultValue);
        var rPtr = rightChild.ptr;
        var rCount = rightChild.count;
        for (var j = count; j--;) {
          var item = ptr[j];
          if (item.hash & mask) {
            rPtr.append(item, rCount++);
            ptr[j] = ptr[--count];
          }
        }
        leftChild.count = count;
        rightChild.count = rCount;
        this._updateMinHeight(false);
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
  var minHeight = this.minHeight;
  var i = (1 << minHeight) - 1 + (minHeight && (hash >>> (BIT - minHeight)));
  for (var bit = minHeight; bit < BIT; bit++) {
    var arrayedTree = this.arrayedTree;
    var node = arrayedTree[i];
    var ptr = node.ptr;
    if (ptr) {
      var res = ptr.remove(hash, node.count);
      if (res && ! (--node.count) && i) { // if it becomes empty after deletion
        var left = i & 1; // left is always an odd number
        var i_1 = i - 1;
        var sibling = arrayedTree[i_1 + left * 2];
        var parent = i_1 >>> 1;
        if (sibling.ptr) { // if sibling does not have children
          arrayedTree[parent] = sibling;
          this._updateMinHeight(true);
        }
      }
      return res;
    }
    var mask = 0x80000000 >>> bit;
    var right = !! (hash & mask);
    i = (i << 1) + 1 + right; // child node
  }
};
