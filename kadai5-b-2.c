#include <stdio.h>
#include <stdlib.h>
void rgb_to_ybr(void);
void processing(void);
void ybr_to_rgb(void);
void get_data(void);
void put_data(void);
unsigned char header[54];
unsigned char imgin[3][512][512] = {};
unsigned char imgout[3][512][512] = {};
unsigned char ycbr[3][512][512] = {}, rgb[3][512][512] = {};
int offset, width, height, bits;

int main(void) {
  get_data();
  rgb_to_ybr();
  processing();
  ybr_to_rgb();
  put_data();
  return 0;
}

void processing(void) {
  int x, y, z;
  //配列にコピーするよ

  for (x = 511; x >= 0; x--) {
    for (y = 0; y < 512; y++) {
      for (z = 2; z >= 0; z--) {
        imgout[z][x][y] = imgin[z][x][y];
      }
    }
  }
  printf("\n入力画像データをコピーして出力画像データを作成しました.\n");
}

void get_data(void) {
  FILE *fp;
  int count = 0, i = 0, x = 0, y = 0, z = 0;
  char filename[100];
  int filesize;
  /*---ファイル・オープン(読み込み用) ---*/
  printf("ファイル名を入力してください：");
  scanf("%s", filename);
  fp = fopen(filename, "rb");
  if (fp == NULL) {
    printf("ファイルをオープンできません．\n");
    exit(1);
  }
  printf("ファイルをオープンしました．\n");

  /*---ファイルから1バイトずつ読み込む---*/

  while (count < 54) {
    header[count] = fgetc(fp);
    count++;
  }

  //結果を出力するよ
  printf("\n＜ファイルタイプ＞\n");
  while (i < 2) {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  printf("\n");

  printf("\n＜ファイルサイズ＞\n");
  while (i < 6) {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  filesize = *(int *)&header[2];
  offset = *(int *)&header[10];
  printf("\n%dバイト\n", filesize);

  printf("\n＜画像の幅＞\n");
  while (i < 22) {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  width = *(int *)&header[18];
  printf("\n%d画素\n", width);

  printf("\n＜画像の高さ＞\n");
  while (i < 26) {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  height = *(int *)&header[22];
  printf("\n%dライン\n", height);

  printf("\n＜１画素当たりのビット数＞\n");
  while (i < 30) {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  bits = *(int *)&header[28];
  printf("\n%dビット\n", bits);
  printf("\n＜挿入ビット数＞\n");

  printf("%dバイト\n", filesize - offset - width * height * (bits / 8));

  for (x = width - 1; x >= 0; x--) {
    for (y = 0; y < height; y++) {
      for (z = 2; z >= 0; z--) {
        imgin[z][x][y] = fgetc(fp);
      }
    }
  }

  /*--- ファイル・クローズ(読み込み用) ---*/
  fclose(fp);
  printf("ファイルをクローズしました．\n");
}

void rgb_to_ybr(void) {
  int j, i, x, y, z, count;
  double YCBR[3][3] = {
      {0.299, 0.5870, 0.114}, {-0.1687, -0.3313, 0.5}, {0.5, -0.4187, -0.0813}};
  double data;
  double offset;

  printf("\n＜入力信号(RGB)＞\n\n");

  // RGBを表示するよ
  printf("---R---\n");
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      printf("%02X ", imgin[0][x][y]);
    }
    printf("\n");
  }
  printf("---G---\n");
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      printf("%02X ", imgin[1][x][y]);
    }
    printf("\n");
  }
  printf("---B---\n");
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      printf("%02X ", imgin[2][x][y]);
    }
    printf("\n");
  }

  // RGBをYCbCrに変換するよ（バグってるかも）
  for (int x = 0; x < height; x++) {
    for (int y = 0; y < width; y++) {
      for (int z = 0; z < 3; z++) {
        ycbr[0][x][y] += YCBR[0][z] * imgin[z][x][y];
        ycbr[1][x][y] += YCBR[1][z] * imgin[z][x][y];
        ycbr[2][x][y] += YCBR[2][z] * imgin[z][x][y];
      }
    }
  }

  // 四捨五入してCbCrに128を足して1バイトのなんちゃら処理をするよ
  for (z = 0; z < 3; z++) {
    for (x = 0; x < height; x++) {
      for (y = 0; y < width; y++) {
        data = ycbr[z][x][y];

        if (data >= 0.0) {
          offset = 0.5;
        } else {
          offset = -0.5;
        }

        ycbr[z][x][y] = (int)(data + offset);

        if (z != 0) {
          ycbr[z][x][y] += 128;
          if (ycbr[z][x][y] > 255) {
            ycbr[z][x][y] = 255;
          } else if (ycbr[z][x][y] < 0) {
            ycbr[z][x][y] = 0;
          }
        }
      }
    }
  }
  // imginに結果を挿入するよ
  for (z = 0; z < 3; z++) {
    for (y = 0; y < width; y++) {
      for (x = 0; x < height; x++) {
        imgin[z][x][y] = (int)ycbr[z][x][y];
      }
    }
  }

  printf("\n＜入力信号(YCbCr)＞\n\n");

  // YCbCrを表示するよ
  printf("---Y---\n");
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      printf("%02X ", imgin[0][x][y]);
    }
    printf("\n");
  }
  printf("---Cb---\n");
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      printf("%02X ", imgin[1][x][y]);
    }
    printf("\n");
  }
  printf("---Cr---\n");
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      printf("%02X ", imgin[2][x][y]);
    }
    printf("\n");
  }
}

