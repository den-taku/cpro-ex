#include "nn.h"
#include <time.h>

// print x (x : m * n matrix)
void print(int m, int n, const float* x);
// y = A * x + b (A : m * n matrix, x : n vector, b : m vector, y : m vector)
void fc(int m, int n, const float* x, const float* A, const float* b, float* y);
  // y = A * x
  void mul(int m, int n, const float* A, const float* x, float* y);
  //  y += x
  void add(int n, const float* b, float* y);
// y = ReLU(x) (y, x : n vector)
void relu(int n, const float* x, float* y);
// y = Softmax(x) (x, y : n vector)
void softmax(int n, const float* x, float* y);
  // sort y (y : n vector)
  void sort(int n, float* y);
  // quick sort l (l : array[length])
  void quick_sort(float* l, int length);
  // private function for quick sort
  void quick_sort_local(float* l, int large, int small);
  // l[a] < - > l[b]
  void change(float* l, int a, int b);
//return answer (A : m * n matrix, x : n vector, b : m vector)
int inference3(const float* A, const float* b, const float* x);
  // return y's largest element's index
  int largest_index(int size, const float* y);
  // array[j] < - > array[j + 1]
  void swap_array(float* array, int j);
// dE/dx (11) (y, t : n vector)
void softmaxwidthloss_bwd(int n, const float* y, unsigned char t, float* dEdx);
// dE/dx = (x > 0) ? dE/dy : 0
void relu_bwd(int n, const float* x, const float* dEdy, float* dEdx);
// dE/dA dE/db dE/dx
void fc_bwd(int m, int n, const float* x, const float* dEdy, const float* A, float* dEdA, float* dEdb, float* dEdx);
// for learning
void backward3(const float* A, const float* b, const float* x, unsigned char t, float* y, float* dEdA, float* dEdb);
// x : n vector
void shuffle(int n, int* x);
float cross_entropy_error(const float* y, int t);
void scale(int n, float x, float* o);
// o[i] = x
void init(int n, float x, float* o);
// o[i] = [-1:1]
void rand_init(int n, float* o);
// LEARNING PHRASE
//e : epoch count, b : batch size,  rate : learning rate
//void learning(int e, int bs, float rate);
//void learn(int epoch_count, int batch_size, float learning_rate);

int inference6(const float* A1, const float* b1, const float* A2, const float* b2, const float* A3, const float* b3, const float*x, float* y3);
void backward6(const float* A1, const float* b1, const float* A2, const float* b2, const float* A3, const float* b3, const float* x, unsigned char t, float* y3, float* dEdA1, float* dEdb1, float* dEdA2, float* dEdb2, float* dEdA3, float* dEdb3);
void save(const char* filename, int m, int n, const float* A, const float* b);
void load(const char* filename, int m, int n, float* A, float* b);

