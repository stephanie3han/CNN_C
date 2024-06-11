#include <stdio.h>

#define DOUBLE
//#define FLOAT

#ifdef DOUBLE
typedef double Tydf;
#else FLOAT 
typedef float Tydf;
#endif


#define INPUT_DEEP 1
#define INPUT_WIDE 1
#define INPUT_LENGTH 4097

#define _1_CONV_ROWS 6  
#define _1_CONV_COLS 1   
#define _1_CONV_NUMBER_ARRAY 4  
#define _1_CONV_OUTPUT_LENGTH  4092
#define _1_MaxPool_LENGTH 2046


#define _2_CONV_ROWS 5   
#define _2_CONV_COLS 4   
#define _2_CONV_NUMBER_ARRAY 4  
#define _2_CONV_OUTPUT_LENGTH  2042
#define _2_MaxPool_LENGTH 1021


#define _3_CONV_ROWS 4   
#define _3_CONV_COLS 4   
#define _3_CONV_NUMBER_ARRAY 10
#define _3_CONV_OUTPUT_LENGTH  1018
#define _3_MaxPool_LENGTH 509


#define _4_CONV_ROWS 4   
#define _4_CONV_COLS 10   
#define _4_CONV_NUMBER_ARRAY 10  
#define _4_CONV_OUTPUT_LENGTH  506
#define _4_MaxPool_LENGTH 253


#define _5_CONV_ROWS 4   
#define _5_CONV_COLS 10   
#define _5_CONV_NUMBER_ARRAY 15 
#define _5_CONV_OUTPUT_LENGTH  250
#define _5_MaxPool_LENGTH 125


#define _6_CONV_ROWS 6  
#define _6_CONV_COLS 15   
#define _6_CONV_NUMBER_ARRAY 8
#define _6_CONV_OUTPUT_LENGTH  120
#define _6_GlobalPool_LENGTH 1


#define _1_FC_ROWS 50  
#define _1_FC_COLS 8


#define _2_FC_ROWS 20  
#define _2_FC_COLS 50



#define _3_FC_ROWS 3  
#define _3_FC_COLS 20


#define OUTPUT_SIZE 3

#define ALPHA 0.01

extern Tydf _input[INPUT_DEEP][INPUT_LENGTH][INPUT_WIDE];

extern Tydf _1_CONV_weight[_1_CONV_NUMBER_ARRAY][_1_CONV_ROWS][_1_CONV_COLS];
extern Tydf _2_CONV_weight[_2_CONV_NUMBER_ARRAY][_2_CONV_ROWS][_2_CONV_COLS];
extern Tydf _3_CONV_weight[_3_CONV_NUMBER_ARRAY][_3_CONV_ROWS][_3_CONV_COLS];
extern Tydf _4_CONV_weight[_4_CONV_NUMBER_ARRAY][_4_CONV_ROWS][_4_CONV_COLS];
extern Tydf _5_CONV_weight[_5_CONV_NUMBER_ARRAY][_5_CONV_ROWS][_5_CONV_COLS];
extern Tydf _6_CONV_weight[_6_CONV_NUMBER_ARRAY][_6_CONV_ROWS][_6_CONV_COLS];


extern Tydf _1_CONV_bias[1][_1_CONV_NUMBER_ARRAY];
extern Tydf _2_CONV_bias[1][_2_CONV_NUMBER_ARRAY];
extern Tydf _3_CONV_bias[1][_3_CONV_NUMBER_ARRAY];
extern Tydf _4_CONV_bias[1][_4_CONV_NUMBER_ARRAY];
extern Tydf _5_CONV_bias[1][_5_CONV_NUMBER_ARRAY];
extern Tydf _6_CONV_bias[1][_6_CONV_NUMBER_ARRAY];

extern Tydf _1_FC_weight[_1_FC_ROWS][_1_FC_COLS];
extern Tydf _2_FC_weight[_2_FC_ROWS][_2_FC_COLS];
extern Tydf _3_FC_weight[_3_FC_ROWS][_3_FC_COLS];

