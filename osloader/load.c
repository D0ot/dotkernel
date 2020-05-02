/**
 * For OS loader 
 * this file will be compiled into a flat binary file 
 *      while will be loaded by start.s
 */

int test();

void osloader_main(void)
{
    int counter = 0;
    int sign = test();
    while(1)
    {
        counter++;
    }
}


int test()
{
    return 123;
}