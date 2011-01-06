#ifdef JEMALLOC_H_INLINES

#include <btypes.h>

#ifndef JEMALLOC_ENABLE_INLINE
t_u64	hash(const void *key, size_t len, t_u64 seed);
#endif

#if (defined(JEMALLOC_ENABLE_INLINE) || defined(HASH_C_))
/*
 * The following hash function is based on MurmurHash64A(), placed into the
 * public domain by Austin Appleby.  See http://murmurhash.googlepages.com/ for
 * details.
 */
JEMALLOC_INLINE t_u64
hash(const void *key, size_t len, t_u64 seed)
{
	const t_u64 m = 0xc6a4a7935bd1e995ull; /* ull for 32-bit */
	const int r = 47;
	t_u64 h = seed ^ (len * m);
	const t_u64 *data = (const t_u64 *)key;
	const t_u64 *end = data + (len/8);
	const t_u8 *data2;

	assert(((t_ptr)key & 0x7) == 0);

	while(data != end) {
		t_u64 k = *data++;

		k *= m;
		k ^= k >> r;
		k *= m;

		h ^= k;
		h *= m;
	}

	data2 = (const unsigned char *)data;
	switch(len & 7) {
		case 7: h ^= ((t_u64)(data2[6])) << 48;
		case 6: h ^= ((t_u64)(data2[5])) << 40;
		case 5: h ^= ((t_u64)(data2[4])) << 32;
		case 4: h ^= ((t_u64)(data2[3])) << 24;
		case 3: h ^= ((t_u64)(data2[2])) << 16;
		case 2: h ^= ((t_u64)(data2[1])) << 8;
		case 1: h ^= ((t_u64)(data2[0]));
			h *= m;
	}

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}
#endif

#endif /* JEMALLOC_H_INLINES */
