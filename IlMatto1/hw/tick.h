#ifndef TICK_H
#define TICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

#define TICK_CYCLE	1000

// Using timer 1
void tick_init(void);
uint16_t tick(void);

#ifdef __cplusplus
}
#endif

#endif
