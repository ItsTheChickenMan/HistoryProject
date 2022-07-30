# HistoryProject

A virtual museum made using the Poor Engine for a history project.

# Prompt

The exact prompt for the project reads as follows:

"
It’s the year 1968 and you’re celebrating the end of America’s involvement in some of the harsher moments during the Cold War! Looking back on your life over the past years, you realize a lot has happened! Wanting to preserve these events for future generations, you want to create a museum to curiate these events for future generations to appreciate and understand...
"

Basically, I had to make a virtual museum of some form about the Cold War.  I imagine he was expecting a powerpoint or video, so I figured a virtual, walkable museum might surprise him and maybe get me that nice upper 90s grade which I desperately need.  While it's not yet graded, I'll update this README when it is (update: A+!).

# Project

The project is made using (a heavily modified version of) the Poor Engine, an engine which I have been working on for the past month.  It was originally meant for multiplayer games, but I was able to adapt it to work for an offline game like this.  Like I said, I had to do a lot of extra work on it to suit the project's needs, since the engine was still really underdeveloped when I started.  I had to add audio support, asset/texture management and support, better lighting, and make some improvements to the physics engine, which was previously broken.  I definitely had to take a few shortcuts, but overall, I'm fairly happy with the final result.  It's one of the first projects in which I've had to create a program before a deadline, with an idea of what the program needs to look like, for an audience whose reaction I could not predict, but whose reaction would need to be positive because my future depends on it.  And I may have succeeded!  In a way, it's like my first released game.

# Known Bugs/Quirks (note: nothing in this category will be addressed, I just like to keep track of things like this)

- there exists a bug in the physics engine in which you can collide with the top face of a cube when pressed against the side and moving up, and likewise for down.  Because of the way the map is designed, this isn't noticeable.
- VSync is always enabled by default and can't be changed other than by changing the source code.
- The force which is applied to you when pressing keys is affected by delta.  However, friction (technically drag, not friction, but it's called friction in the code because I'm stupid) is not; it is consistently 0.3.  Therefore, if your framerate is low, you'll actually move faster, and vice versa.
- The "intended" delta (the value which delta is expected to be) is always 1000/60, or 16.6666667, which is the expected value when the monitor's refresh rate is 60 fps.  I can imagine this creating problems for high refresh rate monitors.
- The program will occasionally seg fault when closing because I overlooked a problem with how SFML audio works.  Audio is played in a seperate thread, and when the program ends, it stops all playing audio and then destroys all loaded SoundAssets.  Destroying a sound asset before a sound finishes playing throws a seg fault, so what I imagine is happening is that the sounds aren't fully stopped in their threads when I try to destroy assets, and the program seg faults.  This would explain why it doesn't always happen: because occasionally, the sounds stop in time or there aren't any sounds playing.  This isn't noticeable, however, because it happens when the program is closing.
- The program needed very little modification in order to use clientside movement, this is because I actually set the networking module's send port and listen port to the same values, and set the server ip to localhost.  This means that every time it sends out a request, it receives it on the same port and parses it immediately, effectively turning the client into its own server.  The only problem with this is that it still triggers the firewall, which is a little annoying, so I left a disclaimer about that for my history teacher.
- I accidentally left in a speed up key: Q.  This doubles your speed, I used it when making the program to let me get around the museum faster.  I didn't tell my history teacher about this key, and I imagine it's unlikely he'll find it, but it would be funny if he did.
- Holding both W and a strafe key will cause you to move faster in a diagonal direction because of an oversight.  For instance, let's say when you hold the W key, it applies 1 unit of force in your forward direction.  The magnitude of your current velocity is 1.  The same applies to the A and D keys, with their respective directions.  So when you hold both W and A, you're applying 1 unit of force in your forward direction, and 1 unit of force to your left.  The magnitude of your velocity would then be sqrt(2), or 1.41421356.  So in this program, pressing both W and A increases your speed from 0.048 to 0.048\*sqrt(2), or 0.067882251.  Combine this with the speed up key Q, and you can reach up to 0.141421356! 

# Closing Notes

This project currently is in the exact same way that my history teacher received it, with the exception of his name being redacted in bin/READTHISFIRST.txt.  If I update my history teacher's version, I will update this version, but other than that, this repository just serves as an archive.  

I've only tested this on Windows 10 x64, and as far as I know, it doesn't work on Windows 7 due to some complications with the audio libraries flac, ogg, and vorbis.  If you want to run this on Windows 7 (which should be possible) then I would recommend recompiling those libraries on your system.  In the future, I'll remember to find a way to make those more cross compatible, or to just release multiple builds.
