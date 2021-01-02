#include <stdio.h>
#include "nn.h"
#include <stdlib.h>
#include <time.h>

//float型配列の中の最大値とを返す
int max(int n, const float *x){
    int xmaxnumber = 0;
    for (int i = 0; i < n; i++){
        if(x[xmaxnumber]<x[i]){
            xmaxnumber = i;
        }
    }
    return xmaxnumber;
}

//float型(m,n)行列表示関数
void print(int m, int n, const float *x){
    for(int i = 0; i < m; i++){
        for(int k = 0; k < n; k++){
            printf("%.4f ", x[i*n+k]);
        }
        printf("\n");
    }
    printf("\n");
}

//行列積和演算関数
void fc(int m, int n, const float *x, const float *A,
        const float *b, float *y){
    for(int i = 0; i<m; i++){
        y[i] = b[i];
        for(int j=0; j<n; j++){
            y[i] = y[i] + A[i*n+j] * x[j];
        }
    }
}

//ReLU演算
void relu(int n, const float *x, float *y){
    for (int i = 0; i < n;i++){
        if(x[i]>0){
            y[i] = x[i];
        }else{
            y[i] = 0;
        }
    }
}

//softmax演算(
void softmax(int n, const float *x, float *y){
    float sum = 0;
    float xmax = x[max(n, x)];
    for (int i = 0; i < n; i++){//分母を求める
        sum = sum + expf(x[i] - xmax);
    }
    for (int i = 0; i < n; i++){
        y[i] = expf(x[i]-xmax)/sum;
    }
}

//推論演算（6層）
int inference6(const float *A1, const float *A2, const float *A3, const float *b1,
                const float *b2, const float *b3, const float *x, float *y){
    float *y50 = malloc(sizeof(float)*50);
    float *y100 = malloc(sizeof(float)*100);
    fc(50,784,x,A1,b1,y50);
    relu(50,y50,y50);
    fc(100,50,y50,A2,b2,y100);
    relu(100,y100,y100);
    fc(10,100,y100,A3,b3,y);
    softmax(10,y,y);
    free(y50);
    free(y100);
    return max(10,y);
}

//誤差逆伝搬(Softmax層)
void softmaxwithloss_bwd(int n, const float *y, unsigned char t, float *dEdx){
    
    for(int k = 0; k < n; k++){
        if(k==t){
            dEdx[k] = y[k] - 1;
        }else{
            dEdx[k] = y[k];
        }
    }
}

//誤差逆伝搬(ReLU層)
void relu_bwd(int n, const float *x, const float *dEdy, float *dEdx){
    for(int k = 0; k < n; k++){
        if(x[k]>0){
            dEdx[k] = dEdy[k];
        }else{
            dEdx[k] = 0;
        }
    }
}

//誤差逆伝搬(FC層)
void fc_bwd(int m, int n, const float *x, const float *dEdy,
            const float *A, float *dEdA, float *dEdb, float *dEdx){
    for(int k = 0; k < m; k++){
        for(int i = 0; i < n; i++){
            dEdA[k*n+i] = dEdy[k]*x[i];
        }
        dEdb[k] = dEdy[k];
    }
    for(int k=0;k<n;k++){
        dEdx[k]=0;
        for(int i=0;i<m;i++){
            dEdx[k]=dEdx[k] + A[i*n+k]*dEdy[i];
        }
    }
}

//誤差逆伝搬(6層)
void backward6(const float *A1, const float *A2, const float *A3, const float *b1,
                const float *b2, const float *b3, const float *x,
                unsigned char t, float *y, float *dEdA1, float *dEdb1,
                float *dEdA2, float *dEdb2,float *dEdA3, float *dEdb3){
    //推論
    float * yfc1 = malloc(sizeof(float)*50);
    float * yrelu1 = malloc(sizeof(float)*50);
    float * yfc2 = malloc(sizeof(float)*100);
    float * yrelu2 = malloc(sizeof(float)*100);
    fc(50,784,x,A1,b1,yfc1);
    relu(50,yfc1,yrelu1);
    fc(100,50,yrelu1,A2,b2,yfc2);
    relu(100,yfc2,yrelu2);
    fc(10,100,yrelu2,A3,b3,y);
    softmax(10,y,y);

    //誤差逆伝搬
    float *dEdx10 = malloc(sizeof(float)*10);
    float *dEdx100 = malloc(sizeof(float)*100);
    float *dEdx50 = malloc(sizeof(float)*50);
    float *dEdx784 = malloc(sizeof(float)*784);
    softmaxwithloss_bwd(10, y, t, dEdx10);
    fc_bwd(10,100,yrelu2,dEdx10,A3,dEdA3,dEdb3,dEdx100);
    relu_bwd(100, yfc2, dEdx100, dEdx100);
    fc_bwd(100,50,yrelu1,dEdx100,A2,dEdA2,dEdb2,dEdx50);
    relu_bwd(50, yfc1, dEdx50, dEdx50);
    fc_bwd(50, 784, x, dEdx50, A1, dEdA1, dEdb1, dEdx784);
    
    //メモリー開放
    free(yfc1);
    free(yfc2);
    free(yrelu1);
    free(yrelu2);
    free(dEdx10);
    free(dEdx100);
    free(dEdx50);
    free(dEdx784);
}

