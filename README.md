# mangy
mangy is a minimal window manager using vim keybindings.

don't expect much out of it, it may lack some important features and it may have bugs.

# how to use
```
+-------------------------------------------------------------------+
| -> exec mangy                                                     |
| -> modkey + esc            Closes the connection to X server      |
| -> modkey + q              Closes the focused window              |
| -> modkey + t              Changes the active window place tiling |
| -> modkey + f              Makes focused window fullscreen        |
| -> modkey + g              Switch the focus between windows       |
| -> modkey + enter          Opens terminal                         |
| -> modkey + h,j,k,l        Moves focused window                   |
| -> modkey + Shft + h,j,k,l Resizes focused window                 |
+-------------------------------------------------------------------+
```
modkey by default is Mod1Mask, you can change it in keymap.h
as for other keys they are specified by their keycodes in keymap.h
you can change the default terminal(st)/browser(chromium)/dmenu in mangy.c

# TODO: 
```
-add CreateNotify and DestroyNotify events
-add mfacts [DONE]
-add border
-add gaps [DONE]
-add different tiling methods
 ├──> dwm tiling [DONE]
 ├──> grid
 ├──> fibonacci
 └──> binary space partitioning (prob wont add cuz its complicated asf)
```
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
