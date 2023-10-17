#include <stdio.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include "keymap.h"
Display *disp;
Window root, fw, parent, *children;
XWindowAttributes attr;
XEvent event;
int childw, revert, sw, sy, snum, mfact = 0, chnotfw, mode = 1;
char *termin[]={"/usr/local/bin/st", NULL};
char *brows[]={"/usr/bin/chromium", NULL};
char *dmenu[] = { "/usr/local/bin/dmenu_run", NULL };
int tile(){
	if(mode == 1) XMoveResizeWindow(disp, fw, 0, 0, sw/2+mfact, sy);
	else if(mode == 2) XMoveResizeWindow(disp, fw, 0, sy/2-mfact, sw, sy/2+mfact);
	else if(mode == 3) XMoveResizeWindow(disp, fw, 0, 0, sw, sy/2-mfact);
	else if(mode == 4) XMoveResizeWindow(disp, fw, sw/2+mfact, 0, sw/2-mfact, sy);
	for(int y = 0; y < childw; y++){
		if(children[y] != fw){
			if(mode == 1) XMoveResizeWindow(disp, children[y], sw/2+mfact, chnotfw*(sy/(childw-1)), sw/2-mfact, sy/(childw-1));
			else if(mode == 2) XMoveResizeWindow(disp, children[y], chnotfw*(sw/(childw-1)), 0, sw/(childw-1), sy/2-mfact);
			else if(mode == 3) XMoveResizeWindow(disp, children[y], chnotfw*(sw/(childw-1)), sy/2-mfact, sw/(childw-1), sy/2+mfact);
			else if(mode == 4) XMoveResizeWindow(disp, children[y], 0, chnotfw*(sy/(childw-1)), sw/2+mfact, sy/(childw-1));
			chnotfw++;
		}
	}
	chnotfw = 0;
	return 0;
}
int main(){
	if(!(disp = XOpenDisplay(0x0))){puts("cant open display"); return 1;}
	int arr[]={escwm, killc, tg, fs, sf, term, br, dm, mvl, mvd, mvu, mvr};
	int mvspeed = 30, rspeed=30;
	root = DefaultRootWindow(disp);
	int len = sizeof(arr)/sizeof(arr[0]);
	for (int i = 0; i < len; i++){
		XGrabKey(disp, arr[i], mod1, root, True, GrabModeAsync, GrabModeAsync);
		if(i>len-5){
			XGrabKey(disp, arr[i], mod1 | ShiftMask, root, True, GrabModeAsync, GrabModeAsync);
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
				XGetInputFocus(disp, &fw, &revert);
				if(mode != 0) XMoveResizeWindow(disp, children[0], 0, 0, sw, sy);
			}
			else if(childw > 1){
				XGetInputFocus(disp, &fw, &revert);
				if(fw != None && mode != 0) tile();
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
			}else if(event.xkey.keycode == tg){
				if(mode < 4) mode++;
				else mode=0;
			}
			if(childw < 1) continue;
			if(fw != None){
				XGetWindowAttributes(disp, fw, &attr);
				switch(event.xkey.keycode){
					case killc:    
						XDestroyWindow(disp, fw);
						if(XQueryTree(disp, root, &root, &parent, &children, &childw)){
							if(childw >= 1)
								XSetInputFocus(disp, children[0], RevertToNone, CurrentTime);
							else
								XSetInputFocus(disp, root, RevertToNone, CurrentTime);
						}
						break;
					case mvl:
						if(event.xkey.state & ShiftMask)
							XMoveResizeWindow(disp, fw, attr.x, attr.y, attr.width-rspeed, attr.height);
						else
							if(mode == 0) XMoveResizeWindow(disp, fw, attr.x-mvspeed, attr.y, attr.width, attr.height);	 
							else mfact-=50;
						break;
					case mvd:
						if(event.xkey.state & ShiftMask)
							XMoveResizeWindow(disp, fw, attr.x, attr.y, attr.width, attr.height+rspeed);
						else
							XMoveResizeWindow(disp, fw, attr.x, attr.y+mvspeed, attr.width,attr.height);
						break;
					case mvu:
						if(event.xkey.state & ShiftMask)
							XMoveResizeWindow(disp, fw, attr.x, attr.y, attr.width, attr.height-rspeed);
						else
							XMoveResizeWindow(disp, fw, attr.x, attr.y-mvspeed, attr.width, attr.height);
						break;
					case mvr:
						if(event.xkey.state & ShiftMask)
							XMoveResizeWindow(disp, fw, attr.x, attr.y, attr.width+rspeed, attr.height);
						else
							if(mode == 0) XMoveResizeWindow(disp, fw, attr.x+mvspeed, attr.y, attr.width, attr.height);
							else mfact+=50;
						break;
					case fs:
						XMoveResizeWindow(disp, fw, 0, 0, sw, sy);
						break;
					case sf:
						for(int a = 0; a < childw; a++){
							if(children[a] == fw){
								if(a+1 < childw){
									XSetInputFocus(disp, children[a+1], RevertToNone, CurrentTime);
									XRaiseWindow(disp, children[a+1]);
								}
								else{
									XSetInputFocus(disp, children[0], RevertToNone, CurrentTime);
									XRaiseWindow(disp, children[0]);
								}
							}
						}
						break;
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
