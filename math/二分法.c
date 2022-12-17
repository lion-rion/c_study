//二分法を用いて√の解を得る

#include <stdio.h>
#include <math.h>

//求める値の範囲をa~bとする
//求める値をz = 5 - yのように入力する．√5の場合はz = 5 - yとなる

int main(void)
{
    double x, y, z;
    double num = 0;
    printf("求める値を入力√"); scanf("%lf", &num);
    double a = 0.0, b = 0.0; //√5は0.0から5.0の間にあるとする
    printf("値の範囲を入力 0.0~"); scanf("%lf", &b);
    int i;

    for (i = 0; i < 100; i++) {
        x = (a + b) / 2; //xはaとbの中点
        y = x * x; //yはxの2乗
        z = num - y; //zは5からyを引いたもの
        if (z > 0) {
            a = x;
        } else {
            b = x;
        }
        printf("√5 = %f (%d回目)\n", x, i);
    }
    return 0;
}