//二変数入れ替えfor int
void swapint(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

//順番シャッフル関数（要素数、配列）
void shuffle(int n, int *x){
    for (int i = 0; i < n; i++){
        int j = (int)(rand()*(n-1)/(1.0+RAND_MAX));
        swapint(&x[i], &x[j]);
    }
}

//交差エントロピー(損失関数)(確率配列、答え)
float cross_entropy_error(const float *y,int t){
    return -log(y[t]+1e-7);
}

//配列の和(要素数、足す配列、足される配列:o=o+x)
void add(int n, const float *x, float *o){
    for(int i = 0; i < n; i++){
        o[i] += x[i];
    }
}

//配列に定数をかける（要素数、かける定数、配列:o=o*x）
void scale(int n, float x, float *o){
    for(int i = 0; i < n; i++){
        o[i] *= x;
    }
}

//配列定数初期化関数（要素数、初期化定数、配列:o=x）
void init(int n, float x, float *o){
    for(int i = 0; i < n; i++){
        o[i] = x;
    }
}

//配列ランダム初期化関数（要素数、配列）
void rand_init(int n, float *o){
    for(int i = 0; i < n; i++){
        o[i] = rand()*2/(1.0+RAND_MAX)-1; 
    }
}

//fcのパラメータの保存(ファイル名、行、列、Ａ、ｂ)
void save(const char *filename, int m, int n, const float *A, const float *b){
    FILE *fp;
    fp = fopen(filename, "wb");
    fwrite(&m, sizeof(int), 1, fp);
    fwrite(&n, sizeof(int), 1, fp);
    fwrite(A, sizeof(float), m*n, fp);
    fwrite(b, sizeof(float), m, fp);
    printf("saved %s\n", filename);
    fclose(fp);
}

//fcのパラメータの読み込み(ファイル名、行、列、Ａ、ｂ)通常0,エラーであれば-1を返す
int load(const char *filename, int m, int n, float *A, float *b){
    FILE *fp=NULL;
    fp = fopen(filename, "rb");
    if(!fp){
        printf("error: File cannot open\n");
        return -1;
    }else{
        int mread, nread;//m,nの読み込み用の変数
        fread(&mread, sizeof(int), 1, fp);
        fread(&nread, sizeof(int), 1, fp);
        if(m==mread && n==nread){
            fread(A, sizeof(float), m*n, fp);
            fread(b, sizeof(float), m, fp);
        }else{
            printf("error: Inputed m or n is different from loaded m or n\n");
            return -1;
        }
        fclose(fp);
        return 0;
    }
}


int main(int argc, char *argv[]){
    float *train_x = NULL;
    unsigned char *train_y = NULL;
    int train_count = -1;

    float *test_x = NULL;
    unsigned char *test_y = NULL;
    int test_count = -1;

    int width = -1;
    int height = -1;

    load_mnist(&train_x, &train_y, &train_count,
             &test_x, &test_y, &test_count,
             &width, &height);
    //推論出力用配列
    float * y = malloc(sizeof(float)*10);
    float * A1 = malloc(sizeof(float)*784*50);
    float * A2 = malloc(sizeof(float)*50*100);
    float * A3 = malloc(sizeof(float)*100*10);
    float * b1 = malloc(sizeof(float)*50);
    float * b2 = malloc(sizeof(float)*100);
    float * b3 = malloc(sizeof(float)*10);

    if(*argv[1]=='l'){//学習モード
        if(argc < 5){
            printf("Prease input 3 output file names after 'l'");
            return -1;
        }
        float cross_entropy_error_sum = 0;//損失関数合計
        int correct_answer_sum = 0;//正解数合計

        int epoch=20;//エポック回数
        int minibatch = 20;//ミニバッチサイズ
        int batchtime = train_count / minibatch; //バッチ学習回数
        float learningrate = 0.05;//学習率
        //勾配及び勾配平均変数を宣言
        float * dEdA1 = malloc(sizeof(float)*784*50);
        float * dEdA2 = malloc(sizeof(float)*50*100);
        float * dEdA3 = malloc(sizeof(float)*100*10);
        float * dEdb1 = malloc(sizeof(float)*50);
        float * dEdb2 = malloc(sizeof(float)*100);
        float * dEdb3 = malloc(sizeof(float)*10);
        float * avedEdA1 = malloc(sizeof(float)*784*50);
        float * avedEdA2 = malloc(sizeof(float)*50*100);
        float * avedEdA3 = malloc(sizeof(float)*100*10);
        float * avedEdb1 = malloc(sizeof(float)*50);
        float * avedEdb2 = malloc(sizeof(float)*100);
        float * avedEdb3 = malloc(sizeof(float)*10);
        srand(time(NULL));
        //パラメータをランダムに初期化
        rand_init(784*50, A1);
        rand_init(50, b1);
        rand_init(50*100, A2);
        rand_init(100, b2);
        rand_init(100*10, A3);
        rand_init(10, b3);
        int *index = malloc(sizeof(int)*train_count);

        for(int i = 0; i < epoch; i++){     //iはエポック回数
            for(int i=0; i<train_count; i++){
                index[i]=i;
            }
            shuffle(train_count, index);
            for(int j = 0; j < batchtime; j++){//jはミニバッチ回数
                //勾配を初期化する
                init(784*50, 0, avedEdA1);
                init(50*100, 0, avedEdA2);
                init(100*10, 0, avedEdA3);
                init(50, 0, avedEdb1);
                init(100, 0, avedEdb2);
                init(10, 0, avedEdb3);

                for(int k = 0; k < minibatch; k++){ //kはバッチ回数
                    backward6(A1, A2, A3, b1, b2, b3, train_x + 784*index[j*minibatch+k], train_y[index[j*minibatch+k]], y, dEdA1, dEdb1, dEdA2, dEdb2, dEdA3, dEdb3);
                    add(784*50, dEdA1, avedEdA1);
                    add(50*100, dEdA2, avedEdA2);
                    add(100*10, dEdA3, avedEdA3);
                    add(50, dEdb1, avedEdb1);
                    add(100, dEdb2, avedEdb2);
                    add(10, dEdb3, avedEdb3);
                }
                //平均の勾配を計算し学習率のマイナス倍をかける
                scale(784 * 50, -(float)learningrate / (float)minibatch, avedEdA1);
                scale(50 * 100, -(float)learningrate / (float)minibatch, avedEdA2);
                scale(100 * 10, -(float)learningrate / (float)minibatch, avedEdA3);
                scale(50, -(float)learningrate / (float)minibatch, avedEdb1);
                scale(100, -(float)learningrate / (float)minibatch, avedEdb2);
                scale(10, -(float)learningrate / (float)minibatch, avedEdb3);
                //パラメータを更新する
                add(784 * 50, avedEdA1, A1);
                add(50 * 100, avedEdA2, A2);
                add(100 * 10, avedEdA3, A3);
                add(50, avedEdb1, b1);
                add(100, avedEdb2, b2);
                add(10, avedEdb3, b3);
            }
            //損失関数の平均と正答率を出力する
            cross_entropy_error_sum = 0;
            correct_answer_sum = 0;
            for (int i = 0; i < train_count; i++){
                if(inference6(A1, A2, A3, b1, b2, b3, train_x + i*width*height, y) == train_y[i]){
                    correct_answer_sum++;
                }
                cross_entropy_error_sum += cross_entropy_error(y, train_y[i]);
            }
            float cee_fortraining = cross_entropy_error_sum / train_count;
            float pca_fortraining = correct_answer_sum * 100.0 / train_count;
            printf("Epoch : %d / %d\n", i + 1, epoch);
            printf("Result for training data\n");
            printf(">>Cross entropy average = %f\n", cee_fortraining);
            printf(">>Percentage of correct answers = %f%%\n", pca_fortraining);
            cross_entropy_error_sum = 0;
            correct_answer_sum = 0;
            for(int i=0; i<test_count; i++){
                if(inference6(A1, A2, A3, b1, b2, b3, test_x + i*width*height, y) == test_y[i]){
                    correct_answer_sum++;
                }
                cross_entropy_error_sum += cross_entropy_error(y, test_y[i]);
            }
            float cee_fortest = cross_entropy_error_sum / test_count;
            float pca_fortest = correct_answer_sum * 100.0 / test_count;
            printf("Result for training data\n");
            printf(">>Cross entropy average = %f\n", cee_fortest);
            printf(">>Percentage of correct answers = %f%%\n", pca_fortest);
            if((pca_fortraining - pca_fortest) > 5){
                printf("The percentage of correct inference for training data is too much than that for test data.\n");
                printf("This training was canceled.\n");
                return 0;
            }
        }
        //パラメータを保存する
        save(argv[2], 50, 784, A1, b1);
        save(argv[3], 100, 50, A2, b2);
        save(argv[4], 10, 100, A3, b3);
    }else if(*argv[1]=='i'){//推論モード
        if(argc < 6){
            printf("Prease specify 3 parameter file names and a bmp image after 'i'");
            return -1;
        }
        int error = 0;
        float * x = load_mnist_bmp(argv[5]);//画像の読み込み
        error += load(argv[2], 50, 784, A1, b1);
        error += load(argv[3], 100, 50, A2, b2);
        error += load(argv[4], 10, 100, A3, b3);
        if (error != 0){//ファイルの読み込みでエラーが発生した場合
            return -1;
        }
        //結果を出力する
        printf("The result of inference is %d\n", inference6(A1, A2, A3, b1, b2, b3, x, y));
        printf("Probability of each number>>\n");
        for(int i = 0; i<10;i++){
            printf("%d: %.4f ", i, y[i]);
        }
        printf("\n");
    }else{
        printf("Mode is not specified.\n");
        printf("Lerning Mode >> l,  Inference Mode >> i\n");
    }
    return 0;
}