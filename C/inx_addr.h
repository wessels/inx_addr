
#ifndef INX_ADDR_H
#define INX_ADDR_H

typedef union {
	struct in6_addr in6;
	struct {
		struct in_addr pad0;
		struct in_addr pad1;
		struct in_addr pad2;
		struct in_addr in4;
	} _;
} inx_addr;

extern int _inx_addr_hash_bits;

extern int inx_addr_version(const inx_addr *);
extern const char * inx_addr_ntop(const inx_addr *, char *, socklen_t len);
extern int inx_addr_pton(const char *, inx_addr *);
extern unsigned int inx_addr_hash_bits(const inx_addr *, unsigned int bits);
extern unsigned int inx_addr_hash(const inx_addr *);
extern int inx_addr_cmp(const inx_addr *a, const inx_addr *b);
extern inx_addr inx_addr_mask (const inx_addr *a, const inx_addr *mask);

extern int inx_addr_assign_v4(inx_addr *, const struct in_addr *);
extern int inx_addr_assign_v6(inx_addr *, const struct in6_addr *);

#endif /* INX_ADDR_H */