int main()
{
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

  // これ以降，３層NN の係数 A_784x10 および b_784x10 と，
  // 訓練データ train_x + 784*i (i=0,...,train_count-1), train_y[0]～train_y[train_count-1],
  // テストデータ test_x + 784*i (i=0,...,test_count-1), test_y[0]～test_y[test_count-1],
  // を使用することができる．

  srand(time(NULL));

  int i, j, k;
  int epoch_count = 10;
  int batch_size = 100;
  float learning_rate = 0.1;
  float* A1 = malloc(sizeof(float)*50*784);
  float* b1 = malloc(sizeof(float)*50);
  float* A2 = malloc(sizeof(float)*100*50);
  float* b2 = malloc(sizeof(float)*100);
  float* A3 = malloc(sizeof(float)*10*100);
  float* b3 = malloc(sizeof(float)*10);
  int* index = malloc(sizeof(int)*train_count);
  rand_init(50 * 784, A1);
  rand_init(50, b1);
  rand_init(100 * 50, A2);
  rand_init(100, b2);
  rand_init(10 * 100, A3);
  rand_init(10, b3);
  for(i = 0; i < 60000; ++i){
    index[i] = i;
  }
// ----i time epoch----
  for(i = 0; i < epoch_count; ++i){
    // ----epoch----
    printf("%d time epoch\n", i+1);
    shuffle(train_count, index);
    for(j = 0; j < train_count / batch_size; ++j){
      // ----mini batch----
      float* _dEdA1 = malloc(sizeof(float)*50*784);
      float* _dEdb1 = malloc(sizeof(float)*50);
      float* _dEdA2 = malloc(sizeof(float)*100*50);
      float* _dEdb2 = malloc(sizeof(float)*100);
      float* _dEdA3 = malloc(sizeof(float)*10*100);
      float* _dEdb3 = malloc(sizeof(float)*10);
      init(50*784, 0, _dEdA1);
      init(50, 0, _dEdb1); 
      init(100*50, 0, _dEdA2);
      init(100, 0, _dEdb2);
      init(10*100, 0, _dEdA3);
      init(10, 0, _dEdb3);
      for(k = 0; k < batch_size; ++k){
        float* x = train_x + 784 * index[j * batch_size + k];
        unsigned char t = train_y[j * batch_size + k];
        float* y3 = malloc(sizeof(float)*10);
        float* dEdA1 = malloc(sizeof(float)*50*784);
        float* dEdb1 = malloc(sizeof(float)*50);
        float* dEdA2 = malloc(sizeof(float)*100*50);
        float* dEdb2 = malloc(sizeof(float)*100);
        float* dEdA3 = malloc(sizeof(float)*10*100);
        float* dEdb3 = malloc(sizeof(float)*10);
        backward6(A1, b1, A2, b2, A3, b3, x, t, y3, dEdA1, dEdb1, dEdA2, dEdb2, dEdA3, dEdb3);
        add(50*784, dEdA1, _dEdA1);
        add(50, dEdb1, _dEdb1);
        add(100*50, dEdA2, _dEdA2);
        add(100, dEdb2, _dEdb2);
        add(10*100, dEdA3, _dEdA3);
        add(10, dEdb3, _dEdb3);
        free(y3);
        free(dEdA1);
        free(dEdb1);
        free(dEdA2);
        free(dEdb2);
        free(dEdA3);
        free(dEdb3);
      }
      scale(50*784, -learning_rate/batch_size, _dEdA1);
      scale(50, -learning_rate/batch_size, _dEdb1);
      scale(100*50, -learning_rate/batch_size, _dEdA2);
      scale(100, -learning_rate/batch_size, _dEdb2);
      scale(10*100, -learning_rate/batch_size, _dEdA3);
      scale(10, -learning_rate/batch_size, _dEdb3);
      add(50*784, _dEdA1, A1);
      add(50, _dEdb1, b1);
      add(100*50, _dEdA2, A2);
      add(100, _dEdb2, b2);
      add(10*100, _dEdA3, A3);
      add(10, _dEdb3, b3);
      free(_dEdA1);
      free(_dEdb1);
      free(_dEdA2);
      free(_dEdb2);
      free(_dEdA3);
      free(_dEdb3);
// ----mini batch----
    }
    int sum = 0;
    float entropy_sum = 0;
    for(j = 0; j < test_count; ++j){
      float* y3 = malloc(sizeof(float)*10);
      if(inference6(A1, b1, A2, b2, A3, b3, test_x + j * width * height, y3) == test_y[j]){
        ++sum;
      }
      entropy_sum += cross_entropy_error(y3, test_y[j]);
    }
    printf("%.2f%%\n", sum * 100.0 / test_count);
    printf("cross entropy error : %.2f\n",entropy_sum / test_count);
    // ----epoch----
  } 
  // ----e time epoch----
  free(index);

  save("fc1.dat", 50, 784, A1, b1);
  save("fc2.dat", 100, 50, A2, b2);
  save("fc3.dat", 10, 100, A3, b3);

  return 0;
}

