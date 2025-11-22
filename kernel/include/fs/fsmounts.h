#ifndef __FS_FSMOUNTS_H__
#define __FS_FSMOUNTS_H__

void fsmounts_init(void (*reqok) (s8 condition, string_t ok_str, string_t fail_str));

#endif