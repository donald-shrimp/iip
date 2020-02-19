#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    FILE *fp;
    int c,count=0,i;
    unsigned char ary[100];
    char filename[100],filenamecp[100];
    //名前を聞く
    printf("コピー元のファイル名を入力してください：");
    scanf("%s",filename);
    printf("コピー先のファイル名を入力してください：");
    scanf("%s",filenamecp);
    /*---ファイル・オープン(読み込み用) ---*/
    fp=fopen(filename,"rb");
    if (fp==NULL){
        printf("読み込み用ファイルをオープンできません．\n");
        exit (1);
    }
    printf("読み込み用ファイルをオープンしました．\n");
    /*---ファイルから1バイトずつ読み込みディスプレイに表示---*/
    c=fgetc(fp);
    while (c!=EOF) {
        ary[count]=c;
        c=fgetc(fp);
        count++;
    }
    /*--- ファイル・クローズ(読み込み用) ---*/
    fclose(fp);
    printf("%d個のデータを読み込み、配列に格納しました.\n",count);
    printf("読み込み用ファイルをクローズしました．\n");

    //ここまで読み込み

    printf("\n配列の内容を表示します.\n");
    for (i=0; i<count ; i++) {
        printf("ary[%d]=%d\n",i,ary[i]);
    }

    //ここから書き出し

    /*---ファイル・オープン(書き出し用) ---*/
    fp=fopen(filenamecp,"wb");
    if (fp==NULL){
        printf("\n書き出し用ファイルをオープンできません．\n");
        exit (1);
    }
    printf("\n書き出し用ファイルをオープンしました．\n");

    printf("配列に格納されていた%d個のデータを書き出しました.",count);

    /*---ファイルに1バイトずつ書き出し---*/
    for (i=0;i<count;i++){

        fputc(ary[i],fp);
    }
    /*--- ファイル・クローズ(書き出し用) ---*/
    fclose(fp);
    printf("\n書き出し用ファイルをクローズしました．\n");
    return (0);
}