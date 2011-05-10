# Naudio

Play sounds or streams with nodejs.
____

This is just an experiment to see how hard is it to make an audio player with nodejs. It uses the libvlc so it can read pretty much any kind of sound file/stream. 

See test.js for usage example.

## Compiling

For now (since I'm new to this wscript thing) you need to edit manually the wscript and add paths your to the libvlc.

On Mac OS X you will need to do this too : 

    install_name_tool -change @loader_path/lib/libvlc.5.dylib <PATH TO THE LIBVLC dylib>/libvlc.5.dy

## ATTENTION

Anyone interested in helping me out is welcome, fork, push, pull and all that jazz.

Help with the wscript would be great.