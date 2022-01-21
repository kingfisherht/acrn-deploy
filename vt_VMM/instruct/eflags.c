#include <stdio.h>
 
#include <stdint.h>
 
#define LAHF() \
({ \
        uint16_t dst;\
        \
        \
        \
        asm("lahf;"     \
        :"=a"(dst)      \
        :       \
        );      \
        dst;\
})
 
#define READ_RFLAGS_LOW16() \
({ \
        uint16_t dst;\
        \
        \
        \
        asm("pushfw; popw %%ax" \
        :"=a"(dst)      \
        :       \
        );      \
        dst;\
}) 
 
 
int main(int argc, char *argv[])
{
        uint16_t res;
 
        res = LAHF(); // read the low 8 bit
 
        res = res >> 8; //read the low 8 bit
 
        printf("BSWAP->res : 0x%X\n", res);

        READ_RFLAGS_LOW16() // read low16 bit
        


}
