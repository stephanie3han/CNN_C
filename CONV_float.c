/*Tydf input_[4097][4]array will not cause lack of memory warnning*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "CONV_float.h"
#include "combined_arrays.h"
/*#include "_input.h"

#include "_1_CONV_weight.h"
#include "_2_CONV_weight.h"
#include "_3_CONV_weight.h"
#include "_4_CONV_weight.h"
#include "_5_CONV_weight.h"
#include "_6_CONV_weight.h"


#include "_1_CONV_bias.h"
#include "_2_CONV_bias.h"
#include "_3_CONV_bias.h"
#include "_4_CONV_bias.h"
#include "_5_CONV_bias.h"
#include "_6_CONV_bias.h"

#include "_1_FC_weight.h"
#include "_2_FC_weight.h"
#include "_3_FC_weight.h"

#include "_1_FC_bias.h"
#include "_2_FC_bias.h"
#include "_3_FC_bias.h"
*/

/*
#define DOUBLE
//#define FLOAT

#ifdef DOUBLE
typedef double Tydf;
#else FLOAT 
typedef float Tydf;
#endif
*/
// Leaky ReLU
Tydf leaky_relu(Tydf x) {
    return (x > 0) ? x : ALPHA * x;
}

Tydf relu(Tydf x){
    return (x > 0) ? x : 0;
}


// Convolutional
void convolution(
                 int const input_wide, //input[input_wide][input_length][1]
                 int const input_length, //
                 int const kernel_size,  //ROWS
                 int const kernel_number, //wide/COLS
                 int const output_length, //
                 Tydf input[input_wide][input_length][1], 
                 Tydf buffer[kernel_number][output_length][1], 
                 Tydf output[kernel_number][output_length][1], 
                 Tydf weight[kernel_number][kernel_size][input_wide], 
                 Tydf bias[1][kernel_number]){

    int d, l, w, col, row;
    Tydf sum=0;
    for(l=0; l<input_length-kernel_size+1;l++){  // control input   //input_length-kernel_size+1
        for (w=0; w< kernel_number; w++){   // control weight  
            for (col=0; col< input_wide; col++){  // control wide/COLS
                for (row=0; row<kernel_size; row++){                
                    sum+=input[col][l+row][0]*weight[w][row][col];                     
                }
            }
            sum=leaky_relu(sum+bias[0][w]);
            output[w][l][0]=sum;
            sum=0;
        }
        sum=0;
    }
}

     
// Maxpooling
void max_pooling(int const kernel_number,  
                 int const conv_output_length, 
                 int const pool_output_length, 
                 Tydf conv_output[kernel_number][conv_output_length][1], 
                 Tydf maxPool[kernel_number][pool_output_length][1]) {
                 
    for (int w = 0; w < kernel_number; w++) {
        for (int l = 0; l < pool_output_length; l++) {
            maxPool[w][l][0]=conv_output[w][2*l][0] > conv_output[w][2*l+1][0] ? conv_output[w][2*l][0] : conv_output[w][2*l+1][0];
        }
    }
}

// Global Average Pooling
void Avg_pooling(int const kernel_number,  
                 int const conv_output_length,
                 int const pool_output_length, 
                 Tydf conv_output[kernel_number][conv_output_length][1], 
                 Tydf avgPool[kernel_number][pool_output_length][1]) {
    Tydf sum=0.0;            
    for (int w = 0; w < kernel_number; w++) {
        for (int l = 0; l < conv_output_length; l++) {
            sum+=conv_output[w][l][0];
        }
        avgPool[w][0][0]=sum/conv_output_length;
        sum=0;
    }
}

void transpose_array(int const kernel_number,
                     int const length,
                     Tydf FC_input[kernel_number][length][1],
                     Tydf transpose[kernel_number][length]){
    for(int i=0; i<kernel_number; i++){
        for(int j=0; j<length;j++){
             transpose[i][j]=FC_input[i][j][0];
        }
    }              
}

void FC_connected(int const input_length, //
                  int const hidden_neuron_number,  //ROWS
                  Tydf input[input_length][1], 
                  Tydf output[hidden_neuron_number][1], 
                  Tydf weight[hidden_neuron_number][input_length], 
                  Tydf bias[hidden_neuron_number][1]){
    Tydf sum=0.0;           
    for(int i=0; i<hidden_neuron_number; i++){
        for(int j=0; j<input_length;j++){
            sum+=input[j][0]*weight[i][j];
        }
        sum=relu(sum+bias[i][0]);
        output[i][0]=sum;
        sum=0;
    }
}          


// Softmax
void softmax(int const output_size,
             Tydf input[output_size][1], 
             Tydf output[output_size]) {
    Tydf sum = 0.0;
    for (int i = 0; i < output_size; i++) {
        output[i] = exp(input[i][0]);
        sum += output[i];
    }
    for (int i = 0; i < output_size; i++) {
        output[i] /= sum;
    }
}


