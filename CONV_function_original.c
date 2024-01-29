/*已经测试过定义double input_[4097][4]大小的数组不会引起内存不足的警告*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CONV_function.h"

#define INPUT_SIZE 10  // 输入信号的长度
#define CONV_SIZE 3    // 卷积核的大小
#define POOL_SIZE 2    // 池化核的大小
#define FC_SIZE 5      // 全连接层神经元个数
#define OUTPUT_SIZE 3   // 输出类别个数

#define ALPHA 0.01
// Leaky ReLU激活函数
double leaky_relu(double x) {
    return (x > 0) ? x : ALPHA * x;
}

void convolution(int const input_wide, //input[input_wide][input_length][1]
                 int const input_length, //
                 int const kernel_size,  //ROWS
                 int const kernel_number, //wide/COLS
                 int const output_length, //
                 double input[input_wide][input_length][1], 
                 double buffer[kernel_number][output_length][1], 
                 double output[kernel_number][output_length][1], 
                 double weight[kernel_number][kernel_size][input_wide], 
                 double bias[1][kernel_number]){

    int l, w, col, row;
    double sum=0;
    
    
    for(l=0; l<input_length-kernel_size+1;l++){  // control input
        for (w=0; w< kernel_number; w++){   // control weight  
            for (col=0; col< input_wide; col++){  // control wide/COLS
                for (row=0; row<kernel_size; row++){
                
                    sum+=input[0][l+row][col]*weight[w][row][col];
                    
                    printf("%lf * %lf = %lf\n",input[0][l+row][col],weight[w][row][col],input[0][l+row][col]*weight[w][row][col]);
                      
                    //buffer[1][k][1]=input[input_wide][k][1]*weight[i][j][input_wide];
                }
                printf("col\n");
                //sleep(5);
            }
            printf("w\n");
            // in this moment, a input array and a weight array finish add, should save it in the buffer
            sum=leaky_relu(sum+bias[0][w]);
            printf("sum=%lf\n", sum);
            output[w][l][0]=sum;
            sum=0;
        }
        printf("l\n");
        printf("\n\n\n");
    }

}



// 卷积操作
void convolution(double input[], double output[], double weight[][CONV_SIZE], double bias[]) {
    for (int i = 0; i < INPUT_SIZE - CONV_SIZE + 1; i++) {
        output[i] = 0;
        for (int j = 0; j < CONV_SIZE; j++) {
            output[i] += input[i + j] * weight[0][j];
        }
        output[i] += bias[0];
        output[i] = leaky_relu(output[i]);
    }
}



// 最大池化操作
void max_pooling(double input[], double output[]) {
    for (int i = 0; i < INPUT_SIZE / POOL_SIZE; i++) {
        output[i] = input[i * POOL_SIZE];
        for (int j = 1; j < POOL_SIZE; j++) {
            if (input[i * POOL_SIZE + j] > output[i]) {
                output[i] = input[i * POOL_SIZE + j];
            }
        }
    }
}

// 全连接层
void fully_connected(double input[], double output[], double weight[][FC_SIZE], double bias[]) {
    for (int i = 0; i < FC_SIZE; i++) {
        output[i] = 0;
        for (int j = 0; j < INPUT_SIZE / POOL_SIZE; j++) {
            output[i] += input[j] * weight[j][i];
        }
        output[i] += bias[1];
        output[i] = leaky_relu(output[i]);
    }
}

// Softmax层
void softmax(double input[], double output[]) {
    double sum = 0.0;
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        output[i] = exp(input[i]);
        sum += output[i];
    }
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        output[i] /= sum;
    }
}

int main() {
    int i, j, k;

    //read parameters

    read_tensor(_1_CONV_ROWS,_1_CONV_COLS,_1_CONV_NUMBER_ARRAY,"TXTdata/CONV1C_6_1_4.txt",&_1_CONV_weight);   //read CONV1 weight

    read_array(1, _1_CONV_NUMBER_ARRAY,"TXTdata/CONV1B_1_4.txt",&_1_CONV_bias); //read CONV1 bias

    

    read_tensor(_2_CONV_ROWS,_2_CONV_COLS,_2_CONV_NUMBER_ARRAY,"TXTdata/CONV2C_5_4_4.txt",&_2_CONV_weight);   //read CONV2 weight

    read_array(1, _2_CONV_NUMBER_ARRAY,"TXTdata/CONV2B_1_4.txt",&_2_CONV_bias); //read CONV2 bias

    

    read_tensor(_3_CONV_ROWS,_3_CONV_COLS,_3_CONV_NUMBER_ARRAY,"TXTdata/CONV3C_4_4_10.txt",&_3_CONV_weight);   //read CONV3 weight

    read_array(1, _3_CONV_NUMBER_ARRAY,"TXTdata/CONV3B_1_10.txt",&_3_CONV_bias); //read CONV3 bias

    

    read_tensor(_4_CONV_ROWS,_4_CONV_COLS,_4_CONV_NUMBER_ARRAY,"TXTdata/CONV4C_4_10_10.txt",&_4_CONV_weight);   //read CONV4 weight

    read_array(1, _4_CONV_NUMBER_ARRAY,"TXTdata/CONV4B_1_10.txt",&_4_CONV_bias); //read CONV4 bias

    

    read_tensor(_5_CONV_ROWS,_5_CONV_COLS,_5_CONV_NUMBER_ARRAY,"TXTdata/CONV5C_4_10_15.txt",&_5_CONV_weight);   //read CONV5 weight

    read_array(1, _5_CONV_NUMBER_ARRAY,"TXTdata/CONV5B_1_15.txt",&_5_CONV_bias); //read CONV5 bias

    

    read_tensor(_6_CONV_ROWS,_6_CONV_COLS,_6_CONV_NUMBER_ARRAY,"TXTdata/CONV6C_6_15_8.txt",&_6_CONV_weight);   //read CONV6 weight

    read_array(1, _6_CONV_NUMBER_ARRAY,"TXTdata/CONV6B_1_8.txt",&_6_CONV_bias); //read CONV6 bias

    

    read_array(_1_FC_ROWS, _1_FC_COLS,"TXTdata/FC1C_50_8.txt",&_1_FC_weight); //read FC1 weight

    read_array(_1_FC_ROWS, 1,"TXTdata/FC1B_50_1.txt",&_1_FC_bias); //read FC1 bias

    

    read_array(_2_FC_ROWS, _2_FC_COLS,"TXTdata/FC2C_20_50.txt",&_2_FC_weight); //read FC2 weight

    read_array(_2_FC_ROWS, 1,"TXTdata/FC2B_20_1.txt",&_2_FC_bias); //read FC2 bias

    

    read_array(_3_FC_ROWS, _3_FC_COLS,"TXTdata/FC3C_3_20.txt",&_3_FC_weight); //read FC3 weight

    read_array(_3_FC_ROWS, 1,"TXTdata/FC3B_3_1.txt",&_3_FC_bias); //read FC3 bias



    read_tensor(4097,1,1,"TXTdata/O001.txt",&_input);
    
//test for array
//    printf("read array：\n");

/*    for (i = 0; i < _2_FC_ROWS; i++) {
        for (j = 0; j < _2_FC_COLS; j++) {
            printf("%lf ", _2_FC_weight[i][j]);
        }
        printf("\n");
    }
*/

