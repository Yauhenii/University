#include "stdafx.h"
#include <iostream>
#include <string>
#include <ctime>

void allocateMemMatrix(float**& A, int n);
void allocateMemVector(float*& X, int n);
void readVector(float*X, int n);
void readMatrixManual(float** A, int n);
void readMatrix(float** A, int n, float k);
void printVector(const char* name, float*X, int n);
void printMatrix(const char* name, float** A, int n);
void multipleMatrixVector(float **A, float *X, float *B, int n);
void subtractVector(float *X, float *A, float *B, int n);
void solveWithLDLTMethod(float **A, float *_X, float *B, int n);
void decomposeAtoLDLT(float **A, float *B, int n);
void reverseMoveU(float **A, float *X, float *B, int n);
void printError(float *X, float *_X, int n);
float get1Norm(float* X, int  n);

using namespace std;
int main()
{
	int n; //size
	cout << "Type matrix size" << endl;
	cin >> n;
	float *X; //prepare vector X
	allocateMemVector(X, n);
	float **A; //prepare matrix
	allocateMemMatrix(A, n);
	readVector(X, n); //get matrix and vector
	float k; //set k
	cout << "Type k - number of the group" << endl;
	cin >> k;
	for (int i = 0; i < 2; i++)
	{
		if (i == 1)
		{
			cout << "With k=0" << endl;
			k = 0;
		}
		readMatrix(A, n, k);
		float *B; //prepare vector B
		allocateMemVector(B, n);
		multipleMatrixVector(A, X, B, n);
		float *_X; //prepare vector _X = X*
		allocateMemVector(_X, n);
		solveWithLDLTMethod(A, _X, B, n);
		printVector("X", X, n); //conclusion
		printVector("X*", _X, n);
		printError(X, _X, n);
	}
	system("pause");
    return 0;
}
void allocateMemMatrix(float**& A, int n)
{
	A = new float*[n]; //i
	for (int j = 0; j < n; j++)
	{

		A[j] = new float[n]; //j
	}
}
void allocateMemVector(float*& X, int n)
{
	X = new float[n];
}
void readMatrixManual(float** A, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cin >> A[i][j];
		}
	}
}
void readMatrix(float** A, int n, float k)
{
	//---------------
	srand(time(0));
	for (int i = 0; i < n; i++) //set whole matrix 
	{
		for (int j = 0; j < n; j++)
		{
			A[i][j] = -(rand() % 5);
		}
	}
	//---------------
	A[0][0] = 0; //set diagonal element a11
	for (int j = 1; j < n; j++)
	{
		A[0][0] -= A[0][j];
	}
	float t = pow(10.0, k);
	A[0][0] += 1 / t;
	//---------------
	for (int i = 1; i < n; i++) //set diagonal elements aii, i>2
	{
		A[i][i] = 0;
		for (int j = 0; j < n; j++)
		{
			if (i != j)
			{
				A[i][i] -= A[i][j];
			}
		}
	}
	//---------------
}
void readVector(float*X, int n)
{
	float m;
	cout << "Type m - your number in the group" << endl;
	cin >> m;
	for (int i = 0; i < n; i++)
	{
		X[i] = m + i;
	}
}
void printMatrix(const char* name, float** A, int n)
{
	cout << "Matrix " << name << endl;
	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i < n; i++)
		{
			cout << A[j][i] << "\t\t";
		}
		cout << endl;
	}
}
void printVector(const char* name,float*X, int n)
{
	cout << "Vector " << name << endl;
	for (int i = 0; i < n; i++)
	{
		cout << X[i] << " ";
	}
	cout << endl;
}
void multipleMatrixVector(float **A, float *X, float *B, int n)
{
	for (int i = 0; i < n; i++)
	{
		B[i] = 0;
		for (int j = 0; j < n; j++)
		{
			B[i] += A[i][j] * X[j];
		}
	}
}

void decomposeAtoLDLT(float **A, float *B, int n)
{
	for (int k = 0; k < n - 1; k++) // k = 1, 2, … , n–1:
	{
		for (int i = k + 1; i < n; i++) //i = k + 1, k + 2, …, n:
		{
			float l = A[i][k] / A[k][k]; // temp l[i][k]= A[i][k]/A[k][k]
			B[i] = B[i] - l * B[k];
			for (int j = k; j < n; j++) //j = k + 1, k + 2, …, n:
			{
				A[i][j] = A[i][j] - l * A[k][j];
			}
		}
		if (k == 0) //print A after the first step
		{
			printMatrix("Matrix A after the first step", A, n);
		}
	}
}
void reverseMoveU(float **A, float *X, float *B, int n)
{
	X[n - 1] = B[n - 1] / A[n - 1][n - 1];
	for (int i = n - 2; i > -1; i--) //i=n–1, n–2,...,1.
	{
		int l = 0; //l = sum(A [i][j]X[j]) below 
		for (int j = i + 1; j < n; j++) //for j=i+1 to n
		{
			l += A[i][j] * X[j]; 
		}
		X[i] = (B[i] - l) / A[i][i];
	}
}
void solveWithLDLTMethod(float **A, float *_X, float *B, int n)
{
	decomposeAtoLDLT(A, B, n);
	reverseMoveU(A, _X, B, n);
}
void printError(float *X, float *_X,int n) 
{
	subtractVector(_X, X, _X, n);
	float x = get1Norm(X,n), _x = get1Norm(_X,n);
	cout << "Error is " << _x / x << endl;
}
void subtractVector(float *X, float *A, float *B, int n)
{
	for (int i = 0; i < n; i++)
	{
		X[i] = A[i] - B[i];
	}
}
float get1Norm(float* X, int  n)
{
	float max = X[0];
	for (int i = 1; i < n; i++)
	{
		if (max < X[i])
		{
			max = X[i];
		}
	}
	return max;
}