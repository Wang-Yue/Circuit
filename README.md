Circuit is a incomplete simulator of Novation Circuit groovebox. It was a hacking project for my
past two weekends, and the prototype showcases a fully fledged groovebox is easy. Most necessary
data structures are there. Many UI controls are available:

- Synth mode and Drum mode
- Pattern chain setting
- Patch selection
- Note setting
- Gate setting
- Velocity setting
- Length setting
- Nudge setting

A few UIs (such as mixer, fx, knobs related controls) are still missing, but could be trivial to
implemented. 

Everything inside the Circuit/ directory is cross platform. Views and view controllers are created
from scratch. Currently a very hacky and thin Cocoa wrapper is in the CircuitApp folder. But
ideally this should be headless (with a Novation Launchpad as input and display for instance) or
with a Qt based binding so one can play it in all kinds of devices (such as desktop computers,
mobile devices, or raspberry pis).

I have no current plan for this project. If you have interest in working on the project, please
contact me. PRs are very welcomed.

The code is released in 3-clause BSD license.

![Screenshot](/Screenshot.png?raw=true)

My three-year-old seems to have great fun with this half baked step sequencer (Youtube link):

[![Screenshot](https://img.youtube.com/vi/ho_5mZv6vVc/0.jpg)](https://youtu.be/ho_5mZv6vVc)
