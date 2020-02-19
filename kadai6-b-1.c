#include <stdio.h>
#include <stdlib.h>

double ycbcr[3][3] = {{0.2990, 0.5870, 0.1140},
                      {-0.1687, -0.3313, 0.5000},
                      {0.5000, -0.4187, -0.0813}};
double rgb[3][3] = {{1.0000, 0.0000, 1.4020},
                    {1.0000, -0.3441, -0.7141},
                    {1.0000, 1.7720, 0.0000}};

double ycbcr_cnv[3][512][512] = {0}, do_ycbcr_cnv[3] = {0}, rgbio[3] = {0},
       rgb_cnv[3][512][512] = {0};
int int_ycbcr_cnv[3] = {0}, int_rgbio[3] = {0}, width, height, ins;

unsigned char header[54];
unsigned char imgin[3][512][512], imgout[3][512][512];

void rgb_to_ybr();
void ybr_to_rgb();
void get_data();
void put_data();
void processing();

int main() {
  get_data();
  rgb_to_ybr();
  processing();
  ybr_to_rgb();
  put_data();
  return 0;
}

void get_data() {
  char fname[20];
  FILE *fp;
  int i;

  // ファイル名の入力
  printf("入力ファイル名を入力してください：");
  scanf("%s", fname);

  // ファイルオープン
  fp = fopen(fname, "rb");

  if (fp == NULL) {
    printf("ファイルをオープンできませんでした\n");
    exit(1);
  }

  printf("ファイルをオープンしました.\n");

  // ヘッダー情報の取得
  for (int cnt = 0; cnt < 54; cnt++) {
    header[cnt] = fgetc(fp);
  }

  printf("\n<ファイルサイズ>\n");
  for (i = 2; i < 6; i++) {
    printf("header[%d] = %02x ", i, header[i]);
  }
  int filesize = *(int *)&header[2];
  printf("\n%dバイト\n", filesize);

  printf("\n<オフセット>\n");
  for (i = 10; i < 13; i++) {
    printf("header[%d] = %02x ", i, header[i]);
  }
  int offset = *(int *)&header[10];
  printf("\n%dバイト\n", offset);

  printf("\n<画像の幅>\n");
  for (i = 18; i < 22; i++) {
    printf("header[%d] = %02x ", i, header[i]);
  }
  width = *(int *)&header[18];
  printf("\n%d画素\n", width);

  printf("\n<画像の高さ>\n");
  for (i = 22; i < 26; i++) {
    printf("header[%d] = %02x ", i, header[i]);
  }
  height = *(int *)&header[22];
  printf("\n%dライン\n", height);

  printf("\n<1画素あたりのビット数>\n");
  for (i = 28; i < 30; i++) {
    printf("header[%d] = %02x ", i, header[i]);
  }
  int bit = *(int *)&header[28];
  printf("\n%oビット\n", *(short *)&header[28]);

  printf("\n<挿入ビット数>\n");
  ins = filesize - offset - width * height * (bit / 8);
  printf("%dバイト\n\n", ins);

  for (i = height - 1; i >= 0; i--) {
    for (int j = 0; j < width; j++) {
      for (int k = 2; k >= 0; k--) {
        imgin[k][j][i] = fgetc(fp);
      }
    }
  }

  // ファイルクローズ
  fclose(fp);
}

void rgb_to_ybr() {
  // 入力値出力
  printf("<入力信号(RGB)>\n");
  printf("---R---\n");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("%.2X ", imgin[0][j][i]);
    }
    putchar('\n');
  }
  printf("---G---\n");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("%.2X ", imgin[1][j][i]);
    }
    putchar('\n');
  }
  printf("---B---\n");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("%.2X ", imgin[2][j][i]);
    }
    putchar('\n');
  }

  // YCbCrへの変換(整数)
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      for (int k = 0; k < 3; k++) {
        ycbcr_cnv[0][i][j] += ycbcr[0][k] * imgin[k][i][j];
        ycbcr_cnv[1][i][j] += ycbcr[1][k] * imgin[k][i][j];
        ycbcr_cnv[2][i][j] += ycbcr[2][k] * imgin[k][i][j];
      }
    }
  }

  // 四捨五入
  for (int k = 0; k < 3; k++) {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (ycbcr_cnv[k][i][j] >= 0) {
          ycbcr_cnv[k][i][j] += 0.5;
          ycbcr_cnv[k][i][j] = (int)ycbcr_cnv[k][i][j];
        } else if (ycbcr_cnv[k][i][j] < 0) {
          ycbcr_cnv[k][i][j] -= 0.5;
          ycbcr_cnv[k][i][j] = (int)ycbcr_cnv[k][i][j];
        }
      }
    }
  }

  // CbとCrにオフセット加算
  for (int k = 1; k < 3; k++) {
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        ycbcr_cnv[k][j][i] += 128;
      }
    }
  }

  // 1バイト外数値時修正処理
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < height; j++) {
      for (int k = 0; k < width; k++) {
        if (ycbcr_cnv[i][k][j] >= 255) {
          ycbcr_cnv[i][k][j] = 255;
        } else if (ycbcr_cnv[i][k][j] <= 0) {
          ycbcr_cnv[i][k][j] = 0;
        }
      }
    }
  }

  // imginへの上書き処理
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < height; j++) {
      for (int k = 0; k < width; k++) {
        imgin[i][k][j] = (int)ycbcr_cnv[i][k][j];
      }
    }
  }

  // 出力
  printf("<入力信号(YCbCr)>\n");
  for (int i = 0; i < 3; i++) {
    switch (i) {
      case 0: {
        printf("--- Y ---\n");
        break;
      }
      case 1: {
        printf("--- Cb ---\n");
        break;
      }
      case 2: {
        printf("--- Cr ---\n");
        break;
      }
    }
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        printf("%.2X ", imgin[i][k][j]);
      }
      putchar('\n');
    }
  }
  putchar('\n');
}

