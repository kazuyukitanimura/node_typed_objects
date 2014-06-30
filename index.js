var _TypedObject = require('./build/Release/typed_objects').TypedObject;

var TypedObject = module.exports = function(defaultVal, existing) {
  if (! (this instanceof TypedObject)) { // enforcing new
    return new TypedObject(defaultVal, existing);
  }
  _TypedObject.call(this, defaultVal);
  var key = '';
  var keys = Object.keys(existing || {});
  for (var i = keys.length; i--;) {
    key = keys[i];
    this.set(key, existing[key]);
  }
};
TypedObject.prototype = Object.create(_TypedObject.prototype);
