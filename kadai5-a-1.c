#include <stdio.h>
int main(){
    double r,g,b,y,cb,cr;
    printf("信号値を入力してください(実数値)\n");
    printf("R : ");
    scanf("%lf",&r);
    printf("G : ");
    scanf("%lf",&g);
    printf("B : ");
    scanf("%lf",&b);
    
    printf("\n<入力されたRGB信号(実数値)>\n");
    printf("R : %.4lf\n",r);
    printf("G : %.4lf\n",g);
    printf("B : %.4lf\n",b);

    y=0.2990*r+0.5870*g+0.1140*b;
    cb=(-0.1687)*r+(-0.3313)*g+(0.5)*b;
    cr=0.5*r+-0.4187*g+-0.0813*b;
    
    printf("\n<変換されたYCbCr信号(実数値)>\n");
    printf("Y : %.4lf\n",y);
    printf("Cb : %.4lf\n",cb);
    printf("Cr : %.4lf\n",cr);
    
    r=y+1.4020*cr;
    g=y+(-0.3441)*cb+(-0.7141)*cr;
    b=y+1.7720*cb;
    
    printf("\n<再度変換されたRGB信号(実数値)>\n");
    printf("R : %.4lf\n",r);
    printf("G : %.4lf\n",g);
    printf("B : %.4lf\n",b);
    
}