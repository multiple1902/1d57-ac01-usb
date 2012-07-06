// Copyright 2012 by Weisi Dai <multiple1902@gmail.com>
// http://github.com/multiple1902/1d57-ac01-usb
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//    
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the
// Free Software Foundation, Inc.,
// 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.

#include <stdio.h>
#include <string.h> // memset
#include <libusb-1.0/libusb.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

uint16_t HID_VENDOR = 0x1d57;
uint16_t HID_PRODUCT = 0xac01;
uint16_t PAGEUP = 0xff55;
uint16_t PAGEDOWN = 0xff56;

const unsigned char ENDPOINT_DOWN = 0x01;
const unsigned char ENDPOINT_UP = 0x81;

struct libusb_device_handle *devh = NULL;
Display *display;

int find_hid_device(void){
    devh = libusb_open_device_with_vid_pid(NULL, HID_VENDOR, HID_PRODUCT);
    return devh ? 0 : 1;
}

void send_key(unsigned int key){
    unsigned int keycode;
    keycode = XKeysymToKeycode(display, key);
    XTestFakeKeyEvent(display, keycode, True, 0);
    XTestFakeKeyEvent(display, keycode, False, 0);
    XFlush(display);
}

int main(){
    int r = 1;
    r = libusb_init(NULL);
    if (r < 0) {
        fprintf(stderr, "Error initializing libusb. \n");
        return r;
    }

    if (find_hid_device()) {
        fprintf(stderr, "Device not found. \n");
        return -1;
    }

    libusb_detach_kernel_driver(devh, 0);

    r = libusb_set_configuration(devh, 1);
    r = libusb_claim_interface(devh, 0);

    if (r) {
        fprintf(stderr, "Failed to claim interface. \n");
        return r;
    }

    unsigned char data[10];
    int actual = 0;
    display = XOpenDisplay(NULL);

    do{
        memset(data, 0, sizeof(data));
        r = libusb_bulk_transfer(devh, ENDPOINT_UP, data, 8, &actual, 0);

        switch(data[2]){ // Tweak values here if needed
            case 78:
                fprintf(stderr, "PgUp\n");
                send_key(0xff55);
                break;

            case 75:
                fprintf(stderr, "PgDn\n");
                send_key(0xff56);
                break;
        }

    } while (r != LIBUSB_ERROR_NO_DEVICE);

    r = libusb_release_interface(devh, 0);
    libusb_close(devh);

    return 0;
}
