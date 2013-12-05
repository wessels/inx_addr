

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "inx_addr.h"

static unsigned char v4_in_v6_prefix[12] = {0,0,0,0,0,0,0,0,0,0,0xFF,0xFF};
int _inx_addr_hash_bits = 16;

static int
is_v4_in_v6(const struct in6_addr *addr)
{
    return (0 == memcmp(addr, v4_in_v6_prefix, 12));
}


const char *
inx_addr_ntop(const inx_addr *a, char *buf, socklen_t len)
{
	const char *p;
	if (!is_v4_in_v6(&a->in6))
		p = inet_ntop(AF_INET6, &a->in6, buf, len);
	else
	p =  inet_ntop(AF_INET, &a->_.in4, buf, len);
	if (p)
		return p;
	return "[unprintable]";
}

int
inx_addr_pton(const char *buf, inx_addr *a)
{
	if (strchr(buf, ':'))
		return inet_pton(AF_INET6, buf, a);
	memcpy(a, v4_in_v6_prefix, 12);
	return inet_pton(AF_INET, buf, &a->_.in4);
}

unsigned int
inx_addr_hash_bits(const inx_addr *a, unsigned int bits)
{
        static const uint32_t A = 2654435769U;  // closest prime to 2^32 * $goldenratio
	const uint32_t *x = (void*) a;
        /*
         * Fibonacci hashing 
         * see http://www.brpreiss.com/books/opus4/html/page214.html
         * Note ignores upper parts of IPv6
         */
        return (A * (x[0]+x[1]+x[2]+x[3])) >> (32 - bits);
}

unsigned int
inx_addr_hash(const inx_addr *a)
{
	return inx_addr_hash_bits(a, _inx_addr_hash_bits);
}

int
inx_addr_cmp(const inx_addr *a, const inx_addr *b)
{
        if (ntohl(a->_.in4.s_addr) < ntohl(b->_.in4.s_addr))
                return -1;
        if (ntohl(a->_.in4.s_addr) > ntohl(b->_.in4.s_addr))
                return 1;
	if (is_v4_in_v6(&a->in6))
		return 0;
        if (ntohl(a->_.pad2.s_addr) < ntohl(b->_.pad2.s_addr))
                return -1;
        if (ntohl(a->_.pad2.s_addr) > ntohl(b->_.pad2.s_addr))
                return 1;
        if (ntohl(a->_.pad1.s_addr) < ntohl(b->_.pad1.s_addr))
                return -1;
        if (ntohl(a->_.pad1.s_addr) > ntohl(b->_.pad1.s_addr))
                return 1;
        if (ntohl(a->_.pad0.s_addr) < ntohl(b->_.pad0.s_addr))
                return -1;
        if (ntohl(a->_.pad0.s_addr) > ntohl(b->_.pad0.s_addr))
                return 1;
	return 0;
}

inx_addr
inx_addr_mask (const inx_addr *a, const inx_addr *mask)
{
	inx_addr masked;
	masked._.in4.s_addr = a->_.in4.s_addr & mask->_.in4.s_addr;
	if (is_v4_in_v6(&a->in6)) {
		masked._.pad2.s_addr = a->_.pad2.s_addr;
		masked._.pad1.s_addr = a->_.pad1.s_addr;
		masked._.pad0.s_addr = a->_.pad0.s_addr;
	} else {
		masked._.pad2.s_addr = a->_.pad2.s_addr & mask->_.pad2.s_addr;
		masked._.pad1.s_addr = a->_.pad1.s_addr & mask->_.pad1.s_addr;
		masked._.pad0.s_addr = a->_.pad0.s_addr & mask->_.pad0.s_addr;
	}
	return masked;
}

int
inx_addr_version(const inx_addr *a)
{
	if (!is_v4_in_v6(&a->in6))
		return 6;
	return 4;
}

int
inx_addr_assign_v4(inx_addr *dst, const struct in_addr *src)
{
	memcpy(dst, v4_in_v6_prefix, 12);
	/* memcpy() instead of struct assignment in case src is not aligned */
	memcpy(&dst->_.in4, src, sizeof(*src));
	return 0;
}

int
inx_addr_assign_v6(inx_addr *dst, const struct in6_addr *src)
{
	/* memcpy() instead of struct assignment in case src is not aligned */
	memcpy(&dst->in6, src, sizeof(*src));
	return 0;
}
