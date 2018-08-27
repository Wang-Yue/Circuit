Circuit is a very incomplete simulator of Novation Circuit groovebox. It was a hacking project
for my past weekend, and the prototype showcases a fully fledged groovebox is easy. Most necessary
data structures are there. A few basic UI controls (synth note edit and sample edit) are available.

The project has lots of things missing. Notably:

- Runloop management needs to be done. 
- Many UIs (such as mixer, pattern, fx) are missing. 

Everything inside the Circuit/ directory is cross platform. Views and view controllers are created
from scratch. Currently a very hacky and thin Cocoa wrapper is in the CircuitApp folder. But
ideally this should be headless (with a Novation Launchpad as input and display for instance) or
with a Qt based binding so one can play it in all kinds of devices (such as desktop computers,
mobile devices, or raspberry pis).

I have no current plan for this project. If you have interest in working on the project, please
contact me. PRs are very welcomed.

The code is released in 3-clause BSD license.

![Screenshot](/Screenshot.png?raw=true)

