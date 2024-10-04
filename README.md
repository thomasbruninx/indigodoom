# Doom for IndigoCare's SIP Touch

This is a port of Doom for IndigoCare's SIP Touch wall display.

## The target

IndigoCare is a Belgian company that's specialized in making nurse call solutions for hospitals and eldercare.
One of their products, the SIP Touch wall display, is a Linux based wall display which is usually used to display notification for nurse's or to allow a patient to call a nurse. The hardware used is mostly custom, however the core component is a VOIPAC i.MX25 SODIMM compute module which runs a custom Linux distro.

The performance of the i.MX25 is fairly limited, however it should be able to handle an optimized port of DOOM.

### Graphics

To display the game I decided to target the Linux framebuffer directly. This means we can directly write to the video memory of the device. The SIP Touch has a 800x480 (16bpp) LCD touch screen. Which means that we have to upscale the original output of the DOOM engine (at 320x200) to 800x240.

However this proved to be fairly challenging because of the fairly slow CPU on the i.MX25. Since all the rendering is done on the CPU and even upscaling the image to 800x480 is very intensive.

### Controls

The SIP Touch itself has no buttons, just a touch screen. It would've been possible to use this as an input source for our source port. However, since I'm not the biggest fan of touch screens to play games I decided on something else.

Indigo Care also has a wide range of nurse call modules in their portfolio, some of which are technically just basic switches. The SIP Touch also exposes up to 5 GPIO pins, which I could use to read these switches.

Although it's limited to only 5 buttons/actions, this proved to be a "great" way to play Doom. You can walk forward, turn right and left, shoot your gun and open doors. The bare minimum to play Doom, but it works.

### Audio

The SIP Touch has 2 speakers and the custom Linux distro is able to use the ALSA library to play sound on these speakers.

However I still haven't managened to get this to work in the current version of this port.

## How to use

This port currently only uses the C standard library and a couple of Linux specific header files. No additional libraries need to be linked. Compiling is fairly simple.

However you'll be needing a cross-compiler targeting the i.MX25 processor (an ARM926EJS). I decided to build mine with Crosstool-NG (my config is also included in this repository), however you can also use buildroot or any other cross-compiler toolkit.

Once you've build and installed your cross-compiler, building this port is as easy as using the Makefile in the root of this project.
`make`

### Customizing the port

There are a few options you can pass to the compiler to customize the port. To apply these, you'll have to add or remove them in the Makefile at the CCFLAGS variable.

| Syntax | Description |
| - | - |
| USEORIGINALRESOLUTION | The graphics will not be upscaled, the output will be at 320x200 |
| NOPIXELCALLOOKUPTABLE | Don't use a lookup table for pixels calculation |

## TODO's

There's still a lot of work to do, here's a list of the things I still want to do, ... some day at least

- Clean up code
- Further optimization since upscaling is still slow
- Improved makefile
- Support for VOIPAC i.MX53 SODIMM (Newer models of SIP Touch)
- Audio output
- Touchscreen support