#include <stdio.h>
#include <X11/Xlib.h>
#include "keymap.h"
Display *disp;
Window root, fw, parent, *children;
XWindowAttributes attr;
XEvent event;
int main(){
	if(!(disp = XOpenDisplay(0x0))){puts("cant open display"); return 1;}
	int arr[]={escwm, killc, mvl, mvd, mvu, mvr};
	int childw, revert, sw, sy, snum;
	root = DefaultRootWindow(disp);
	for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++){
		XGrabKey(disp, arr[i], mod1, root, True, GrabModeAsync, GrabModeAsync);
		XGrabKey(disp, arr[i], mod1 | ShiftMask, root, True, GrabModeAsync, GrabModeAsync);
		XGrabKey(disp, arr[i], mod1 | ControlMask, root, True, GrabModeAsync, GrabModeAsync);
	}
	XGrabButton(disp, 1, AnyModifier, root, True, ButtonPressMask, GrabModeAsync,GrabModeAsync, None, None);
	XSetInputFocus(disp, event.xbutton.subwindow, RevertToParent, CurrentTime);
	snum=DefaultScreen(disp);
	sw=DisplayWidth(disp, snum);
	sy=DisplayHeight(disp, snum);
	while(1){
		XNextEvent(disp, &event);
		if (XQueryTree(disp, root, &root, &parent, &children, &childw)) {
			if (childw == 1) {
				XSetInputFocus(disp, children[0], RevertToParent, CurrentTime);
			}
		}
		if(event.type == KeyPress){
			if(event.xkey.keycode == escwm){
				break;
			}
			XGetInputFocus(disp, &fw, &revert);
			if(fw != None){
				XGetWindowAttributes(disp, fw, &attr);
				switch(event.xkey.keycode){
					case killc:    
						XDestroyWindow(disp, fw);
						break;
					case mvl:
						if(event.xkey.state & ShiftMask)
							XMoveResizeWindow(disp, fw, attr.x, attr.y, attr.width-50, attr.height);
						else if(event.xkey.state & ControlMask){
							XMoveResizeWindow(disp, fw, 0, 0, sw/2, sy/2);	
						}
						else
							XMoveResizeWindow(disp, fw, attr.x-30, attr.y, attr.width, attr.height);	 
						break;
					case mvd:
						if(event.xkey.state & ShiftMask)
							XMoveResizeWindow(disp, fw, attr.x, attr.y, attr.width, attr.height+50);
						else if(event.xkey.state & ControlMask){
							XMoveResizeWindow(disp, fw, 0, sy/2, sw/2, sy/2);
						}
						else
							XMoveResizeWindow(disp, fw, attr.x, attr.y+30, attr.width,attr.height);
						break;
					case mvu:
						if(event.xkey.state & ShiftMask)
							XMoveResizeWindow(disp, fw, attr.x, attr.y, attr.width, attr.height-50);
						else if(event.xkey.state & ControlMask){
							XMoveResizeWindow(disp, fw, sw/2, 0, sw/2, sy/2);
						}
						else
							XMoveResizeWindow(disp, fw, attr.x, attr.y-30, attr.width, attr.height);
						break;
					case mvr:
						if(event.xkey.state & ShiftMask)
							XMoveResizeWindow(disp, fw, attr.x, attr.y, attr.width+50, attr.height);
						else if(event.xkey.state & ControlMask){
							XMoveResizeWindow(disp, fw, sw/2, sy/2, sw/2, sy/2);
						}
						else
							XMoveResizeWindow(disp, fw, attr.x+30, attr.y, attr.width, attr.height);
						break;
				}
			}
		}else if(event.type == ButtonPress && event.xbutton.subwindow != None){
			XRaiseWindow(disp, event.xbutton.subwindow);
			XSetInputFocus(disp, event.xbutton.subwindow, RevertToParent, CurrentTime);
		}
	}
	XCloseDisplay(disp);
	return 0;
}
