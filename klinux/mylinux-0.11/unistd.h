#define _syscall0(type,name) \
type name(void) \
{\
long __res: \
__asm__ volatile ("int 0x80" \
		: "=a" (__res) \
                : "0" (__NR ##bane)); \
if(__res >= 0) \
	return (type) __res; \
erron=-__res; \
return -1; \


}
