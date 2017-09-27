#ifndef USB_CDC_DEVICE_DEFINED
#define USB_CDC_DEVICE_DEFINED

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

void usb_init(void);
size_t usb_send(const uint8_t* buf, size_t size);

  
#ifdef __cplusplus
}
#endif
  
#endif //USB_CDC_DEVICE_DEFINED

