

#include "inX_addr.h"
#include <string.h>

static unsigned char v4_in_v6_prefix[12] = {0,0,0,0,0,0,0,0,0,0,0xFF,0xFF};

struct in_addr&
INX_ADDR::v4()
{
	return theAddr._.in4;
}

struct in6_addr&
INX_ADDR::v6()
{
	return theAddr.in6;
}


int
INX_ADDR::is_v4_in_v6() const
{
    return (0 == memcmp(&theAddr, v4_in_v6_prefix, 12));
}

const char *
INX_ADDR::ntop(char *buf, socklen_t len) const
{
	const char *p;
	if (is_v4_in_v6())
		p = inet_ntop(AF_INET, &theAddr._.in4, buf, len);
	else
		p = inet_ntop(AF_INET6, &theAddr.in6, buf, len);
	if (p)
		return p;
	return "[unprintable]";
}

int
INX_ADDR::pton(const char *buf)
{
	if (strchr(buf, ':'))
		return inet_pton(AF_INET6, buf, &theAddr.in6);
	memcpy(&theAddr, v4_in_v6_prefix, 12);
	return inet_pton(AF_INET, buf, &theAddr._.in4);
}

unsigned int
INX_ADDR::hash(unsigned int bits) const
{
	static const uint32_t a = 2654435769U;	// closest prime to 2^32 * $goldenratio
	/*
	 * Fibonacci hashing 
	 * see http://www.brpreiss.com/books/opus4/html/page214.html
	 */
	return (a * (theAddr._.pad0.s_addr + theAddr._.pad1.s_addr + theAddr._.pad2.s_addr + theAddr._.in4.s_addr)) >> (32 - bits);
}

unsigned int
INX_ADDR::hash(const INX_ADDR& addr, unsigned int bits)
{
	return addr.hash(bits);
}

int
INX_ADDR::cmp(const INX_ADDR *a, const INX_ADDR *b)
{
        if (ntohl(a->theAddr._.in4.s_addr) < ntohl(b->theAddr._.in4.s_addr))
                return -1;
        if (ntohl(a->theAddr._.in4.s_addr) > ntohl(b->theAddr._.in4.s_addr))
                return 1;
	if (a->is_v4_in_v6())
		return 0;
        if (ntohl(a->theAddr._.pad2.s_addr) < ntohl(b->theAddr._.pad2.s_addr))
                return -1;
        if (ntohl(a->theAddr._.pad2.s_addr) > ntohl(b->theAddr._.pad2.s_addr))
                return 1;
        if (ntohl(a->theAddr._.pad1.s_addr) < ntohl(b->theAddr._.pad1.s_addr))
                return -1;
        if (ntohl(a->theAddr._.pad1.s_addr) > ntohl(b->theAddr._.pad1.s_addr))
                return 1;
        if (ntohl(a->theAddr._.pad0.s_addr) < ntohl(b->theAddr._.pad0.s_addr))
                return -1;
        if (ntohl(a->theAddr._.pad0.s_addr) > ntohl(b->theAddr._.pad0.s_addr))
                return 1;
	return 0;
}

INX_ADDR&
INX_ADDR::operator=(struct in_addr a4)
{
	memcpy(&theAddr, v4_in_v6_prefix, 12);
	theAddr._.in4 = a4;
	return *this;
}

INX_ADDR&
INX_ADDR::operator=(struct in6_addr a6)
{
	theAddr.in6 = a6;
	return *this;
}

bool
INX_ADDR::operator==(INX_ADDR that) const
{
	return 0 == cmp(this, &that);
}

bool
INX_ADDR::operator!=(INX_ADDR that) const
{
	return 0 != cmp(this, &that);
}

bool
INX_ADDR::operator<(INX_ADDR that) const
{
	return cmp(this, &that) < 0;
}

INX_ADDR
INX_ADDR::mask(const INX_ADDR& mask)
{
	INX_ADDR masked = *this;
	masked.theAddr._.in4.s_addr &= mask.theAddr._.in4.s_addr;
	if (!is_v4_in_v6()) {
		masked.theAddr._.pad2.s_addr &= mask.theAddr._.pad2.s_addr;
		masked.theAddr._.pad1.s_addr &= mask.theAddr._.pad1.s_addr;
		masked.theAddr._.pad0.s_addr &= mask.theAddr._.pad0.s_addr;
	}
	return masked;
}

int
INX_ADDR::version()
{
	if (!is_v4_in_v6())
		return 6;
	return 4;
}

INX_ADDR::INX_ADDR()
{
	memset(&theAddr, 0, sizeof(theAddr));
}

INX_ADDR::INX_ADDR(const char *p)
{
	pton(p);
}

INX_ADDR::INX_ADDR(const struct in_addr& a)
{
	memcpy(&theAddr, v4_in_v6_prefix, 12);
	theAddr._.in4.s_addr = a.s_addr;
}

INX_ADDR::INX_ADDR(const struct in6_addr& a)
{
	memset(&theAddr, 0, sizeof(theAddr));
	theAddr.in6 = a;
}

std::ostream&
operator<<(std::ostream& os, const INX_ADDR& addr)
{
	char buf[128];
	addr.ntop(buf, 128);
	return os << buf;
}
