#include <stdint.h>
//#define LOCAL_RUN 
#ifndef LOCAL_RUN
#include "services.h"
#else
#include <stdio.h>
void printInt(int x){
    printf("%d - %x\n", x, x);
}
#endif

/*
 * The Dan Bernstein popuralized hash..  See
 * https://github.com/pjps/ndjbdns/blob/master/cdb_hash.c#L26 Due to hash
 * collisions it seems to be replaced with "siphash" in n-djbdns, see
 * https://github.com/pjps/ndjbdns/commit/16cb625eccbd68045737729792f09b4945a4b508
 */
uint32_t djb33_hash(const char* s, int len)
{
    uint32_t h = 5381;
    while (len--) {
        /* h = 33 * h ^ s[i]; */
        h += (h << 5);  
        h ^= *s++;
    }
    return h;
}


/*
 *
 * The Java hash, but really no-one seems to know where it came from, see
 * https://bugs.java.com/bugdatabase/view_bug.do?bug_id=4045622
 */
/*
uint32_t h31_hash(const char* s, int len)
{
    uint32_t h = 0;
    while (len) {
        h = 31 * h + *s++;
        --len;
    }
    return h;
}
*/
/*
 * The FNV Hash, or more precisely the "FNV-1a alternate algorithm"
 * See: http://www.isthe.com/chongo/tech/comp/fnv/
 *      https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
 */
/*
uint32_t fnv32_hash(const char *str, int len)
{
    unsigned char *s = (unsigned char *)str;	


    const uint32_t FNV_32_PRIME = 0x01000193; 

    uint32_t h = 0x811c9dc5; 
    while (len--) {
        h ^= *s++;
        h *= FNV_32_PRIME;
    }

    return h;
}

*/
/*
 * "This came from ejb's hsearch."
 */
/*
uint32_t ejb_hash(const char *s, int len)
{
	unsigned char *key = (unsigned char*) s;
	const uint32_t PRIME1 = 37;
	const uint32_t PRIME2 = 1048583;
	uint32_t h = 0;

	while (len--) {
		h = h * PRIME1 ^ (*key++ - ' ');
	}
	h %= PRIME2;

	return h;
}
*/
/*
 * Bob Jenkins "One-at-a-time" hash
 */
uint32_t oat_hash(const char *s, int len)
{
    unsigned char *p = (unsigned char*) s;
    uint32_t h = 0;

    while(len--) {
        h += *p++;
        h += (h << 10);
        h ^= (h >> 6);
    }

    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15);

    return h;
}

int strlen(char *s){
    int i=0;
    for(; s[i]!='\0'; i++);
    return i;
}
char *str = "The quick brown fox jumps over the lazy dog";

main(){

    uint32_t h0 = djb33_hash(str, strlen(str));
    printInt(h0);

    //uint32_t h1 = h31_hash(str, strlen(str));
    //printInt(h1);
    
    uint32_t h2 = oat_hash(str, strlen(str));
    printInt(h2);
    
    return 1;
}