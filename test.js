var Naudio = require('./build/default/naudio').Naudio;

var naudio = new Naudio();

naudio.play('http://72.13.81.178:8020', function () {
    console.log('first ended');
});

setTimeout(function() {
    naudio.play('test.wav', function() { console.log('done')});
}, 1000);

console.log('end');