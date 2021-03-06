#include "pch.h"

#include<iostream>

#include<math.h>

#include<iomanip>

using namespace std;

int main()

{

	float A[15], B[15], C[15], Y[15], F[15];

	float a[15], b[15], Y1[15];

	int n, i;

	cout << "Print n\n";

	cin >> n;

	int m = 17; //number in the group

	int k = 1; // group number

	for (i = 0; i <= n; i++)

		Y[i] = i + 1; // fill Y

	for (i = 1; i <= n; i++) // Set matrix

		A[i] = -k; //Set A

	for (i = 0; i < n; i++) 

		B[i] = m + i - 1; //Set B

	C[0] = m;

	for (i = 1; i <= n; i++) 

		C[i] = m + k + i - 1; //Set C

	F[0] = C[0] * Y[0] + B[0] * Y[1]; 

	for (i = 1; i < n; i++) {

		F[i] = A[i] * Y[i - 1] + C[i] * Y[i] + B[i] * Y[i + 1]; // Set F

	}

	F[n] = A[n] * Y[n - 1] + C[n] * Y[n];

	for (int i = 0; i < n + 1; i++) {  // Print source matrix

		for (int j = 0; j < i - 1; j++) {

			cout << setw(5) << 0;

		}

		if (i != 0) {

			cout << setw(5) << A[i];

		}

		cout << setw(5) << C[i];

		if (i != n)

			cout << setw(5) << B[i];

		for (int j = i + 3 - (i == 0); j < n + 2; j++) {

			cout << setw(5) << 0;

		}

		cout << setw(10) << F[i] << endl;;

	}

	a[1] = -B[0] / C[0]; // Transform matrix with sweep method

	b[1] = F[0] / C[0];

	for (i = 2; i < n + 1; i++)

	{

		a[i] = -B[i - 1] / (C[i - 1] + A[i - 1] * a[i - 1]);

		b[i] = (F[i - 1] - A[i - 1] * b[i - 1]) / (C[i - 1] + A[i - 1] * a[i - 1]);

	}

	b[n + 1] = (F[n] - A[n] * b[n]) / (C[n] + A[n] * a[n]);

	// Print transformed matrix

	for (int i = 1; i < n + 2; i++) {

		for (int j = 1; j < i; j++) {

			cout << setw(8) << 0;

		}

		cout << setw(8) << 1;

		if (i != n + 1) {

			cout << setw(10) << -a[i];

			for (int j = i + 2; j < n + 2; j++) {

				cout << setw(8) << 0;

			}

		}

		cout << setw(12) << b[i] << endl;

	}

	Y1[n] = b[n + 1]; // Finding Y*

	for (i = n - 1; i >= 0; i--) {

		Y1[i] = a[i + 1] * Y1[i + 1] + b[i + 1];

	}

	cout << "y*=(";

	for (i = 0; i < n; i++)

	cout << Y1[i] << ", ";

	cout << Y1[n];

	cout << ")\n";

	float maxd = 0;

	for (i = 0; i < n; i++) { // finding ||y-y*||

		if (maxd < fabs(Y1[i] - Y[i]))

			maxd = fabs(Y1[i] - Y[i]);

	}

	cout << "Error = " << maxd / (n + 1) << endl;

	system("pause");

}