void processing() {
  int i, j, k,y,cb,cr;
  printf("コピーモードを入力して下さい．\n(コピーする場合：1，固定値に置き換える場合：0)\n");
printf("Y  :");
scanf("%d",&y);
printf("Cb :");
scanf("%d",&cb);
printf("Cr :");
scanf("%d",&cr);
  // imgin 2 imgout
  for (i = 0; i < 3; i++) {
    for (j = 0; j < height; j++) {
      for (k = 0; k < width; k++) {
        imgout[i][k][j] = imgin[i][k][j];
        if(y==0&&i==0){
            imgout[i][k][j] = 0;
      }
    }
  }
  printf("\n入力画像データをコピーして出力画像データを作成しました.\n");
}

void ybr_to_rgb() {
  // RGB変換(整数)
  printf("<出力信号(YCbCr)>\n");
  printf("--- Y ---\n");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("%.2X ", imgin[0][j][i]);
    }
    putchar('\n');
  }
  printf("--- Cb ---\n");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("%.2X ", imgin[1][j][i]);
    }
    putchar('\n');
  }
  printf("--- Cr ---\n");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      printf("%.2X ", imgin[2][j][i]);
    }
    putchar('\n');
  }
  putchar('\n');

  // オフセット加算値減算
  short tmp[3][512][512] = {0};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < height; j++) {
      for (int k = 0; k < width; k++) {
        tmp[i][k][j] = imgout[i][k][j];
      }
    }
  }
  for (int i = 1; i < 3; i++) {
    for (int j = 0; j < height; j++) {
      for (int k = 0; k < width; k++) {
        tmp[i][k][j] = tmp[i][k][j] - 128.0;
      }
    }
  }

  // RGB変換
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      for (int k = 0; k < 3; k++) {
        rgb_cnv[0][i][j] += rgb[0][k] * tmp[k][i][j];
        rgb_cnv[1][i][j] += rgb[1][k] * tmp[k][i][j];
        rgb_cnv[2][i][j] += rgb[2][k] * tmp[k][i][j];
      }
    }
  }

  // 四捨五入
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < height; j++) {
      for (int k = 0; k < width; k++) {
        if (rgb_cnv[i][k][j] >= 0) {
          rgb_cnv[i][k][j] += 0.5;
        } else if (rgb_cnv[i][k][j] < 0) {
          rgb_cnv[i][k][j] -= 0.5;
        }
        rgb_cnv[i][k][j] = (int)rgb_cnv[i][k][j];
      }
    }
  }

  // 1バイト範囲外値修正処理
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < height; j++) {
      for (int k = 0; k < width; k++) {
        if (rgb_cnv[i][k][j] > 255) {
          rgb_cnv[i][k][j] = 255;
        } else if (rgb_cnv[i][k][j] < 0) {
          rgb_cnv[i][k][j] = 0;
        }
      }
    }
  }

  // rgb_cnv 2 imgout
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < height; j++) {
      for (int k = 0; k < width; k++) {
        imgout[i][k][j] = rgb_cnv[i][k][j];
      }
    }
  }

  // 出力
  printf("<出力信号(RGB)>\n");
  for (int i = 0; i < 3; i++) {
    switch (i) {
      case 0: {
        printf("--- R ---\n");
        break;
      }
      case 1: {
        printf("--- G ---\n");
        break;
      }
      case 2: {
        printf("--- B ---\n");
        break;
      }
    }
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        printf("%.2X ", imgout[i][k][j]);
      }
      putchar('\n');
    }
  }
}

void put_data() {
  char n_fname[20];
  FILE *fp;
  printf("出力画像データを作成しました.\n\n");
  printf("ファイル名を入力:");
  scanf("%s", n_fname);
  fp = fopen(n_fname, "wb");
  if (fp == NULL) {
    printf("ファイルをオープンできません\n");
    exit(1);
  }
  printf("ファイルをオープンしました\n");
  for (int i = 0; i < 54; i++) {
    fputc(header[i], fp);
  }
  for (int i = height - 1; i >= 0; i--) {
    for (int j = 0; j < width; j++) {
      for (int k = 2; k >= 0; k--) {
        fputc(imgout[k][j][i], fp);
      }
    }
  }
  fclose(fp);
  printf("ファイルをクローズしました\n");
}