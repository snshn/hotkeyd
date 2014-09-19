hotkeyd
===

Fork of keyd (originally developed by Benjamin Bolton, https://bennybolton.com)

LICENSE
---
	keyd is licensed under the GNU General Public License version 3 (GPLv3)
	http://www.gnu.org/licenses/quick-guide-gplv3.html

BUILD
---
	To build simply run "make"
	To change the c compiler used CC variable to the desired c compiler

INSTALLING
---
	To install simply run "make install"
	To change to root dir for installing (e.g. for building a binary package),
	change the DESTDIR variable to the desired location, e.g.
		make DESTDIR=pkg/ install

UNINSTALLING
---
	To uninstall simply run "make uninstall"
	Note: This makes use of the DESTDIR variable

CONFIGURING
---
	The default configuration script is located at "/etc/keyd.conf". The format
	of this file is:
		<KEYS> <COMMAND>
	where <KEYS> is a list of modifiers and a key seperated by a '+', e.g.:
		F1          <command when F1 pressed>
		LEFTCTRL+F1 <command when Control-F1 pressed>
	You can use "/usr/bin/keyd --test" to find key names
	After changing the configuration file, keyd will need to been restarted,
	e.g. with systemd as root:
		systemctl restart keyd.service
	or with initscripts:
		/etc/rc.d/keyd restart

RUNNING
---
	the Makefile will install daemon packages for initscripts and systemd.
	To start the daemon using systemd run as root:
		systemctl start keyd.service
	and to stop it run as root:
		systemctl stop keyd.service
	and to enable it to run at startup run as root:
		systemctl enable keyd.service
	To start the daemon using initscripts run as root:
		/etc/rc.d/keyd start
	and to stop it run as root:
		/etc/rc.d/keyd stop
	and to enable it to run at startup add keyd to the DAEMONS array in
	/etc/rc.conf
	keyd can also be run indepent of the daemon scripts by running
	"/usr/bin/keyd", see "/usr/bin/keyd --help" for help running keyd
