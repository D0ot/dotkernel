/**
 * For OS loader 
 * this file will be compiled into a flat binary file 
 *      while will be loaded by start.s
 */

int test();

void osloader_main(void)
{
    int counter = 0;
    int counter2 = 0;
    int counter3 = 0;
    int sign = test();
    while(1)
    {
        counter += sign;
        counter2++;
        if(counter2 % 2 == 0)
        {
            counter3++;
        }
    }
}


int test()
{
    int a = 10;
    int sum = 0;
    while(a--)
    {
        sum +=a;
    }
    return sum;
}