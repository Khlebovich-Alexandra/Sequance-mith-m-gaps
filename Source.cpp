#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdio.h>
using namespace std;

int main()
{
	FILE *inFile;
	FILE *outFile;
	fopen_s(&outFile, "m-gaps.out", "w+t");
	if (!fopen_s(&inFile, "m-gaps.in", "r+t") == 0)
	{
		return 1;
	}
	int n;
	int m;
	fscanf_s(inFile, "%d %d", &n, &m);
	if (n - 1 == m)
	{
		fprintf_s(outFile, "%d", n);
		return 0;
	}
	int* startVector = new int[n];
	int** matrix = new int*[m + 2];
	for (int i = 0; i < m + 2; i++)
	{
		matrix[i] = new int[n];
	}
	for (int i = 0; i < n; i++)
	{
		fscanf_s(inFile, "%d", &startVector[i]);
	}
	for (int i = 0; i < n; i++)
	{
		matrix[0][i] = 0;
	}

	for (int step = 1; step < m + 2; step++)
	{
		matrix[step][n - 1] = 1;
		for (int i = n - 2; i >= 0; i--)
		{
			int res = 0;
			for (int j = n - 1; j > i; j--)
			{
				if (startVector[i] < startVector[j] && res < matrix[step][j])
				{
					res = matrix[step][j];
				}
			}
			for (int j = i + 1; j < n; j++)
			{
				if (res < matrix[step - 1][j])
				{
					res = matrix[step - 1][j];
				}
			}
			matrix[step][i] = res + 1;
		}
	}

	for (int i = 1; i < m + 2; i++)
	{
		for (int j = 0; j < n; j++)

		{
			cout << matrix[i][j] << ' ';
		}
		cout << endl;
	}

	int result = *max_element(matrix[m + 1], matrix[m + 1] + n);
	cout << result;
	fprintf_s(outFile, "%d", result);
	system("pause");
	return 0;
}