void ybr_to_rgb() {
  int x, y, z, i, j, count, ycbrint[3], rgbint[3];
  double RGB[3][3] = {{1, 0, 1.4020}, {1, -0.3441, -0.7141}, {1, 1.7720, 0}};
  double data;
  double offset;

  // YCbCrを表示するよ
  printf("\n<出力信号(YCbCr)>\n");
  printf("---Y---\n");
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      printf("%02X ", imgout[0][x][y]);
    }
    printf("\n");
  }
  printf("---Cb---\n");
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      printf("%02X ", imgout[1][x][y]);
    }
    printf("\n");
  }
  printf("---Cr---\n");
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      printf("%02X ", imgout[2][x][y]);
    }
    printf("\n");
  }

  // YCbCrをRGBに変換しているよ CbCrからは128だけ引いているよ
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      for (int z = 0; z < 3; z++) {
        rgb[0][x][y] += RGB[0][z] * imgout[z][x][y];
        rgb[1][x][y] += RGB[1][z] * (imgout[z][x][y] - 128);
        rgb[2][x][y] += RGB[2][z] * (imgout[z][x][y] - 128);
      }
    }
  }

  // 四捨五入してオフセット処理をしているよ
  for (z = 0; z < 3; z++) {
    for (x = 0; x < height; x++) {
      for (y = 0; y < width; y++) {
        data = rgb[z][x][y];

        if (data >= 0.0) {
          offset = 0.5;
        } else {
          offset = -0.5;
        }

        rgb[z][x][y] = (int)(data + offset);

        if (z != 0) {
          if (rgb[z][x][y] > 255.0) {
            rgb[z][x][y] = 255;
          } else if (rgb[z][x][y] < 0.0) {
            rgb[z][x][y] = 0.0;
          }
        }
      }
    }
  }


  // 結果をimgoutに挿入しているよ
  for (z = 0; z < 3; z++) {
    for (y = 0; y < width; y++) {
      for (x = 0; x < height; x++) {
        imgout[z][x][y] = (int)rgb[z][x][y];
      }
    }
  }


  // RGBを表示するよ
  printf("\n<出力信号(RGB)>\n");
  printf("---R---\n");
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      printf("%02X ", imgout[0][x][y]);
    }
    printf("\n");
  }
  printf("---G---\n");
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      printf("%02X ", imgout[1][x][y]);
    }
    printf("\n");
  }
  printf("---B---\n");
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      printf("%02X ", imgout[2][x][y]);
    }
    printf("\n");
  }
}

void put_data(void) {
  FILE *fp;
  int count = 0, x, y, z;
  char filename[100];
  printf("ファイル名を入力してください：");
  scanf("%s", filename);
  /*---ファイル・オープン(書き出し用) ---*/
  fp = fopen(filename, "wb");
  if (fp == NULL) {
    printf("ファイルをオープンできません．\n");
    exit(1);
  }
  printf("ファイルをオープンしました．\n");

  /*---ファイルに1バイトずつ書き出し ---*/
  while (count < 54) {
    fputc(header[count], fp);
    count++;
  }
  for (x = width - 1; x >= 0; x--) {
    for (y = 0; y < height; y++) {
      for (z = 2; z >= 0; z--) {
        fputc(imgout[z][x][y], fp);
      }
    }
  }

  /*--- ファイル・クローズ(書き出し用) ---*/
  fclose(fp);
  printf("\nファイルをクローズしました．\n");
}