/*
    for (i = 0; i < 1; i++) {
        for (j = 0; j < 4097; j++) {
            for (k = 0; k < 1; k++) {
            printf("%lf ", _input[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
   
*/
    // 输入信号
    double input[INPUT_SIZE] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};

    // 卷积层权重和偏差
    double conv_weight[][CONV_SIZE] = {{0.1, 0.2, 0.3},{0.1, 0.2, 0.3}};
    double conv_bias[1] = {0.5};

    // 池化层输出
    double pool_output[INPUT_SIZE / POOL_SIZE];

    // 全连接层权重和偏差
    double fc_weight[INPUT_SIZE / POOL_SIZE][FC_SIZE] = {{0.1, 0.2, 0.3, 0.4, 0.5},
                                                        {0.6, 0.7, 0.8, 0.9, 1.0}};
    double fc_bias[1] = {0.1};

    // Softmax层输出
    double softmax_output[OUTPUT_SIZE];

    // 进行前向传播
    double conv_output[INPUT_SIZE - CONV_SIZE + 1];
    convolution(input, conv_output, conv_weight, conv_bias);
    max_pooling(conv_output, pool_output);
    double fc_output[FC_SIZE];
    fully_connected(pool_output, fc_output, fc_weight, fc_bias);
    softmax(fc_output, softmax_output);

    // 打印结果
    printf("Softmax输出：");
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        printf("%lf ", softmax_output[i]);
    }
    printf("\n");

    return 0;
}

