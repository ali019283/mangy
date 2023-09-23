#include <stdio.h>
#include <X11/Xlib.h>
#include "keymap.h"
Display *disp;
Window root;
XWindowAttributes attr;
XEvent event;
int mvrs(int x, int y, int mvx, int mvy){
    XGetWindowAttributes(disp, event.xkey.subwindow, &attr);
    XMoveResizeWindow(disp, event.xkey.subwindow, attr.x+x, attr.y+y, attr.width+mvx, attr.height+mvy);
}
int main(){
    if(!(disp = XOpenDisplay(0x0))){puts("cant open display"); return 1;}
    int arr[]={escwm, killc, mvl, mvd, mvu, mvr};
    root = DefaultRootWindow(disp);
    for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++){
        XGrabKey(disp, arr[i], mod1, root, True, GrabModeAsync, GrabModeAsync);
	XGrabKey(disp, arr[i], mod1 | ShiftMask, root, True, GrabModeAsync, GrabModeAsync);
        printf("%d", arr[i]);
    }
    XGrabButton(disp, 1, AnyModifier, root, True, ButtonPressMask, GrabModeAsync,GrabModeAsync, None, None);
    while(1){
        XNextEvent(disp, &event);
        if(event.type == KeyPress){
            if(event.xkey.keycode == escwm){
                break;
            }
            if(event.xkey.subwindow != None){
                switch(event.xkey.keycode){
                    case killc:    
                        XDestroyWindow(disp, event.xkey.subwindow);
                        break;
                    case mvl:
			if(event.xkey.state & ShiftMask)
				mvrs(0,0,-50,0);
			else 
				mvrs(-30,0,0,0);
                        break;
                    case mvd:
			if(event.xkey.state & ShiftMask)
				mvrs(0,0,0,50);
			else
				mvrs(0,30,0,0);
                        break;
                    case mvu:
			if(event.xkey.state & ShiftMask)
				mvrs(0,0,0,-50);
			else
                        	mvrs(0,-30,0,0);
                        break;
                    case mvr:
			if(event.xkey.state & ShiftMask)
				mvrs(0,0,50,0);
			else
                        	mvrs(30,0,0,0);
                        break;
                }
            }
        }else if(event.type == ButtonPress && event.xbutton.subwindow != None){XRaiseWindow(disp, event.xbutton.subwindow);}
    }
    XCloseDisplay(disp);
    return 0;
}
