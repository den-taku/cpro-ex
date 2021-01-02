#include "nn.h"
#include <time.h>

/* m×n行列の成分を表示する関数 */
void print(int m, int n, const float * x){
    int i, j;
    for (i = 0; i < m; i++){
        for (j = 0; j < n; j++){
            printf("%.4f ", x[n * i + j]);
        }
        printf("\n");
    }
}

/* Ax+bを計算する関数 */
void fc(int m, int n, const float * x, const float * A, const float * b, float * y){
    int i, j;
    for (i = 0; i < m; i++){
        y[i] = b[i];
        for (j = 0; j < n; j++){
            y[i] += A[n * i + j] * x[j];
        }
    }
}

/* ReLU関数 */
void relu(int n, const float *x, float *y){
    int i;
    for (i = 0; i < n; i++){
        if (x[i] > 0){
            y[i] = x[i];
        } else {
            y[i] = 0;
        }
    }
}

/* Softmax関数 */
void softmax(int n, const float *x, float *y){
    float sum_expx = 0;
    int i;
    float xmax = x[0];
    for (i = 0; i < n; i++){
        if (xmax < x[i]){
            xmax = x[i];
        }
    }
    for (i = 0; i < n; i++){
        sum_expx += exp(x[i] - xmax);
    }
    for (i = 0; i < n; i++){
        y[i] = exp(x[i] - xmax) / sum_expx;
    }
}

/* 画像データから数字を推論する関数 */
int inference6(const float *A1, const float *b1, const float *A2, const float *b2, const float *A3, const float *b3, const float *x, float *y) {
    float *y1 = malloc(sizeof(float) * 50);     /* FC1層の出力 */
    float *y2 = malloc(sizeof(float) * 100);    /* FC2層の出力 */

    fc(50, 784, x, A1, b1, y1);
    relu(50, y1, y1);
    fc(100, 50, y1, A2, b2, y2);
    relu(100, y2, y2);
    fc(10, 100, y2, A3, b3, y);  /* yはFC3層の出力 */
    softmax(10, y, y);

    int i;
    float max = y[0];
    int maxindex = 0;
    for (i = 0; i < 10; i++){
        if (max < y[i]){
            max = y[i];
            maxindex = i;
        }
    }

    free(y1);
    free(y2);

    return maxindex;
}

/* Softmax層の勾配を計算する関数（誤差逆伝搬）　*/
void softmaxwithloss_bwd(int n, const float *y, unsigned char t, float *dEdx){
    int i;
    int ans[10] = {0};
    for (i = 0; i < 10; i++){
        if (i == t){
            ans[i] = 1;
        }
    }
    for (i = 0; i < n; i++){
        dEdx[i] = y[i] - ans[i];
    }
}

/* ReLU層の勾配を計算する関数（誤差逆伝搬） */
void relu_bwd(int n, const float *x, const float *dEdy, float *dEdx){
    int i;
    for (i = 0; i < n; i++){
        if (x[i] > 0){
            dEdx[i] = dEdy[i];
        } else {
            dEdx[i] = 0;
        }
    }
}

/* FC層の勾配を計算する関数（誤差逆伝搬） */
void fc_bwd(int m, int n, const float *x, const float *dEdy, const float *A, float *dEdA, float *dEdb, float *dEdx){
    int i, j;
    for (i = 0; i < m; i++){
        for (j = 0; j < n; j++){
            dEdA[i * n + j] = dEdy[i] * x[j];
        }
    }
    for (i = 0; i < m; i++){
        dEdb[i] = dEdy[i];
    }
    for (i = 0; i < n; i++){
        dEdx[i] = 0;
        for (j = 0; j < m; j++){
            dEdx[i] += A[j * n + i] * dEdy[j];
        }
    }
}

/* 6層の誤差逆伝搬を行う関数 */
void backward6(const float *A1, const float *b1, const float *A2, const float *b2, const float *A3, const float *b3, const float *x, unsigned char t, float *y, float *dEdA1, float *dEdb1, float *dEdA2, float *dEdb2, float *dEdA3, float *dEdb3) {
    float *y_fc1 = malloc(sizeof(float) * 50);     /* FC1層の出力 */
    float *y_relu1 = malloc(sizeof(float) * 50);   /* ReLU1層の出力 */
    float *y_fc2 = malloc(sizeof(float) * 100);    /* FC2層の出力 */
    float *y_relu2 = malloc(sizeof(float) * 100);  /* ReLU2層の出力 */
    float *y_fc3 = malloc(sizeof(float) * 10);     /* FC3層の出力 */
    float *dEdx3 = malloc(sizeof(float) * 10);     /* Softmax層の入力の勾配 */
    float *dEdx2 = malloc(sizeof(float) * 100);    /* FC3層の入力の勾配 */
    float *dEdx1 = malloc(sizeof(float) * 50);     /* FC2層の入力の勾配 */
    float *dEdx = malloc(sizeof(float) * 784);     /* FC1層の入力の勾配 */

    /* 推論 */
    fc(50, 784, x, A1, b1, y_fc1);
    relu(50, y_fc1, y_relu1);
    fc(100, 50, y_relu1, A2, b2, y_fc2);
    relu(100, y_fc2, y_relu2);
    fc(10, 100, y_relu2, A3, b3, y_fc3);
    softmax(10, y_fc3, y);

    /* 誤差逆伝搬 */
    softmaxwithloss_bwd(10, y, t, dEdx3);
    fc_bwd(10, 100, y_relu2, dEdx3, A3, dEdA3, dEdb3, dEdx2);
    relu_bwd(100, y_fc2, dEdx2, dEdx2);
    fc_bwd(100, 50, y_relu1, dEdx2, A2, dEdA2, dEdb2, dEdx1);
    relu_bwd(50, y_fc1, dEdx1, dEdx1);
    fc_bwd(50, 784, x, dEdx1, A1, dEdA1, dEdb1, dEdx);

    /* メモリ開放 */
    free(y_fc1);
    free(y_relu1);
    free(y_fc2);
    free(y_relu2);
    free(y_fc3);
    free(dEdx3);
    free(dEdx2);
    free(dEdx1);
    free(dEdx);
}

