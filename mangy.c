#include <stdio.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include "keymap.h"
Display *disp;
Window root, fw, parent, *children;
XWindowAttributes attr;
XEvent event;
int childw, revert, sw, sy, snum, chnotfw;

/* CONFIG */
int gaps = 10, mfact = 0, mode = 1;
char *termin[]={"/usr/local/bin/st", NULL};
char *brows[]={"/usr/bin/chromium", NULL};
char *dmenu[] = { "/usr/local/bin/dmenu_run", NULL };
/* CONFIG END */

int tile(){
	if(mode == 1) XMoveResizeWindow(disp, fw, gaps, gaps, sw/2+mfact-gaps, sy-(gaps*2));
	else if(mode == 2) XMoveResizeWindow(disp, fw, gaps, sy/2-mfact+gaps, sw-(gaps*2), sy/2+mfact-(gaps*2));
	else if(mode == 3) XMoveResizeWindow(disp, fw, gaps, gaps, sw-(gaps*2), sy/2-mfact-gaps);
	else if(mode == 4) XMoveResizeWindow(disp, fw, sw/2+mfact+gaps, gaps, sw/2-mfact-(gaps*2), sy-(gaps*2));
	else if (mode == 5){
		if(childw==2){
			XMoveResizeWindow(disp, children[0], 0, 0, sw/2+mfact, sy);
			XMoveResizeWindow(disp, children[1], sw/2, 0, sw/2, sy);
		}
		else{
			int toph=childw/2;
			int both=childw-toph;
			for (int i; i < childw; i++){
				if (i < toph){
					XMoveResizeWindow(disp, children[i], i*(sw/toph), 0, sw/(toph), sy/2);
				}else{
					XMoveResizeWindow(disp, children[i], (i-toph)*(sw/both), sy/2, sw/(both), sy/2);
				}
			}
		}
		return 0;
	}
	for(int y = 0; y < childw; y++){
		if(children[y] != fw){
			if(mode == 1){
				if(chnotfw==childw-2){
					XMoveResizeWindow(disp, children[y], sw/2+mfact+gaps, chnotfw*(sy/(childw-1))+gaps, sw/2-mfact-(gaps*2), sy/(childw-1)-(gaps*2));
				}else{
					XMoveResizeWindow(disp, children[y], sw/2+mfact+gaps, chnotfw*(sy/(childw-1))+gaps, sw/2-mfact-(gaps*2), sy/(childw-1)-gaps);
				}
			}
			else if(mode == 2){
				if(chnotfw==childw-2){
					XMoveResizeWindow(disp, children[y], chnotfw*(sw/(childw-1))+gaps, gaps, sw/(childw-1)-(gaps*2), sy/2-mfact-gaps);
				}else{
					XMoveResizeWindow(disp, children[y], chnotfw*(sw/(childw-1))+gaps, gaps, sw/(childw-1)-gaps, sy/2-mfact-gaps);
				}
			}
			else if(mode == 3){
				if(chnotfw==childw-2){
					XMoveResizeWindow(disp, children[y], chnotfw*(sw/(childw-1))+gaps, sy/2-mfact+gaps, sw/(childw-1)-(gaps*2), sy/2+mfact-(gaps*2));
				}else{
					XMoveResizeWindow(disp, children[y], chnotfw*(sw/(childw-1))+gaps, sy/2-mfact+gaps, sw/(childw-1)-gaps, sy/2+mfact-(gaps*2));
				}
			}
			else if(mode == 4){
				if(chnotfw==childw-2){
					XMoveResizeWindow(disp, children[y], gaps, chnotfw*(sy/(childw-1))+gaps, sw/2+mfact-gaps, sy/(childw-1)-(gaps*2));
				}else{
					XMoveResizeWindow(disp, children[y], gaps, chnotfw*(sy/(childw-1))+gaps, sw/2+mfact-gaps, sy/(childw-1)-gaps);
				}
			}
			chnotfw++;
		}
	}
	chnotfw = 0;
	return 0;
}
int main(){
	if(!(disp = XOpenDisplay(0x0))){puts("cant open display :("); return 1;}
	int arr[]={escwm, killc, tg, fs, rs, sf, term, br, dm, mvl, mvd, mvu, mvr};
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
	snum = DefaultScreen(disp);
	sw = DisplayWidth(disp, snum);
	sy = DisplayHeight(disp, snum);
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
				if(mode < 5) mode++;
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
					case rs:
						XRaiseWindow(disp, fw);
						break;
					case sf:
						for(int a = 0; a < childw; a++){
							if(children[a] == fw){
								if(a+1 < childw){
									XSetInputFocus(disp, children[a+1], RevertToNone, CurrentTime);
								}
								else{
									XSetInputFocus(disp, children[0], RevertToNone, CurrentTime);
								}
								break;
							}
						}
						break;
				}
			}
		}else if(event.type == ButtonPress && event.xbutton.subwindow != None){
			XRaiseWindow(disp, event.xbutton.subwindow);
			XSetInputFocus(disp, event.xbutton.subwindow, RevertToNone, CurrentTime);
		}
		//DOESNT WORK IDK WHYY
		/*else if(event.type == DestroyNotify){
			if(XQueryTree(disp, root, &root, &parent, &children, &childw)){
				if(childw >= 1)
					XSetInputFocus(disp, children[0], RevertToNone, CurrentTime);
				else
					XSetInputFocus(disp, root, RevertToNone, CurrentTime);
			}XGetInputFocus(disp, &fw, &revert);
		}*/
	}
	XCloseDisplay(disp);
	return 0;
}