void print(int m, int n, const float* x){
  int i,j;
  for(i = 0; i < m; ++i){
    for(j = 0; j < n; ++j){
      printf("%.4f ", x[i * n + j]);
    }
    printf("\n");
  }
}

// y = A * x (y, x : vector^m, A : matrix^m^n)
void mul(int m, int n, const float* A, const float* x, float* y){
  int i, j;
  for(i = 0; i < m; ++i){
    y[i] = 0;
    for(j = 0; j < n; ++j){
      y[i] += A[i * n + j] * x[j];
    }
  }
}


// y += b (y, b: vector^n)
void add(int m, const float* b, float* y){
  int i;
  for(i = 0; i < m; ++i){
    y[i] += b[i];
  }
}

// y = A * x + b (y, x : vector^n, b : vector^m, A : matrix^m^n)
void fc(int m, int n, const float* x, const float* A, const float* b, float* y){
  mul(m, n, A, x, y);
  add(m, b, y);
}


void relu(int n, const float* x, float* y){
  int i;
  for(i = 0; i < n; ++i){
    y[i] = (x[i] > 0) ? x[i] : 0;
  }
}

void softmax(int n, const float* x, float* y){
  int i;
  float exp_all = 0;
  float* z = malloc(sizeof(float) * n);
  for(i = 0; i < n; ++i){
    z[i] = x[i];
  }
  sort(n, z);
  for(i = 0; i < n; ++i){
    exp_all += exp(x[i] - z[n - 1]);
  }
  for(i = 0; i < n; ++i){
    y[i] = exp(x[i] - z[n - 1]) / exp_all;
  }
}


void change(float* l, int a, int b){
  float num = l[a];
  l[a] = l[b];
  l[b] = num;
}

void quick_sort_local(float* l, int large, int small) {
  float pivot = l[(large + small) / 2];
  int lag = large;
  int sml = small;

  while(1){
    while(l[lag] < pivot){
      ++lag;
    }
    while(l[sml] > pivot){
      --sml;
    }

    if(lag >= sml){
      break;
    }

    change(l, sml, lag);

    ++lag;
    --sml;
  }

  if(lag - large >= 2){
    quick_sort_local(l, large, lag - 1);
  }
  if(small - sml >= 2){
    quick_sort_local(l, sml + 1, small);
  }
}

void quick_sort(float* l, int length) {
  quick_sort_local(l, 0, length - 1);
}

void sort(int n, float*y){
  quick_sort(y, n);
}

int inference3(const float* A, const float* b, const float* x){
  float* y = malloc(sizeof(float)*10);
  //(1)
  fc(10, 784, x, A, b, y);
  //(2)
  relu(10, y, y);
  //(4)
  softmax(10, y, y);
  //index_return
  return largest_index(10, y);
}

int largest_index(int size, const float* y){
  float index[size];
  float y_[size];
  int i, j;
  for(i = 0; i < size; ++i){
    index[i] = (float)i;
    y_[i] = y[i];
  }
  for(i = 0; i < size; ++i){
    for(j = 0; j < size - (i + 1); ++j){
      if(y_[j] > y_[j + 1]){
        swap_array(y_, j);
        swap_array(index, j);
      }
    }
  }
  return index[size - 1];
}

void swap_array(float* array, int j){
  array[j] += array[j + 1];
  array[j + 1] = array[j] - array[j + 1];
  array[j] -= array[j + 1];
}

//dEdx : pointer to array whose size is equal to x
void softmaxwidthloss_bwd(int n, const float* y, unsigned char t, float* dEdx){
  unsigned int i;
  for(i = 0; i < n; ++i){
    if(i == t){
      dEdx[i] = y[i] - 1.0;
    }else{
      dEdx[i] = y[i];
    }
  }
}

