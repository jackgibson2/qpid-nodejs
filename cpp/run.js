var module = require('./build/Release/helloworld');
var hello= new module.HelloWorld();
console.warn(hello.hello());
