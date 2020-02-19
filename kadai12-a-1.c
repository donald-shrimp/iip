#include <stdio.h>

void get_data(void);
void processing(void);
void put_data(void);
void rgb_to_ybr(void);
void ybr_to_rgb(void);
int round_off(double data);
int limit(int data);


unsigned char header[54];
unsigned char imgin[3][512][512];
unsigned char imgout[3][512][512];
double tmp_r2y[3][512][512] = {};
double tmp_y2r[3][512][512] = {};
int filesize, width, height, ins_byte;
const double convert[3][3] = {  //変換式(RGB -> YCbCr)
    {0.2990, 0.5870, 0.1140},
    {-0.1687, -0.3313, 0.5000},
    {0.5000, -0.4187, -0.0813}};
const double convert_rev[3][3] = {  //変換式(YCbCr -> RGB)
    {1.0000, 0.0000, 1.4020},
    {1.0000, -0.3441, -0.7141},
    {1.0000, 1.7720, 0.0000}};

int main(void) {
  get_data();
  rgb_to_ybr();
  
  processing();

  ybr_to_rgb();
  put_data();
  return 0;
}

void processing(void) {
  int i,j,x,y,z;
  double percent,xd,yd,s,t,filted;
double num,r1,r0;
double filter[3][3]={{1.0/9.0,1.0/9.0,1.0/9.0},
                    {1.0/9.0,1.0/9.0,1.0/9.0},
                    {1.0/9.0,1.0/9.0,1.0/9.0}};
printf("\n＜フィルタ係数＞\n");
for(x=0;x<3;x++){
    for(y=0;y<3;y++){
        printf("%lf ",filter[x][y]);
    }
    printf("\n");
}

printf("\nフィルタ処理する画素の座標を入力してください\n");
printf("水平座標：");
scanf("%d",&x);
printf("垂直座標：");
scanf("%d",&y);

//とりあえずコピーする
  for(i=0;i<height;i++){
    for(j=0;j<width;j++){
      imgout[0][i][j]=imgin[0][i][j];
      imgout[1][i][j]=imgin[1][i][j];
      imgout[2][i][j]=imgin[2][i][j];
    }
  }

if(x==0||x==width-1||y==0||y==height-1){//画像端の判別
  printf("\n----- この座標は画像の端に位置するためフィルタ処理ができません\n");
}else{
  printf("\n＜フィルタ処理に使用する画素の値＞\n");
  filted=0;

  //フィルター処理
  for(j=-1;j<2;j++){
    for(i=-1;i<2;i++){
      printf("%d ",imgin[0][y+j][x+i]);
      filted+=(double)(imgin[0][y+j][x+i]*filter[j+1][i+1]);
    }
      printf("\n");
    }
  
  //フィルターをかけたものを足したやつを四捨五入してimgoutに代入
  filted=(int)(filted+0.5);
  imgout[0][y][x]=(int)filted;
  
  printf("\n＜フィルタ処理後の画素値＞\n%d\n",(int)filted);
  printf("\n入力画像データをコピーして出力画像データを作成しました.\n");
}
}

