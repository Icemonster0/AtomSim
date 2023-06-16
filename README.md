# AtomSim

AtomSim is a soft body simulation using atoms connected with springs in an [octet truss mesh](https://www.researchgate.net/figure/A-typical-example-of-an-octet-truss-mesh-It-is-comprised-of-an-octahedral-grid-with_fig6_4015919). I wrote it from scratch during a two-week student internship at the local university. You can see the results [here](https://drive.google.com/drive/folders/1Z-G9e8HcBg1xo4Fpxa567JAkOlcG-DZn?usp=sharing).

---

### Requirements

###### OS

Runs on Linux and probably Mac but I'm doubtful about Windows because of weird command line syntax.

###### Packages

AtomSim uses `imagemagick` and `gnuplot`.
- Can't use `imagemagick`? Disable the *output_png* option. This way it will output pnm images which are less commonly used.
- Can't use `gnuplot`? Disable the *temperature_sim* feature. You need `gnuplot` to be able to draw the temperature curves.

---

### Running

###### Configuration

All settings are available in `src/settings.h`. They should all be self-explanatory and there are short descriptions for some of them.

###### Compiling

Run `compile.sh` or its equivalent on your OS to generate the executable.
