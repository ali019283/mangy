# mangy
mangy is a minimal window manager using vim keybindings.

don't expect much out of it, it may lack some important features and it may have bugs.

# how to use
```
+-------------------------------------------------------------------+
| -> exec mangy                                                     |
| -> modkey + esc            Closes the connection to X server      |
| -> modkey + q              Closes the focused window              |
| -> modkey + h,j,k,l        Moves focused window                   |
| -> modkey + arrow keys     Resizes focused window.                |
+-------------------------------------------------------------------+
```
modkey by default is Mod4Mask, you can change it in mangy.c
as for other keys they are specified by their keycodes

# installing / compiling
run 
```
# make
```
to compile

run 
```
# make install
```
to install
