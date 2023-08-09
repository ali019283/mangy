#include <stdio.h>
#include <X11/Xlib.h>
#define modkey Mod4Mask
Display *disp;
XWindowAttributes attr;
XEvent event;
int mvrs(int x, int y, int mvx, int mvy){
    XGetWindowAttributes(disp, event.xkey.subwindow, &attr);
    XMoveResizeWindow(disp, event.xkey.subwindow, attr.x+x, attr.y+y, attr.width+mvx, attr.height+mvy);
}
int main(){
    if(!(disp = XOpenDisplay(0x0))){puts("cant open display"); return 1;}
    int arr[]={9, 24, 43, 44, 45, 46, 111, 113, 114, 116};
    for (int i = 0; i <= 9; i++){
        XGrabKey(disp, arr[i], modkey, DefaultRootWindow(disp), True, GrabModeAsync, GrabModeAsync);
        printf("%d", arr[i]);
    }
    while(1){
        XNextEvent(disp, &event);
        if(event.type == KeyPress){
            if(event.xkey.keycode == 0x09){
                break;
            }
            if(event.xkey.subwindow != None){
                switch(event.xkey.keycode){
                    case 24:    
                        XDestroyWindow(disp, event.xkey.subwindow);
                        break;
                    case 43:
                        mvrs(-30,0,0,0);
                        break;
                    case 44:
                        mvrs(0,30,0,0);
                        break;
                    case 45:
                        mvrs(0,-30,0,0);
                        break;
                    case 46:
                        mvrs(30,0,0,0);
                        break;
                    case 114:
                        mvrs(0,0,50,0);
                        break;
                    case 111:
                        mvrs(0,0,0,-50);
                        break;
                    case 113:
                        mvrs(0,0,-50,0);
                        break;
                    case 116:
                        mvrs(0,0,0,50);
                        break;

                }
            }
        }
    }
    XCloseDisplay(disp);
    return 0;
}