#include <stdio.h>
#include <string.h>

extern void dispstr(const char*,const int);

void puts_asm(const char* str){
    dispstr(str,strlen(str));
}

int main(){
    puts_asm("This is a message printed by asm.\n");
    puts("This is a message printed by C.");
    return 0;
}
