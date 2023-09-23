# mangy
mangy is a minimal window manager using vim keybindings.

don't expect much out of it, it may lack some important features and it may have bugs.

# how to use
```
+-------------------------------------------------------------------+
| -> exec mangy                                                     |
| -> modkey + esc            Closes the connection to X server      |
| -> modkey + q              Closes the focused window              |
| -> modkey + f              Makes focused window fullscreen        |
| -> modkey + enter          Opens terminal                         |
| -> modkey + h,j,k,l        Moves focused window                   |
| -> modkey + Shft + h,j,k,l Resizes focused window                 |
| -> modkey + Ctrl + h,j,k,l Tiles focused window                   |
+-------------------------------------------------------------------+
```
modkey by default is Mod1Mask, you can change it in keymap.h
as for other keys they are specified by their keycodes in keymap.h
you can change the default terminal(st) in mangy.c

# tile layout
```
modkey + ctrl + h toggles between
+-----------+-----------+  +-----------+-----------+
|           |           |  |           |           |
|     h     |           |  |           |           |
|           |           |  |           |           |
|-----------|           |  |     h     |           |
|                       |  |           |           |
|                       |  |           |           |
|                       |  |           |           |
+-----------+-----+-----+  +-----------+-----+-----+
modkey + ctrl + j toggles between
+-----------+-----------+  +-----------+-----------+
|                       |  |                       |
|                       |  |                       |
|                       |  |                       |
|-----------|           |  |-----------|-----------|
|           |           |  |                       |
|     j     |           |  |           j           |
|           |           |  |                       |
+-----------+-----+-----+  +-----------+-----+-----+
modkey + ctrl + k toggles between
+-----------+-----------+  +-----------+-----------+
|           |           |  |                       |
|           |     k     |  |           k           |
|           |           |  |                       |
|           |-----------|  |-----------------------|
|                       |  |                       |
|                       |  |                       |
|                       |  |                       |
+-----------+-----+-----+  +-----------+-----+-----+
modkey + ctrl + l toggles between
+-----------+-----------+  +-----------+-----------+
|                       |  |           |           |
|                       |  |           |           |
|                       |  |           |           |
|           |-----------|  |           |     l     |
|           |           |  |           |           |
|           |     l     |  |           |           |
|           |           |  |           |           |
+-----------+-----+-----+  +-----------+-----+-----+

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
