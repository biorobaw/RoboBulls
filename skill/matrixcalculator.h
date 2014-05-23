#ifndef matrixmalculator_h_
#define matrixmalculator_h_

#define _USE_MATH_DEFINES


//****************************************************************************************************
//	Method names: multiply_(rows x columns for matrix A)_(rows x columns for matrix B)
//	Method parameters: (Matrix A, Matrix B, Matrix Result)
//	All parameters are passed by reference
//	Muhaimen Shamsi
//****************************************************************************************************

void multiply_3x3_3x1(double[3][3], double[3][1], double[3][1]);
void multiply_3x3_3x2(double[3][3], double[3][2], double[3][2]);
void multiply_3x2_2x1(double[3][2], double[2][1], double[3][1]);
void inverse_3x3(double[3][3], double[3][3]);
double remove_rounding_error(double input);


#endif