void relu_bwd(int n, const float* x, const float* dEdy, float* dEdx){
  int i;
  for(i = 0; i < n; ++i){
    dEdx[i] = (x[i] > 0) ? dEdy[i] : 0;
  }
}

void fc_bwd(int m, int n, const float* x, const float* dEdy, const float* A, float* dEdA, float* dEdb, float* dEdx){
  int i, j;
  //(14)dEdA[i]
  for(i = 0; i < m; ++i){
    for(j = 0; j < n; ++j){
      dEdA[m * i + j] = dEdy[i] * x[i];
    }
  }

  //(15)dEdb
    for(i = 0; i < m; ++i){
      dEdb[i] = dEdy[i];
    }

  //(16)dEdx[i]
    for(i = 0; i < m; ++i){
      dEdx[i] = 0;
      for(j = 0; j < n; ++j){
        dEdx[i] += A[m * i + j] * dEdy[i];
      }
    }
}

void backward3(const float* A, const float* b, const float* x, unsigned char t, float* y, float* dEdA, float* dEdb){
  //Step.1
  int i;
  // x_y : for learning
  float* x_y = malloc(sizeof(float)*10);
  fc(10, 784, x, A, b, y);
  for(i = 0; i < 10; ++i){
    x_y[i] = y[i];
  }
//print(1, 10, y);
  relu(10, y, y);
//print(1, 10, y);
  softmax(10, y, y);
//print(1, 10, y);

  //Step.2
  float* dEdx = malloc(sizeof(float)*10);
  softmaxwidthloss_bwd(10, y, t, dEdx);
//print(1, 10, dEdx);


  //Step.3
  relu_bwd(10, x_y, dEdx, dEdx);
//print(1, 10, dEdx);

  //Step.4
  float* _dEdx = malloc(sizeof(float)*784);
  fc_bwd(10, 784, x_y, dEdx, A, dEdA, dEdb, _dEdx);

}

void shuffle(int n, int* x){
  int i, k;
  for(i = 0; i < n; ++i){
    k = rand() % n;
    if(k != i){
      x[i] += x[k];
      x[k] = x[i] - x[k];
      x[i] -= x[k];
    }
  }
}

float cross_entropy_error(const float* y, int t){
  return -log(y[t] + exp(1) - 7);
}

void scale(int n, float x, float* o){
  int i;
  for(i = 0; i < n; ++i){
    o[i] *= x;
  }
}

void init(int n, float x, float* o){
  int i;
  for(i = 0; i < n; ++i){
    o[i] = x;
  }
}

void rand_init(int n, float* o){
  int i;
  for(i = 0; i < n; ++i){
    o[i] = (float) rand() / (RAND_MAX / 2) - 1;
  }
}

// void learning(int e, int bs, float rate){
//   int i, j, k;
//   float* A = malloc(sizeof(float)*10*784);
//   float* b = malloc(sizeof(float)*10);
//   rand_init(10 * 784, A);
//   rand_init(10, b);
//   // ----e time epoch----
//   for(i = 0; i < e; ++i)
//     int* index = malloc(sizeof(int)*60000);
//     for(j = 0; j < 60000; ++i){
//       index[j] = j;
//     }
//     shuffle(60000, index);
//     // ----epoch----
//     for(j = 0; j < 60000 / bs; ++j){
//       float* _dEdA = malloc(sizeof(float)*784*10);
//       float* _dEdb = malloc(sizeof(float)*10);
//       // ----mini batch----
//       for(k = 0; k < bs; ++k){
//         float* x = train_x + 784 * index[j * bs + k];
//         float* y = malloc(sizeof(float)*10);
//         float* dEdA = malloc(sizeof(float)*784*10);
//         float* dEdb = malloc(sizeof(float)*10);
//         unsigned char t = train_y[j * bs + k];
//         backward3(A, b, x, t, y, dEdA, dEdb);
//         add(784*10, dEdA, _dEdA);
//         add(10, dEdb, _dEdb);
//       }
//       // ----mini batch----
//       scale(784*10, -rate/bs, _dEdA);
//       scale(10, -rate/bs, _dEdb);
//       add(784*10, _dEdA, A);
//       add(10, _dEdb, b);
//     }
//     // ----epoch----
//     int sum = 0;
//     for(j = 0; j < test_count; ++j){
//       if(inference3(A_784x10, b_784x10, test_x + j * width * height) == test_y[j]){
//         ++sum;
//       }
//     }
//     printf("%f%%\n", sum * 100.0 / test_count);
//     float sum2 = 0;
//    for(j = 0, j < test_count; ++j){
//      float* y = malloc(sizeof(float)*10);
//      //(1)
//      fc(10, 784, test_x + 784*j, A, b, y);
//      //(2)
//      relu(10, y, y);
//      //(4)
//      softmax(10, y, y);
//      sum2 += cross_entropy_error(y, test_y[j]);
//      printf("cross entropy error : %f \n",sum2 / test_count)
//    }
//  }
//  // ----e time epoch----
//}


