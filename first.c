#include <stdio.h>

int myFunction(int x) {
    
    int r;
    r = x*x + 2*x + 3;
    return r;
}

int main(int nargs, char** args){ 
    int x,y;
    x = atoi(args[1]);
    y = myFunction(x);
    printf("x=%d, y=%d\n",x,y);
    return 0;
}


