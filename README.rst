Userspace Driver for 1d57:ac01 USB Controller / Presenter
=========================================================

:Author:    Weisi Dai <multiple1902@gmail.com>
:Date:      Jul 6, 2012

This is a user-space parser for the 1d57:ac01 USB HID controller, which acts as a keyboard (actually presenter) in my case. It receives signals using libusb 1.0, and sends the key to X11. That is to say, you'll need the following when building:

* libusb 1.0 and its devel package
* libX11 and its devel package
* ``gcc`` from GCC, or ``clang`` from LLVM

The presenter is OK with computers that are running Windows, but can't be treated as keyboard on my openSUSE.

Released under GNU GPL v3.

Compiling
---------

If you like to use ``gcc``, just type ``make``::

    gcc -std=c99 -Wall -lusb-1.0 -lX11 -lXtst -o presenter presenter.c

or if you prefer ``clang``, type ``make clang``::

    clang -std=c99 -Wall -lusb-1.0 -lX11 -lXtst -o presenter presenter.c

Both works well on my laptop.

Troubleshooting
---------------

If running ``presenter`` from command line, remember to set the ``DISPLAY`` environment variable early. For the first X screen, use::

    export DISPLAY=:0
    ./presenter

or you may trigger segmentaion fault.