/*
int e = 15;
  int bs = 600;
  float rate = 0.1;
  int i, j, k;
  float* A = malloc(sizeof(float)*10*784);
  float* b = malloc(sizeof(float)*10);
  int* index = malloc(sizeof(int)*60000);
  rand_init(10 * 784, A);
  rand_init(10, b);
  // ----e time epoch----
  for(i = 0; i < e; ++i){
    for(j = 0; j < 60000; ++j){
      index[j] = j;
    }
    printf("%d time epoch\n", i);
    shuffle(60000, index);
    // ----epoch----
    for(j = 0; j < 60000 / bs; ++j){
      float* _dEdA = malloc(sizeof(float)*784*10);
      float* _dEdb = malloc(sizeof(float)*10);
      // ----mini batch----
      for(k = 0; k < bs; ++k){
        float* x = train_x + 784 * index[j * bs + k];
        float* y = malloc(sizeof(float)*10);
        float* dEdA = malloc(sizeof(float)*784*10);
        float* dEdb = malloc(sizeof(float)*10);
        unsigned char t = train_y[j * bs + k];
        backward3(A, b, x, t, y, dEdA, dEdb);
        add(784*10, dEdA, _dEdA);
        add(10, dEdb, _dEdb);
      }
      // ----mini batch----
      scale(784*10, -rate/bs, _dEdA);
      scale(10, -rate/bs, _dEdb);
      add(784*10, _dEdA, A);
      add(10, _dEdb, b);
    }
    // ----epoch----
    int sum = 0;
    for(j = 0; j < test_count; ++j){
      if(inference3(A, b, test_x + j * width * height) == test_y[j]){
        ++sum;
      }
    }
    printf("%f%%\n", sum * 100.0 / test_count);
    float sum2 = 0;
    for(j = 0; j < test_count; ++j){
      float* y = malloc(sizeof(float)*10);
      //(1)
      fc(10, 784, test_x + 784*j, A, b, y);
      //(2)
      relu(10, y, y);
      //(4)
      softmax(10, y, y);
      sum2 += cross_entropy_error(y, test_y[j]);
    }
    printf("cross entropy error : %f \n",sum2 / test_count);
  } 
  // ----e time epoch----


*/


int inference6(const float* A1, const float* b1, const float* A2, const float* b2, const float* A3, const float* b3, const float*x, float* y3){
  //FC1 784 50
  float* y1 = malloc(sizeof(float)*50);
  fc(50, 784, x, A1, b1, y1);

  //ReLU1 50 50
  relu(50, y1, y1);

  //FC2 50 100
  float* y2 = malloc(sizeof(float)*100);
  fc(100, 50, y1, A2, b2, y2);

  //ReLU2 100 100
  relu(100, y2, y2);

  //FC3 100 10
  fc(10, 100, y2, A3, b3, y3);

  //Softmax 10 10
  softmax(10, y3, y3);

  //index return 
  return largest_index(10, y3);
}