int main(int argc,char **argv) {

  int loop_=1;
  if (argc==2)
  sscanf(argv[1],"%d",&loop_);
  printf("loop_ = %d\n",loop_);
  while(loop_--){

    //CONV_2 and MaxPooling_2
//    printf("/*****************************now for CONV1*************************/\n\n\n\n");
    convolution(INPUT_WIDE, INPUT_LENGTH, 
               _1_CONV_ROWS, _1_CONV_NUMBER_ARRAY, _1_CONV_OUTPUT_LENGTH,
               &_input, &_1_CONV_Buffer, &_1_CONV_Output,
               &_1_CONV_weight, &_1_CONV_bias);
              
    max_pooling(_1_CONV_NUMBER_ARRAY,
                _1_CONV_OUTPUT_LENGTH,
                _1_MaxPool_LENGTH, 
                &_1_CONV_Output,
                _1_Maxpool_output);
    
    //CONV_2 and MaxPooling_2
//    printf("/*****************************now for CONV2*************************/\n\n\n\n");
    convolution(_1_CONV_NUMBER_ARRAY, _1_MaxPool_LENGTH, 
               _2_CONV_ROWS, _2_CONV_NUMBER_ARRAY, _2_CONV_OUTPUT_LENGTH,
               &_1_Maxpool_output, &_2_CONV_Buffer, &_2_CONV_Output,
               &_2_CONV_weight, &_2_CONV_bias);
  
    max_pooling(_2_CONV_NUMBER_ARRAY,
                _2_CONV_OUTPUT_LENGTH,
                _2_MaxPool_LENGTH, 
                &_2_CONV_Output,
                _2_Maxpool_output);
     
    //CONV_3 and MaxPooling_3
//    printf("/*****************************now for CONV3*************************/\n\n\n\n");
    convolution(_2_CONV_NUMBER_ARRAY, _2_MaxPool_LENGTH,
               _3_CONV_ROWS, _3_CONV_NUMBER_ARRAY, _3_CONV_OUTPUT_LENGTH,
               &_2_Maxpool_output, &_3_CONV_Buffer, &_3_CONV_Output,
               &_3_CONV_weight, &_3_CONV_bias);
  
    max_pooling(_3_CONV_NUMBER_ARRAY,
                _3_CONV_OUTPUT_LENGTH,
                _3_MaxPool_LENGTH, 
                &_3_CONV_Output,
                _3_Maxpool_output);

    //CONV_4 and MaxPooling_4
//    printf("/*****************************now for CONV4*************************/\n\n\n\n");
    convolution(_3_CONV_NUMBER_ARRAY, _3_MaxPool_LENGTH, 
               _4_CONV_ROWS, _4_CONV_NUMBER_ARRAY, _4_CONV_OUTPUT_LENGTH,
               &_3_Maxpool_output, &_4_CONV_Buffer, &_4_CONV_Output,
               &_4_CONV_weight, &_4_CONV_bias);
  
    max_pooling(_4_CONV_NUMBER_ARRAY,_4_CONV_OUTPUT_LENGTH,_4_MaxPool_LENGTH, &_4_CONV_Output,_4_Maxpool_output);
    
    //CONV_5 and MaxPooling_5
//    printf("/*****************************now for CONV5*************************/\n\n\n\n");
    convolution(_4_CONV_NUMBER_ARRAY, _4_MaxPool_LENGTH, 
               _5_CONV_ROWS, _5_CONV_NUMBER_ARRAY, _5_CONV_OUTPUT_LENGTH,
               &_4_Maxpool_output, &_5_CONV_Buffer, &_5_CONV_Output,
               &_5_CONV_weight, &_5_CONV_bias);
  
    max_pooling(_5_CONV_NUMBER_ARRAY,_5_CONV_OUTPUT_LENGTH,_5_MaxPool_LENGTH, &_5_CONV_Output,_5_Maxpool_output);
    
    //CONV_6 and Avg_pooling_6
//    printf("/*****************************now for CONV6*************************/\n\n\n\n");
    convolution(_5_CONV_NUMBER_ARRAY, _5_MaxPool_LENGTH, 
               _6_CONV_ROWS, _6_CONV_NUMBER_ARRAY, _6_CONV_OUTPUT_LENGTH,
               &_5_Maxpool_output, &_6_CONV_Buffer, &_6_CONV_Output,
               &_6_CONV_weight, &_6_CONV_bias);

    Avg_pooling(_6_CONV_NUMBER_ARRAY,  
             _6_CONV_OUTPUT_LENGTH,
             _6_GlobalPool_LENGTH, 
             &_6_CONV_Output, 
             &_6_Globalpool_output) ;
    
 transpose_array(_6_CONV_NUMBER_ARRAY,_6_GlobalPool_LENGTH, &_6_Globalpool_output, &FC_input); //make the first input size the same as output, for the repite of function
    

FC_connected(_6_CONV_NUMBER_ARRAY, _1_FC_ROWS, &FC_input, &_1_FC_Output, _1_FC_weight, _1_FC_bias);

FC_connected(_1_FC_ROWS, _2_FC_ROWS, &_1_FC_Output, &_2_FC_Output, _2_FC_weight, _2_FC_bias);

FC_connected(_2_FC_ROWS, _3_FC_ROWS, &_2_FC_Output, &_3_FC_Output, _3_FC_weight, _3_FC_bias);


softmax(OUTPUT_SIZE, &_3_FC_Output, &_output);
}
    printf("FC output  Softmax output\n");
for(int i=0; i<OUTPUT_SIZE; i++){
    printf("%f  %f \n", _3_FC_Output[i][0], _output[i]);
}
    return 0;
}

