/*
 * eagle-owl application.
 *
 * Copyright (C) 2012 Philippe Cornet <phil.cornet@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <stdio.h>
#include <usb.h>
#include "cm160.h"
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>
#include <string.h>

#define OWL_VENDOR_ID 0x0fde
#define CM160_DEV_ID  0xca05

extern struct cm160_device g_devices[MAX_DEVICES];

static int scan_device(struct usb_device *dev, int *dev_cnt)
{
  if(dev->descriptor.idVendor == OWL_VENDOR_ID && 
     dev->descriptor.idProduct == CM160_DEV_ID)
  {
    printf("Found compatible device #%d: %04x:%04x (%s/%s)\n",
           *dev_cnt, dev->descriptor.idVendor, 
           dev->descriptor.idProduct, dev->bus->dirname, dev->filename);
    g_devices[*dev_cnt].usb_dev = dev;
    (*dev_cnt)++;
    //reset device
    int fd;
    int rc;
    char filename[80];
    snprintf(filename, sizeof filename, "/dev/bus/usb/%s/%s", dev->bus->dirname, dev->filename);
    fd = open(filename, O_WRONLY);
    if (fd < 0) {
	    	perror("Error opening output file");
			return 1;
    }

    printf("Resetting USB device %s\n", filename);
    rc = ioctl(fd, USBDEVFS_RESET, 0);
    if (rc < 0) {
	    	perror("Error in ioctl");
			return 1;
    }
    printf("Reset successful\n");
    close(fd);
  }

  return 0;
}

int scan_usb()
{
  int dev_cnt = 0;

  usb_init();
  usb_find_busses();
  usb_find_devices();

  struct usb_bus *bus = NULL;
  struct usb_device *dev = NULL;
  for(bus = usb_get_busses(); bus; bus = bus->next)
  {
    for(dev = bus->devices; dev; dev=dev->next)
    {
      scan_device(dev, &dev_cnt);
      if(dev_cnt==MAX_DEVICES) return dev_cnt;
    }
  }

  return dev_cnt;
}

