shadertoy
=========

I love [shadertoy.com](https://www.shadertoy.com/) but I can't stand typing in
a non vi(m) editor.  So this replicates the basic functionality (no audio or VR
craziness) of shadertoy.com.  I still have a number of small improvments and
features I want to add or try out and of course I can/should simplify the code
to the bare minimum necessary but it works well enough already.

	usage: ./shadertoy filename [refreshdelay]

shadertoy will try to reload/recompile the fragment shader in filename
every refreshdelay seconds.

