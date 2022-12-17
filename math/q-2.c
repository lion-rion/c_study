#include <stdio.h>
#include <math.h>
int main(void)
{
    double x = 1.0; //初期値として1.0を入れた

    double y = 0.0;
    for (int i = 0; i < 5; i++)
    {
        y = (10 * (x * sin(x) + cos(x)) + 1) /( 1 + 10*sin(x)); //g(x)
        printf("x = %lf y = %lf\n", x,y);
        x = y;
    }
    return 0;
}
