#include <stdio.h>
int main() {
  int i, j, count, ycbrint[3], rgbint[3];
  double rgb[3],
      ycbr[3] = {0, 0, 0},
      RGB[3][3] = {{1, 0, 1.4020}, {1, -0.3441, -0.7141}, {1, 1.7720, 0}},
      YCBR[3][3] = {{0.299, 0.5870, 0.114},
                    {-0.1687, -0.3313, 0.5},
                    {0.5, -0.4187, -0.0813}};
  double data;
  double offset;

  printf("信号値を入力してください(整数値)\n");
  printf("R : ");
  scanf("%lf", &rgb[0]);
  printf("G : ");
  scanf("%lf", &rgb[1]);
  printf("B : ");
  scanf("%lf", &rgb[2]);

  printf("\n<入力されたRGB信号(整数値)>\n");
  printf("R : %d\n", (int)rgb[0]);
  printf("G : %d\n", (int)rgb[1]);
  printf("B : %d\n", (int)rgb[2]);

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      ycbr[i] += rgb[j] * YCBR[i][j];
    }
  }

  printf("\n<変換されたYCbCr信号(実数値)>\n");
  printf("Y : %.4lf\n", ycbr[0]);
  printf("Cb : %.4lf\n", ycbr[1]);
  printf("Cr : %.4lf\n", ycbr[2]);

  printf("\n<変換されたYCbCr信号(整数値)>\n");
  for (j = 0; j < 3; j++) {
    data = ycbr[j];

    if (data >= 0.0) {
      offset = 0.5;
    } else {
      offset = -0.5;
    }

    ycbrint[j] = (int)(data + offset);

    if (j != 0) {
      ycbrint[j] += 128.0;

      if (ycbrint[j] > 255.0) {
        ycbrint[j] = 255;
      } else if (ycbrint[j] < 0.0) {
        ycbrint[j] = 0.0;
      }
    }
  }
  printf("Y : %d\n", (int)ycbrint[0]);
  printf("Cb : %d\n", (int)ycbrint[1]);
  printf("Cr : %d\n", (int)ycbrint[2]);

  for (i = 0; i < 3; i++) {
    rgb[i] = 0;
    for (j = 0; j < 3; j++) {
        if (j==0) {
            rgb[i] += ycbrint[j] * RGB[i][j];
        }else{
            rgb[i] += (ycbrint[j]-128) * RGB[i][j];
        }
    }
  }

  printf("\n<再度変換されたRGB信号(実数値)>\n");
  printf("R : %.4lf\n", rgb[0]);
  printf("G : %.4lf\n", rgb[1]);
  printf("B : %.4lf\n", rgb[2]);

  printf("\n<再度変換されたRGB信号(整数値)>\n");
  for (j = 0; j < 3; j++) {
    data = rgb[j];

    if (data >= 0.0) {
      offset = 0.5;
    } else {
      offset = -0.5;
    }

    rgbint[j] = (int)(data + offset);

    if (rgb[j] > 255) {
      rgbint[j] = 255;
    } else if (rgb[j] < 0) {
      rgbint[j] = 0;
    }
  }
  printf("Y : %d\n", (int)rgbint[0]);
  printf("Cb : %d\n", (int)rgbint[1]);
  printf("Cr : %d\n", (int)rgbint[2]);
}