/* n列の配列の要素をランダムに並び替える関数 */
void shuffle(int n, int *x){
    int i, p, q, temp;
    for (i = 0; i < n; i++){
        p = rand() % n;
        q = rand() % n;
        temp = x[p];
        x[p] = x[q];
        x[q] = temp;
    }
}

/* 損失関数 */
float cross_entropy_error(const float *y, int t){
    return -log(y[t] + 1e-7);
}

/* n列の配列の加算を行う関数 */
void add(int n, const float * x, float * o){
    int i;
    for (i = 0; i < n; i++){
        o[i] += x[i];
    }
}

/* n列の配列に定数を乗算する関数 */
void scale(int n, float x, float * o){
    int i;
    for (i = 0; i < n; i++){
        o[i] = o[i] * x;
    }
}

/* n列の配列の要素をすべて一つの定数で初期化する関数 */
void init(int n, float x, float * o){
    int i;
    for (i = 0; i < n; i++){
        o[i] = x;
    }
}

/* n列の配列の要素を[-1:1]の乱数で初期化する関数 */
void rand_init(int n, float * o){
    int i;
    for (i = 0; i < n; i++){
        o[i] = (float)rand() * 2.0 / (RAND_MAX+1) - 1.0;
    }
}

/* 1層分のパラメータを保存する関数 */
void save(const char *filename, int m, int n, const float *A, const float *b){
    FILE *fp;
    fp = fopen(filename, "w");
    fwrite(A, sizeof(float), m * n, fp);
    fwrite(b, sizeof(float), n, fp);
    fclose(fp);
}

/* 1層分のパラメータを読み込む関数 */
void load(const char *filename, int m, int n, float *A, float *b){
    FILE *fp;
    fp = fopen(filename, "r");
    fread(A, sizeof(float), m * n, fp);
    fread(b, sizeof(float), n, fp);
    fclose(fp);
}

