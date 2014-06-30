var assert = require("assert");
var TypedObject = require('..');

describe('TypedObject', function() {
  var to = new TypedObject(1.0, {
    a: 1.1,
    'b': 1.2,
    3: 1.3
  });
  to.A = 2.1;
  to['B-'] = 2.2;
  describe('#Get', function() {
    it('should to the expected value', function() {
      console.log(to);
      assert.deepEqual(to.a, 1.1);
      assert.deepEqual(to.b, 1.2);
      assert.deepEqual(to[3], 1.3);
      assert.deepEqual(to.A, 2.1);
      assert.deepEqual(to['B-'], 2.2);
      assert.deepEqual(to.C, 1.0); // default value
    });
  });
});
