
Debian
====================
This directory contains files used to package deliond/delion-qt
for Debian-based Linux systems. If you compile deliond/delion-qt yourself, there are some useful files here.

## delion: URI support ##


delion-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install delion-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your delionqt binary to `/usr/bin`
and the `../../share/pixmaps/delion128.png` to `/usr/share/pixmaps`

delion-qt.protocol (KDE)

