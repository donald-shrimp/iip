#include<stdio.h>
int main(){
    int d[2][2],j,k,num;
    double x,y;
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

    num=d[(int)(x+0.5)][(int)(y+0.5)];
    printf("＜内挿点の画素値＞\n%d\n",num);
}