void get_data(void)  //ヘッダのデータを取得
{
  FILE *fp;            //ファイルポインタ
  char filename[100];  //ファイル名格納
  int i, j, k;         //ループカウンタ

  //参照ファイル名入力
  printf("入力ファイル名を入力してください：");
  scanf("%s", filename);

  //バイナリファイル読み取りでfopen
  fp = fopen(filename, "rb");

  if (fp == NULL)  //エラー処理
  {
    printf("ファイルが存在しませんでした\n");
    return;
  }

  printf("ファイルをオープンしました\n");

  //配列にデータ格納
  for (i = 0; i < 54; i++) {
    header[i] = fgetc(fp);
  }

  filesize = *(int *)&header[2];
  width = *(int *)&header[18];   //幅
  height = *(int *)&header[22];  //高さ
  ins_byte = *(int *)&header[2] - *(int *)&header[10] -
             *(int *)&header[18] * *(int *)&header[22] *
                 (*(short *)&header[28] / 8);  //挿入バイト数

  //以下データ出力部
  // printf("＜ファイルタイプ＞\nheader[0]=%02x
  // header[1]=%02x\n",header[0],header[1]);

  printf("\n＜ファイルサイズ＞\n");
  printf("\n%dバイト\n", filesize);

  // printf("\n＜予約領域＞\nheader[6]=%02x header[7]=%02x header[8]=%02x
  // header[9]=%02x\n",header[6],header[7],header[8],header[9]);

  printf("\n＜オフセット＞\n");
  printf("\n%dバイト\n", *(int *)&header[10]);

  // printf("\n＜情報ヘッダサイズ＞\nheader[14]=%02x header[15]=%02x
  // header[16]=%02x
  // header[17]=%02x\n",header[14],header[15],header[16],header[17]);

  printf("\n＜画像の幅＞\n");
  printf("\n%d画素\n", width);

  printf("\n＜画像の高さ＞\n");
  printf("\n%dライン\n", height);


  printf("\n＜１画素当たりのビット数＞\n");
  printf("\n%oバイト\n", *(short *)&header[28]);


  printf("\n＜挿入ビット数＞\n%dバイト\n", ins_byte);

  //画像データ格納
  for (i = height - 1; i >= 0; i--) {
    for (j = 0; j < width; j++) {
      for (k = 2; k >= 0; k--) {
        imgin[k][i][j] = fgetc(fp);
      }
    }
  }

  fclose(fp);
  printf("ファイルをクローズしました\n\n");
}

void put_data(void) {
  int i, j, k;
  char new_fname[100];
  printf("出力ファイル名を入力してください：");
  scanf("%s", new_fname);

  FILE *fp;
  fp = fopen(new_fname, "w");

  if (fp == NULL) {
    printf("ファイルをオープンできません\n");
    return;
  }

  printf("ファイルをオープンしました\n");

  for (int i = 0; i < 54; i++) {
    fputc(header[i], fp);
  }

  for (i = height - 1; i >= 0; i--) {
    for (j = 0; j < width; j++) {
      for (k = 2; k >= 0; k--) {
        fputc(imgout[k][i][j], fp);
      }
    }
  }

  fclose(fp);
  printf("ファイルをクローズしました\n");
}

void rgb_to_ybr(void) {
  int i, j, k;

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      for (k = 0; k < 3; k++) {
        tmp_r2y[0][i][j] += convert[0][k] * imgin[k][i][j];
        tmp_r2y[1][i][j] += convert[1][k] * imgin[k][i][j];
        tmp_r2y[2][i][j] += convert[2][k] * imgin[k][i][j];
      }
    }
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < width; j++) {
      for (int k = 0; k < height; k++) {
        if (i == 0)
          imgin[i][j][k] = limit(round_off(tmp_r2y[i][j][k]));
        else
          imgin[i][j][k] = limit(round_off(tmp_r2y[i][j][k]) + 128);
      }
    }
  }
}

void ybr_to_rgb(void) {
  short hoge[3][512][512] = {};

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < height; j++) {
      for (int k = 0; k < width; k++) {
        hoge[i][k][j] = imgout[i][k][j];
      }
    }
  }

  for (int i = 1; i < 3; i++) {
    for (int j = 0; j < height; j++) {
      for (int k = 0; k < width; k++) {
        hoge[i][k][j] -= 128.0;
      }
    }
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      for (int k = 0; k < 3; k++) {
        tmp_y2r[0][i][j] += convert_rev[0][k] * hoge[k][i][j];
        tmp_y2r[1][i][j] += convert_rev[1][k] * hoge[k][i][j];
        tmp_y2r[2][i][j] += convert_rev[2][k] * hoge[k][i][j];
      }
    }
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < width; j++) {
      for (int k = 0; k < height; k++) {
        imgout[i][j][k] = limit(round_off(tmp_y2r[i][j][k]));
      }
    }
  }
}

int round_off(double data) {
  if (data >= 0)
    data += 0.5;
  else
    data -= 0.5;

  return (int)data;
}

int limit(int data) {
  if (data > 255)
    data = 255;
  else if (data < 0)
    data = 0;

  return data;
}

