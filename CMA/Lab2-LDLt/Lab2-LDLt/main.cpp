
#include <iostream>
#include <string>
#include <ctime>

//Memory
void allocateMemMatrix(float**& A, int n);
void allocateMemVector(float*& X, int n);
//Matrix and vector
void readVector(float*X, int n);
void readMatrixManual(float** A, int n);
void readMatrix(float** A, int n, float k);
void printVector(const char* name, float*X, int n);
void printMatrix(const char* name, float** A, int n);
void multipleMatrixVector(float **A, float *X, float *B, int n);
void multipleMatrixMatrix(float **A, float** B, float** C, int n);
void subtractVector(float *X, float *A, float *B, int n);
//Solving
void solveWithLDLTMethod(float **A, float *_X, float *B, int n);
void decomposeAtoLDLT(float **A, float *B, int n);
void reverseMoveU(float **A, float *X, float *B, int n);
void reverseMoveL(float **A, float *X, float *B, int n);
void convertAtoDLT(float** A, int n);
//Error
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
	srand(time(0));
	for (int i = 0; i < n; i++) //set whole matrix 
	{
		for (int j = 0; j < n; j++)
		{
			if (i < j)
			{
				A[i][j] = -(rand() % 5);
				A[j][i] = A[i][j];
			}
		}
	}
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
			cout << A[j][i] << "\t";
		}
		cout << endl;
	}
}
void printVector(const char* name, float*X, int n)
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
void multipleMatrixMatrix(float **A, float **B, float** C, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			C[i][j] = 0;
			for (int p = 0; p < n; p++)
			{
				C[i][j] += A[i][p] * B[p][j];
			}
		}
	}
}
void decomposeAtoLDLT(float **A, float *B, int n)
{
	float *T;
	for (int k = 0; k < n - 1; k++) // k = 1, 2, … , n–1:
	{
		allocateMemVector(T, n);
		for (int i = k + 1; i < n; i++) //i = k + 1, k + 2, …, n:
		{
			T[i] = A[i][k]; // temp t[i]= A[i][k]
			A[i][k] /= A[k][k];
			for (int j = k + 1; j < i; j++) //j = k + 1, k + 2, …, n:
			{
				A[i][j] -= T[j] * A[i][k];
			}
		}
	}
}
void reverseMoveU(float **A, float *X, float *B, int n)
{
	
	X[n - 1] = B[n - 1] / A[n - 1][n - 1];
	for (int i = n - 2; i > -1; i--) //i=n–1, n–2,...,1.
	{
		float l = 0; //l = sum(A [i][j]X[j]) below 
		for (int j = i + 1; j < n; j++) //for j=i+1 to n
		{
			l += A[i][j] * X[j];
		}
		X[i] = (B[i] - l) / A[i][i];
	}
}
void reverseMoveL(float **A, float *X, float *B, int n)
{
	X[0] = B[0];
	for (int i = 1; i < n; i++) //i=2,3,...,n.
	{
		float l = 0; //l = sum(A [i][j]X[j]) below 
		for (int j = 0; j < i; j++) //for j=1 to i
		{
			l += A[i][j] * X[j];
		}
		X[i] = (B[i] - l);
	}
}
void convertAtoDLT(float** A, int n)
{
	cout << "Matrix " << "L" << endl;
	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i < n; i++)
		{
			if (i < j)
			cout << A[j][i] << "\t";
			else if (i==j)
				cout << "1" << "\t";
			else
				cout << "0" << "\t";
		}
		cout << endl;
	}
	//--------------------
	float** D; //set D
	allocateMemMatrix(D, n);
	for (int i = 0; i < n; i++) //set diagonal elements
	{
		for (int j = 0; j < n; j++)
		{
			if (i == j)
			{
				D[i][i] = A[i][i];
			}
			else
			{
				D[i][j] = 0;
			}
		}
	}
	printMatrix("D", D, n);
	//--------------------
	float **LT;
	allocateMemMatrix(LT, n);
	for (int i = 0; i < n; i++) //set whole matrix 
	{
		for (int j = 0; j < n; j++)
		{
			if (i <= j)
			{
				if (i == j)
				{
					LT[i][i] = 1;
				}
				else
				{ 
				LT[i][j] = A[j][i];
				LT[j][i] = 0;
				}
			}
		}
	}
	printMatrix("LT", LT, n);
	//--------------------
	multipleMatrixMatrix(D, LT, A, n);
}
void solveWithLDLTMethod(float **A, float *_X, float *B, int n)
{
	decomposeAtoLDLT(A, B, n);
	float* Y;
	allocateMemVector(Y, n); //Ly=b
	reverseMoveL(A, Y, B, n);
	convertAtoDLT(A, n);
	reverseMoveU(A, _X, Y, n);

}
void printError(float *X, float *_X, int n)
{
	subtractVector(_X, X, _X, n);
	float x = get1Norm(X, n), _x = get1Norm(_X, n);
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