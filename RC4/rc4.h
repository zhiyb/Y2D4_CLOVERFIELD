#ifndef RC4_H
#define RC4_H

#ifdef __cplusplus
extern "C" {
#endif

void rc4_regen(const char *key, const uint8_t length);
unsigned char rc4_output(void);
void rc4_codec(char *buff, const uint8_t length);

#ifdef __cplusplus
}
#endif

#endif
