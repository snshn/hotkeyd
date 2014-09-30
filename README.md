hotkeyd
===

Hotkeyd is a tiny daemon which runs in the background and 
executes commands tied to certain keypresses in the config file.

Based on keyd developed by Benjamin Bolton.

CHANGES
---
- renamed to hotkeyd
- no logging to files
- removed the systemd shit


LICENSE
---
hotkeyd is licensed under the GNU General Public License version 3 (GPLv3)
[more info](http://www.gnu.org/licenses/quick-guide-gplv3.html)


COMPILING
---
To build simply run
    `make`
To change the compiler use the CC variable
    `make CC=g++`


INSTALLING
---
To install simply run
    `sudo make install`

To change to root dir for installing (e.g. for building a binary package),
use the DESTDIR variable, e.g.
    `sudo make DESTDIR=pkg/ install`


UNINSTALLING
---
To uninstall simply run `make uninstall`
Note: This makes use of the DESTDIR variable


CONFIGURING
---
The default configuration script is located at "/etc/hotkeyd.conf".

The format of this file is:
    `<KEYS> <COMMAND>`

where <KEYS> is a list of modifiers and a key seperated by a '+', e.g.:
    F1          <command when F1 pressed>
    LEFTCTRL+F1 <command when Control-F1 pressed>

You can use `hotkeyd --test` to get a list of key names

RUNNING
---
the Makefile will install daemon packages for initscripts.

To start the daemon using initscripts run as root:
    `/etc/rc.d/hotkeyd start`
and to stop it run as root:
    `/etc/rc.d/hotkeyd stop`

To enable it to run at startup add hotkeyd to the DAEMONS array of `/etc/rc.conf`

hotkeyd can also be run indepently of the daemon scripts by running
`hotkeyd`, see `hotkeyd --help` for information on usage

THANKS
---
1. to Benjamin Bolton for sharing the keyd's source code

USEFUL LINKS
---
[boycott systemd](http://boycottsystemd.org)


