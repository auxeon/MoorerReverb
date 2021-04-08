# MoorerReverb
Moorer Reverb Research Paper Implementation
-----

### wav file source path
* MoorerReverb\reverb.hpp
* MoorerReverb\reverb.cpp


### vst solution path
MoorerReverb/vst/AUReverb/Builds/VisualStudio2019

### vst source code path
* MoorerReverb\vst\AUReverb\Source

gain 	: [0.0, 1.0 ]
mix	    : [0.0, 1.0 ]	0[dry] 1[wet]
a	    : [0.0, 0.99]	all pass gain
m	    : [0.0, 1500]	all pass delay in ms
g[0-5]	: [0.0, 1.0 ]	comb filter gain
R[0-5]	: [0.0, 1.0 ]	comb filter coefficient lp
L[0-5]	: [0.0, 1.0 ]	comb filter delay in ms

### extras
* Realtime Reverb PLugin
* Responsive controls g<->R correlation
* RealTime Wave form display
* Optimized for VST use as well