void backward6(const float* A1, const float* b1, const float* A2, const float* b2, const float* A3, const float* b3, const float* x, unsigned char t, float* y3, float* dEdA1, float* dEdb1, float* dEdA2, float* dEdb2, float* dEdA3, float* dEdb3){
  int i;


  float* y1 = malloc(sizeof(float)*50);

  float* x_fc1 = malloc(sizeof(float)*784);

  float* x_relu1 = malloc(sizeof(float)*50);

  float* y2 = malloc(sizeof(float)*100);

  float* x_fc2 = malloc(sizeof(float)*50);

  float* x_relu2 = malloc(sizeof(float)*100);

  float* x_fc3 = malloc(sizeof(float)*100);

  float* dEdx1 = malloc(sizeof(float)*10);

  float* dEdx2 = malloc(sizeof(float)*100);

  float* dEdx3 = malloc(sizeof(float)*50);

  float* dEdx4 = malloc(sizeof(float)*784);






  //FC1 784 50
  for(i = 0; i < 784; ++i){
    x_fc1[i] = x[i];
  } 
  fc(50, 784, x, A1, b1, y1);

  //ReLU1 50 50
  for(i = 0; i < 50; ++i){
    x_relu1[i] = y1[i];
  }
  relu(50, y1, y1);

  //FC2 50 100
  for(i = 0; i < 50; ++i){
    x_fc2[i] = y1[i];
  }
  fc(100, 50, y1, A2, b2, y2);

  //ReLU2 100 100
  for(i = 0; i < 100; ++i){
    x_relu2[i] = y2[i];
  }
  relu(100, y2, y2);

  //FC3 100 10
  for(i = 0; i < 100; ++i){
    x_fc3[i] = y2[i];
  }
  fc(10, 100, y2, A3, b3, y3);

  //Softmax 10 10
  softmax(10, y3, y3); 

  softmaxwidthloss_bwd(10, y3, t, dEdx1);

  fc_bwd(10, 100, x_fc3, dEdx1, A3, dEdA3, dEdb3, dEdx2);

  relu_bwd(100, x_relu2, dEdx2, dEdx2);

  fc_bwd(100, 50, x_fc2, dEdx2, A2, dEdA2, dEdb2, dEdx3);
 
  relu_bwd(50, x_relu1, dEdx3, dEdx3);

  fc_bwd(50, 784, x_fc1, dEdx3, A1, dEdA1, dEdb1, dEdx4);

  free(y1);
  free(y2);
  free(x_fc1);
  free(x_fc2);
  free(x_fc3);
  free(x_relu1);
  free(x_relu2);
  free(dEdx1);
  free(dEdx2);
  free(dEdx3);
  free(dEdx4);
}

void load(const char* filename, int m, int n, float* A, float* b){
  int i;
  FILE* fp = NULL;
  float* buf = (float*)malloc(sizeof(float) * (m + 1) * n);
  fp = fopen(filename, "r");
  if(!fp){
    printf("Err : %s cannot open\n", filename);
    exit(1);
  }
  fread(buf, sizeof(float), (m + 1) * n, fp);
  for(i = 0; i < m * n; ++i){
    A[i] = buf[i];
  }
  for(i = 0; i < n; ++i){
    b[i] = buf[(m * n) + i];
  }
  fclose(fp);
}

void save(const char* filename, int m, int n, const float* A, const float* b){
  FILE *fp = NULL;
  fp = fopen(filename, "w");
  if(!fp){
    printf("Err : %s cannot open\n", filename);
    exit(1);
  }
  fwrite(A, sizeof(A[0]), m * n, fp);
  fwrite(b, sizeof(b[0]), n, fp); 
  fclose(fp);
}
