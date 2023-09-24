#include <stdio.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include "keymap.h"
Display *disp;
Window root, fw, parent, *children;
XWindowAttributes attr;
XEvent event;
char *termin[]={"/usr/local/bin/st", NULL};
char *brows[]={"/usr/bin/chromium", NULL};
char *dmenu[] = { "/usr/local/bin/dmenu_run", NULL };
int main(){
	if(!(disp = XOpenDisplay(0x0))){puts("cant open display"); return 1;}
	int arr[]={escwm, killc, fs, term, br, dm, mvl, mvd, mvu, mvr};
	int mvspeed = 30, rspeed=30;
	int childw, revert, sw, sy, snum;
	root = DefaultRootWindow(disp);
	int len = sizeof(arr)/sizeof(arr[0]);
	for (int i = 0; i < len; i++){
		XGrabKey(disp, arr[i], mod1, root, True, GrabModeAsync, GrabModeAsync);
		if(i>len-5){
			XGrabKey(disp, arr[i], mod1 | ShiftMask, root, True, GrabModeAsync, GrabModeAsync);
			XGrabKey(disp, arr[i], mod1 | ControlMask, root, True, GrabModeAsync, GrabModeAsync);
		}
	}
	XGrabButton(disp, 1, mod1, root, True, ButtonPressMask, GrabModeAsync,GrabModeAsync, None, None);
	snum=DefaultScreen(disp);
	sw=DisplayWidth(disp, snum);
	sy=DisplayHeight(disp, snum);
	while(1){
		XNextEvent(disp, &event);
		if (XQueryTree(disp, root, &root, &parent, &children, &childw)) {
			if (childw == 1) {
				XSetInputFocus(disp, children[0], RevertToNone, CurrentTime);
			}
		}
		if(event.type == KeyPress){
			if(event.xkey.keycode == escwm){
				break;
			}
			else if(event.xkey.keycode == term){
				pid_t pid; 
				pid = fork(); 
				if(pid == 0) execv(termin[0], termin);
			}
			else if(event.xkey.keycode == br){
				pid_t pid;
				pid = fork();
				if(pid == 0) execv(brows[0], brows);
			}
			else if(event.xkey.keycode == dm){
				pid_t pid;
				pid = fork();
				if(pid == 0) execv(dmenu[0], dmenu);
			}
			if(childw < 1) continue;
			XGetInputFocus(disp, &fw, &revert);
			if(fw != None){
				XGetWindowAttributes(disp, fw, &attr);
				switch(event.xkey.keycode){
					case killc:    
						XDestroyWindow(disp, fw);
						XSetInputFocus(disp, root, RevertToNone, CurrentTime);
						break;
					case mvl:
						if(event.xkey.state & ShiftMask)
							XMoveResizeWindow(disp, fw, attr.x, attr.y, attr.width-rspeed, attr.height);
						else if(event.xkey.state & ControlMask){
							if(!attr.x && !attr.y && attr.width == sw/2 && attr.height == sy/2)
								XMoveResizeWindow(disp, fw, 0, 0, sw/2, sy);
							else
								XMoveResizeWindow(disp, fw, 0, 0, sw/2, sy/2);
						}	
						else
							XMoveResizeWindow(disp, fw, attr.x-mvspeed, attr.y, attr.width, attr.height);	 
						break;
					case mvd:
						if(event.xkey.state & ShiftMask)
							XMoveResizeWindow(disp, fw, attr.x, attr.y, attr.width, attr.height+rspeed);
						else if(event.xkey.state & ControlMask){
							if(!attr.x && attr.y == sy/2 && attr.width == sw/2 && attr.height == sy/2)
								XMoveResizeWindow(disp, fw, 0, sy/2, sw, sy/2);
							else
								XMoveResizeWindow(disp, fw, 0, sy/2, sw/2, sy/2);
						}
						else
							XMoveResizeWindow(disp, fw, attr.x, attr.y+mvspeed, attr.width,attr.height);
						break;
					case mvu:
						if(event.xkey.state & ShiftMask)
							XMoveResizeWindow(disp, fw, attr.x, attr.y, attr.width, attr.height-rspeed);
						else if(event.xkey.state & ControlMask){
							if(attr.x == sw/2 && !attr.y && attr.width == sw/2 && attr.height == sy/2)
								XMoveResizeWindow(disp, fw, 0, 0, sw, sy/2);
							else
								XMoveResizeWindow(disp, fw, sw/2, 0, sw/2, sy/2);
						}
						else
							XMoveResizeWindow(disp, fw, attr.x, attr.y-mvspeed, attr.width, attr.height);
						break;
					case mvr:
						if(event.xkey.state & ShiftMask)
							XMoveResizeWindow(disp, fw, attr.x, attr.y, attr.width+rspeed, attr.height);
						else if(event.xkey.state & ControlMask){
							if(attr.x == sw/2 && attr.y == sy/2 && attr.width == sw/2 && attr.height == sy/2)
								XMoveResizeWindow(disp, fw, sw/2, 0, sw/2, sy);
							else
								XMoveResizeWindow(disp, fw, sw/2, sy/2, sw/2, sy/2);
						}
						else
							XMoveResizeWindow(disp, fw, attr.x+mvspeed, attr.y, attr.width, attr.height);
						break;
					case fs:
						XMoveResizeWindow(disp, fw, 0, 0, sw, sy);
				}
			}
		}else if(event.type == ButtonPress && event.xbutton.subwindow != None){
			XRaiseWindow(disp, event.xbutton.subwindow);
			XSetInputFocus(disp, event.xbutton.subwindow, RevertToNone, CurrentTime);
		}
	}
	XCloseDisplay(disp);
	return 0;
}
