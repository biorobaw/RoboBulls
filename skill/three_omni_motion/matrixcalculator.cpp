#include "matrixcalculator.h"

//****************************************************************************************************
//	Method names: multiply_(rows x columns for matrix A)_(rows x columns for matrix B)
//	Method parameters: (Matrix A, Matrix B, Matrix Result)
//	All parameters are passed by reference
//  Muhaimen Shamsi
//****************************************************************************************************

//****************************************************************************************************
void multiply_3x3_3x1(double matrix_a[3][3], double matrix_b[3][1], double matrix_result[3][1])
{
    //Set result matrix to a null matrix
    for (int row = 0; row<3; row++)
    {
        for (int col = 0; col < 1; col++)
        {
            matrix_result[row][col] = 0.0;
        }
    }

    //Multiply the row of A with the column of B
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 1; col++)
        {
            for (int inner = 0; inner < 3; inner++)
            {
                matrix_result[row][col] += (matrix_a[row][inner] * matrix_b[inner][col]);
            }
        }
    }
}
//****************************************************************************************************

//****************************************************************************************************
void multiply_3x3_3x2(double matrix_a[3][3], double matrix_b[3][2], double matrix_result[3][2])
{
    //Set result matrix to a null matrix
    for (int row = 0; row<3; row++)
    {
        for (int col = 0; col < 2; col++)
        {
            matrix_result[row][col] = 0.0;
        }
    }

    //Multiply the row of A with the column of B
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 2; col++)
        {
            for (int inner = 0; inner < 3; inner++)
            {
                matrix_result[row][col] += (matrix_a[row][inner] * matrix_b[inner][col]);
            }
        }
    }
}
//****************************************************************************************************

//****************************************************************************************************
void multiply_3x2_2x1(double matrix_a[3][2], double matrix_b[2][1], double matrix_result[3][1])
{
    //Set result matrix to a null matrix
    for (int row = 0; row<3; row++)
    {
        for (int col = 0; col < 1; col++)
        {
            matrix_result[row][col] = 0.0;
        }
    }

    //Multiply the row of A with the column of B
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 1; col++)
        {
            for (int inner = 0; inner < 2; inner++)
            {
                matrix_result[row][col] += (matrix_a[row][inner] * matrix_b[inner][col]);
            }
        }
    }
}
//****************************************************************************************************

//****************************************************************************************************
double remove_rounding_error(double input)
{
    if (input < 0.00000000001 && input > -0.00000000001)
    {
        input = 0;
    }
    return input;
}
//****************************************************************************************************

//****************************************************************************************************
void inverse_3x3(double matrix[3][3], double inverse[3][3])
{
    //Set result matrix to a null matrix
    for (int row = 0; row<3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            inverse[row][col] = 0.0;
        }
    }

    //Assign row and column values to letters for easy reference
    double a = matrix[0][0];
    double b = matrix[0][1];
    double c = matrix[0][2];
    double d = matrix[1][0];
    double e = matrix[1][1];
    double f = matrix[1][2];
    double g = matrix[2][0];
    double h = matrix[2][1];
    double i = matrix[2][2];

    //Find inverse of determinant
    double det_inv = 1/(a*(e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g));

    //Calculate result
    inverse[0][0] = remove_rounding_error(det_inv*(e*i - f*h));
    inverse[0][1] = remove_rounding_error(det_inv*(c*h - b*i));
    inverse[0][2] = remove_rounding_error(det_inv*(b*f - c*e));
    inverse[1][0] = remove_rounding_error(det_inv*(f*g - d*i));
    inverse[1][1] = remove_rounding_error(det_inv*(a*i - c*g));
    inverse[1][2] = remove_rounding_error(det_inv*(c*d - a*f));
    inverse[2][0] = remove_rounding_error(det_inv*(d*h - e*g));
    inverse[2][1] = remove_rounding_error(det_inv*(b*g - a*h));
    inverse[2][2] = remove_rounding_error(det_inv*(a*e - b*d));
}
//****************************************************************************************************





