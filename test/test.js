var assert = require("assert");
var TypedObject = require('..');

describe('TypedObject', function() {
  var to = new TypedObject(1.0, {
    a: 1.1,
    'b': 1.2,
    3: 1.3
  });
  to.set('A', 2.1);
  to.set('B-', 2.2);
  describe('#Get', function() {
    it('should to the expected value', function() {
      console.log(to);
      assert.deepEqual(to.get('a'), 1.1);
      assert.deepEqual(to.get('b'), 1.2);
      assert.deepEqual(to.get('3'), 1.3);
      assert.deepEqual(to.get('A'), 2.1);
      assert.deepEqual(to.get('B-'), 2.2);
      assert.deepEqual(to.get('C'), 1.0); // default value
    });
  });
});
