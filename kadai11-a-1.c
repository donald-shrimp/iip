#include<stdio.h>
int main(){
    int d[2][2],j,k;
    double x,y,num,r1,r0;
    printf("＜使用する４つの画素値＞\n");
    int plus=0;
    for(k=0;k<2;k++){
        for(j=0;j<2;j++){
            plus+=10;
            d[j][k]=plus;
            printf("%d ",d[j][k]);
        }
        printf("\n");
    }

    printf("内挿点の座標を入力してください\n");
    printf("水平(0.0以上1.0未満)：");
    scanf("%lf",&x);
    printf("垂直(0.0以上1.0未満)：");
    scanf("%lf",&y);

    r1=(1-x)*d[0][1]+x*d[1][1];
    r0=(1-x)*d[0][0]+x*d[1][0];
    num=(1-y)*r0+y*r1;
    printf("＜内挿点の画素値＞\n%.0lf\n",num);
}