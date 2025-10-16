#ifndef __DRIVER_H__
#define __DRIVER_H__

typedef struct driver_t {
  void (* driver_init) ();
  void (* driver_shutdown) ();
} driver_t;

#endif