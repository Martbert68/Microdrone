# Microdrone QUADMODE

Welcome to Quadmode! I will describe this is part 3 of the Video series. Which I haven't filmed yet.

At the end of part two the Microdrone had one operating mode. To change modes I would have needed to add another switch and I really didnt want to change the hardware.
I also wanted to be able free the oscillators from 8 frepeqncy locked; phase shifters into 16 oscillators.

The solution! Depending on the position of the tuning knob at power on, left or right, the code branches in two ways.
Press the button at power on and two more modes are availabe.

This version of the code has 4 modes

1) Classic phase shifted algorithm
2) A quad note drone builder that has a random walk in the frequencies. Makes giant, ominous droning.
3) A tinkabell. 16 exponentially spaced notes switch on and off in sequence.
4) 16 random frequencies that hold for a random amount of time. Its a perfect mode if you want a little droney blast after a difficult phone converastion.

I was going to quantize this thing to real notes...maybe that will be an option in the next release but I have sealed the case and disconnected the SWD cables now.


This is the code for the microdrone. It is a very simple 8 oscillator audio drone.

I describe its conception here. I doni't see this as a "thing" more of a demonstration and
development of an idea.

https://www.youtube.com/watch?v=QqVuwxzvt6Q&lc=UgyV2Gorxfad10N3GFp4AaABAg

To be honest with you I am not actually sure what this thing is for. As a drone oscillator that can be sent on to other pedals and effects 
maybe it has some value. But it doesn't have a midi or control voltage interface or even a stylus so its not even in stylophone territory..  
It does however do one thing really well and this is probably the only reason I bothered with it; it varies its volume by using precise out i
of phase oscillators. This gave me a reason to finish soldering it together and put it in a case so I could at least explain it..

