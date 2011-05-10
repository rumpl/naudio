var Naudio = require('./build/default/naudio').Naudio;

var naudio = new Naudio();

/*
// You can plays streams
naudio.play('http://72.13.81.178:8020', function () {
    console.log('stream ended');
});
*/

// Or odrinary files.
naudio.play('test.wav', function() { console.log('done')});

console.log('end');