/* 6層で学習する関数 */
void learn6(int epoch_count, int mini_batch_size, float h, int train_count, float *train_x, unsigned char *train_y, int test_count, float *test_x, unsigned char *test_y,float *A1, float *b1, float *A2, float *b2, float *A3, float *b3, char*argv[]){
    /* パラメータを乱数で初期化 */
    rand_init(50 * 784, A1);
    rand_init(50, b1);
    rand_init(100 * 50, A2);
    rand_init(100, b2);
    rand_init(10 * 100, A3);
    rand_init(10, b3);

    int *index = malloc(sizeof(int) * train_count);        /* 訓練データの添字 */
    float *ave_dEdA1 = malloc(sizeof(float) * 50 * 784);   /* A1の勾配の平均 */
    float *ave_dEdb1 = malloc(sizeof(float) * 50);         /* b1の勾配の平均 */
    float *ave_dEdA2 = malloc(sizeof(float) * 100 * 50);   /* A2の勾配の平均 */
    float *ave_dEdb2 = malloc(sizeof(float) * 100);        /* b2の勾配の平均 */
    float *ave_dEdA3 = malloc(sizeof(float) * 10 * 100);   /* A3の勾配の平均 */
    float *ave_dEdb3 = malloc(sizeof(float) * 10);         /* b3の勾配の平均 */
    float *dEdA1 = malloc(sizeof(float) * 50 * 784);       /* 各訓練データのA1の勾配 */
    float *dEdb1 = malloc(sizeof(float) * 50);             /* 各訓練データのb1の勾配 */
    float *dEdA2 = malloc(sizeof(float) * 100 * 50);       /* 各訓練データのA2の勾配 */
    float *dEdb2 = malloc(sizeof(float) * 100);            /* 各訓練データのb2の勾配 */
    float *dEdA3 = malloc(sizeof(float) * 10 * 100);       /* 各訓練データのA3の勾配 */
    float *dEdb3 = malloc(sizeof(float) * 10);             /* 各訓練データのb3の勾配 */
    float *y = malloc(sizeof(float) * 10);                 /* 推論結果 */

    int i, j, k;
    /* 添字を初期化 */
    for (i = 0; i < train_count; i++){
        index[i] = i;
    }

    for (i = 0; i < epoch_count; i++){
        /* 1エポック分の学習 */
        printf("=============== Epoch %d/%d ===============\n", i + 1, epoch_count);
        shuffle(train_count, index);   /* 添字をランダムに入れ替える */

        for (j = 0; j < train_count / mini_batch_size; j++){
            /* 1回分のミニバッチ学習 */

            /* パラメータの平均勾配を0で初期化 */
            init(50 * 784, 0, ave_dEdA1);
            init(50, 0, ave_dEdb1);
            init(100 * 50, 0, ave_dEdA2);
            init(100, 0, ave_dEdb2);
            init(10 * 100, 0, ave_dEdA3);
            init(10, 0, ave_dEdb3);

            for (k = 0; k < mini_batch_size; k++){
                /* 一つ分のデータの学習 */
                backward6(A1, b1, A2, b2, A3, b3, train_x + 784 * index[j * mini_batch_size + k], train_y[j * mini_batch_size + k], y, dEdA1, dEdb1, dEdA2, dEdb2, dEdA3, dEdb3);
                /* パラメータの勾配を平均勾配に加えていく */
                add(50 * 784, dEdA1, ave_dEdA1);
                add(50, dEdb1, ave_dEdb1);
                add(100 * 50, dEdA2, ave_dEdA2);
                add(100, dEdb2, ave_dEdb2);
                add(10 * 100, dEdA3, ave_dEdA3);
                add(10, dEdb3, ave_dEdb3);
            }

            /* パラメータの平均勾配を出し, 学習率を乗ずる */
            scale(50 * 784, -h / mini_batch_size, ave_dEdA1);
            scale(50, -h / mini_batch_size, ave_dEdb1);
            scale(100 * 50, -h / mini_batch_size, ave_dEdA2);
            scale(100, -h / mini_batch_size, ave_dEdb2);
            scale(10 * 100, -h / mini_batch_size, ave_dEdA3);
            scale(10, -h / mini_batch_size, ave_dEdb3);

            /* パラメータの更新 */
            add(50 * 784, ave_dEdA1, A1);
            add(50, ave_dEdb1, b1);
            add(100 * 50, ave_dEdA2, A2);
            add(100, ave_dEdb2, b2);
            add(10 * 100, ave_dEdA3, A3);
            add(10, ave_dEdb3, b3);

            printf("%.1f %% of training in Epoch%d is completed. \r", (float)j * 100 / (train_count / mini_batch_size), i + 1);
        }

        printf("                                            \r");
        /* 訓練データの推論 */
        printf("Now inferring...\r");
        int sum_train = 0;
        float sum_loss_train = 0;
        for (j = 0; j < train_count; j++){
            if(inference6(A1,b1,A2,b2,A3,b3,train_x+784*j,y)==train_y[j]){
                sum_train++;
            }
            sum_loss_train += cross_entropy_error(y, train_y[j]);
        }
        printf("Accuracy -train- :%f%%\n", sum_train*100.0/train_count);
        printf("Loss     -train- :%f\n", sum_loss_train/train_count);

        /* テストデータの推論 */
        printf("Now inferring...\r");
        int sum_test = 0;
        float sum_loss_test = 0;
        for (j = 0; j < test_count; j++){
            if(inference6(A1,b1,A2,b2,A3,b3,test_x+784*j,y)==test_y[j]){
                sum_test++;
            }
            sum_loss_test += cross_entropy_error(y, test_y[j]);
        }
        printf("Accuracy  -test- :%f%%\n", sum_test*100.0/test_count);
        printf("Loss      -test- :%f\n", sum_loss_test/test_count);
    }

    /* パラメータの保存 */
    save(argv[1], 50, 784, A1, b1);
    save(argv[2], 100, 50, A2, b2);
    save(argv[3], 10, 100, A3, b3);
}

    int main(int argc, char *argv[]){  /* パラメータの保存先を受け取る */
    /* 訓練データとテストデータを読み込む */
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

    /* パラメータの動的メモリを確保 */
    float *A1 = malloc(sizeof(float) * 50 * 784);
    float *b1 = malloc(sizeof(float) * 50);
    float *A2 = malloc(sizeof(float) * 100 * 50);
    float *b2 = malloc(sizeof(float) * 100);
    float *A3 = malloc(sizeof(float) * 10 * 100);
    float *b3 = malloc(sizeof(float) * 10);

    /* 乱数を現在時刻で初期化 */
    srand(time(NULL));

    /* 訓練データとテストデータで学習し, パラメータを保存 */
    learn6(20, 100, 0.1, train_count, train_x, train_y, test_count, test_x, test_y, A1, b1, A2, b2, A3, b3, argv);

    return 0;
}