extern Tydf _1_FC_bias[_1_FC_ROWS][1];
extern Tydf _2_FC_bias[_2_FC_ROWS][1];
extern Tydf _3_FC_bias[_3_FC_ROWS][1];

Tydf _1_CONV_Buffer[_1_CONV_NUMBER_ARRAY][_1_CONV_OUTPUT_LENGTH][1];
Tydf _1_CONV_Output[_1_CONV_NUMBER_ARRAY][_1_CONV_OUTPUT_LENGTH][1];
Tydf _1_Maxpool_output[_1_CONV_NUMBER_ARRAY][_1_MaxPool_LENGTH][1];

Tydf _2_CONV_Buffer[_2_CONV_NUMBER_ARRAY][_2_CONV_OUTPUT_LENGTH][1];
Tydf _2_CONV_Output[_2_CONV_NUMBER_ARRAY][_2_CONV_OUTPUT_LENGTH][1];
Tydf _2_Maxpool_output[_2_CONV_NUMBER_ARRAY][_2_MaxPool_LENGTH][1];

Tydf _3_CONV_Buffer[_3_CONV_NUMBER_ARRAY][_3_CONV_OUTPUT_LENGTH][1];
Tydf _3_CONV_Output[_3_CONV_NUMBER_ARRAY][_3_CONV_OUTPUT_LENGTH][1];
Tydf _3_Maxpool_output[_3_CONV_NUMBER_ARRAY][_3_MaxPool_LENGTH][1];

Tydf _4_CONV_Buffer[_4_CONV_NUMBER_ARRAY][_4_CONV_OUTPUT_LENGTH][1];
Tydf _4_CONV_Output[_4_CONV_NUMBER_ARRAY][_4_CONV_OUTPUT_LENGTH][1];
Tydf _4_Maxpool_output[_4_CONV_NUMBER_ARRAY][_4_MaxPool_LENGTH][1];

Tydf _5_CONV_Buffer[_5_CONV_NUMBER_ARRAY][_5_CONV_OUTPUT_LENGTH][1];
Tydf _5_CONV_Output[_5_CONV_NUMBER_ARRAY][_5_CONV_OUTPUT_LENGTH][1];
Tydf _5_Maxpool_output[_5_CONV_NUMBER_ARRAY][_5_MaxPool_LENGTH][1];

Tydf _6_CONV_Buffer[_6_CONV_NUMBER_ARRAY][_6_CONV_OUTPUT_LENGTH][1];
Tydf _6_CONV_Output[_6_CONV_NUMBER_ARRAY][_6_CONV_OUTPUT_LENGTH][1];
Tydf _6_Globalpool_output[_6_CONV_NUMBER_ARRAY][_6_GlobalPool_LENGTH][1];


Tydf FC_input[_6_CONV_NUMBER_ARRAY][_6_GlobalPool_LENGTH];


Tydf _1_FC_Output[_1_FC_ROWS][1];
Tydf _2_FC_Output[_2_FC_ROWS][1];
Tydf _3_FC_Output[_3_FC_ROWS][1];


Tydf _output[OUTPUT_SIZE];
int classification;



void read_tensor(int const rows, int const cols, int const numberArray, const char* filename, Tydf array[numberArray][rows][cols]){
    FILE *file;
    int i, j, k;
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Can't open file\n");
        //return 1;
    }
    for (i = 0; i < numberArray; i++) {
        for (j = 0; j < rows; j++) {
            for (k = 0; k < cols; k++) {
            fscanf(file, "%lf", &array[i][j][k]);// fprintf(pointFiche(w), nom Tableau)
            }
        }
    }
    fclose(file);
}



void read_array(int const R_, int const C_, const char* filename, Tydf array[R_][C_]){
    FILE *file;
    int i, j;
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Can't open file%s\n",filename);
        //return 1;
    }
    for (i = 0; i < R_; i++) {
        for (j = 0; j < C_; j++) {
            fscanf(file, "%lf", &array[i][j]);
        }
    }
    fclose(file);
}




