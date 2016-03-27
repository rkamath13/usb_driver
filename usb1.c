#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/usb.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("rkamath@scu.edu");

/**
  * struct usb_driver - identifies USB interface driver to usbcore
  * @name: The driver name should be unique among USB drivers,
  *      and should normally be the same as the module name.
  * @probe: Called to see if the driver is willing to manage a particular
  *      interface on a device.  If it is, probe returns zero and uses
  *      usb_set_intfdata() to associate driver-specific data with the
  *      interface.  It may also use usb_set_interface() to specify the
  *      appropriate altsetting.  If unwilling to manage the interface,
  *      return -ENODEV, if genuine IO errors occurred, an appropriate
  *      negative errno value.
  * @disconnect: Called when the interface is no longer accessible, usually
  *      because its device has been (or is being) disconnected or the
  *      driver module is being unloaded.
  * @unlocked_ioctl: Used for drivers that want to talk to userspace through
  *      the "usbfs" filesystem.  This lets devices provide ways to
  *      expose information to user space regardless of where they
  *      do (or don't) show up otherwise in the filesystem.
  * @suspend: Called when the device is going to be suspended by the
  *      system either from system sleep or runtime suspend context. The
  *      return value will be ignored in system sleep context, so do NOT
  *      try to continue using the device if suspend fails in this case.
  *      Instead, let the resume or reset-resume routine recover from
  *      the failure.
  * @resume: Called when the device is being resumed by the system.
  * @reset_resume: Called when the suspended device has been reset instead
  *      of being resumed.
  * @pre_reset: Called by usb_reset_device() when the device is about to be
  *      reset.  This routine must not return until the driver has no active
  *      URBs for the device, and no more URBs may be submitted until the
  *      post_reset method is called.
  * @post_reset: Called by usb_reset_device() after the device
  *      has been reset
  * @id_table: USB drivers use ID table to support hotplugging.
  *      Export this with MODULE_DEVICE_TABLE(usb,...).  This must be set
  *      or your driver's probe function will never get called.
  * @dynids: used internally to hold the list of dynamically added device
  *      ids for this driver.
  * @drvwrap: Driver-model core structure wrapper.
  * @no_dynamic_id: if set to 1, the USB core will not allow dynamic ids to be
  *      added to this driver by preventing the sysfs file from being created.
  * @supports_autosuspend: if set to 0, the USB core will not allow autosuspend
  *      for interfaces bound to this driver.
  * @soft_unbind: if set to 1, the USB core will not kill URBs and disable
  *      endpoints before calling the driver's disconnect method.
  * @disable_hub_initiated_lpm: if set to 0, the USB core will not allow hubs
  *      to initiate lower power link state transitions when an idle timeout
  *      occurs.  Device-initiated USB 3.0 link PM will still be allowed.
  *
  * USB interface drivers must provide a name, probe() and disconnect()
  * methods, and an id_table.  Other driver fields are optional.
  *
  * The id_table is used in hotplugging.  It holds a set of descriptors,
  * and specialized data may be associated with each entry.  That table
  * is used by both user and kernel mode hotplugging support.
  *
  * The probe() and disconnect() methods are called in a context where
  * they can sleep, but they should avoid abusing the privilege.  Most
  * work to connect to a device should be done when the device is opened,
  * and undone at the last close.  The disconnect code needs to address
  * concurrency issues with respect to open() and close() methods, as
  * well as forcing all pending I/O requests to complete (by unlinking

 struct usb_driver {
         const char *name;
 
         int (*probe) (struct usb_interface *intf,
                       const struct usb_device_id *id);
 
         void (*disconnect) (struct usb_interface *intf);
 
         int (*unlocked_ioctl) (struct usb_interface *intf, unsigned int code,
                         void *buf);
 
         int (*suspend) (struct usb_interface *intf, pm_message_t message);
         int (*resume) (struct usb_interface *intf);
         int (*reset_resume)(struct usb_interface *intf);
 
         int (*pre_reset)(struct usb_interface *intf);
         int (*post_reset)(struct usb_interface *intf);
 
         const struct usb_device_id *id_table;
 
         struct usb_dynids dynids;
         struct usbdrv_wrap drvwrap;
         unsigned int no_dynamic_id:1;
         unsigned int supports_autosuspend:1;
         unsigned int disable_hub_initiated_lpm:1;
         unsigned int soft_unbind:1;
 };
*/

static int my_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	printk("Inside the function: %s\n", __FUNCTION__);
	printk("Pendrive (%04X:%04X) plugged in\n", id->idVendor, id->idProduct);
return 0;
}

static void pen_disconnect(struct usb_interface *interface)
{
	printk("Pendrive removed\n");
}

static struct usb_device_id my_table[]= {
	{USB_DEVICE(0X03f0, 0X5307)},
	{USB_DEVICE(0X0951, 0X1607)},
	{} //termination parenthesis
};
MODULE_DEVICE_TABLE (usb, my_table);


//usb driver
static struct usb_driver my_driver =
{
	.name = "Rameshkamath",
	.id_table = my_table,  //needed when we plug in a usb device
	.probe = my_probe,
	.disconnect = pen_disconnect,
};


static int __init enter_function(void)
{
	int ret =-1;
	printk("Driver for USb device by rkamath\n");
	printk("registering the USB device\n");
	ret = usb_register(&my_driver);
	printk("Driver registered\n");
	return ret;
return 0;
}

static void __init exit_function(void)
{
	printk("unregistering the USB device\n");
	usb_deregister(&my_driver);
	printk("unregisteration complete\n");
}

module_init(enter_function);
module_exit(exit_function);

