#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <iomanip>
using namespace std;

int getMax(int a, int b);
int getMax(vector<int>& tree, int l, int r);
void buildTree(vector<int>& tree, vector<int>& tmpArray);
void updateTree(vector<int>& tree, int position, int newValue);

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
	vector<int> startVector(n);
	vector<int> arrayForCompression(n, 0);
	int newN;
	_asm
	{
		bsr eax, n
		bsf ebx, n
		cmp eax, ebx
		jne first
		dec eax
		first :
		mov newN, eax
	}
	newN = 1 << newN + 1;
	vector<int> tree(2 * newN - 1, 0);
	vector<int> treePrev(2 * newN - 1, 0);
	vector<int> tmpArray(n);

	for (int i = 0; i < n; i++)
	{
		fscanf_s(inFile, "%d", &startVector[i]);
	}
	copy(startVector.begin(), startVector.end(), arrayForCompression.begin());
	sort(arrayForCompression.begin(), arrayForCompression.end(), less<int>());
	vector<int>::iterator it = unique(arrayForCompression.begin(), arrayForCompression.end());
	for (int i = 0; i < n; i++)
	{
		int position = lower_bound(arrayForCompression.begin(), it, startVector.at(i), less<int>()) - arrayForCompression.begin();
		startVector.at(i) = position;
	}


	for (int step = 0; step <= m; step++)
	{
		for (int i = 0; i < n; i++)
		{
			int res = getMax(tree, 0, startVector.at(i) - 1);
			int maxInPrevStep = getMax(treePrev, 0, i - 1);
			if (maxInPrevStep > res)
			{
				res = maxInPrevStep;
			}
			res++;
			tmpArray.at(i) = res;
			updateTree(tree, startVector.at(i), res);
		}
		buildTree(treePrev, tmpArray);
		fill(tree.begin(), tree.end(), 0);
	}
	

	int result = treePrev.at(0);
	fprintf_s(outFile, "%d", result);
	system("pause");
	return 0;
}

int getMax(int a, int b)
{
	return (a > b) ? a : b;
}

int getMax(vector<int>& tree, int l, int r)
{
	if (r < l)

	{
		return 0;
	}
	l++;
	r++;
	int result = 0;
	int n = tree.size() / 2;
	l += n - 1, r += n - 1;
	bool flag = true;
	while (l <= r)
	{
		// если l - правый сын своего родителя, 
		// учитываем его фундаментальный отрезок
		if (l % 2 == 0)
			result = getMax(result, tree[l++]);
		// если r - левый сын своего родителя, 
		// учитываем его фундаментальный отрезок
		if (r % 2 == 1)
			result = getMax(result, tree[r--]);
		// сдвигаем указатели на уровень выше
		l = (l - 1) / 2;
		r = (r - 1) / 2;
		if (!flag)
		{
			break;
		}
		if (l == r)
		{
			flag = false;
		}
	}
	return result;
}

void buildTree(vector<int>& tree, vector<int>& tmpArray)
{
	int newN = (tree.size() + 1) / 2;
	int n = tmpArray.size();
	fill(tree.begin(), tree.end(), 0);
	copy(tmpArray.begin(), tmpArray.end(), tree.begin() + newN - 1);
	for (int i = newN - 1; i > 0; i--)
	{
		tree[i - 1] = max(tree[2 * i], tree[2 * i - 1]);
	}
}

void updateTree(vector<int>& tree, int position, int newValue)
{
	int newN = (tree.size() + 1) / 2;
	position += newN - 1;
	tree[position] = newValue;
	position = (position - 1) / 2;
	while (position)
	{
		tree[position] = getMax(tree[2 * position + 1], tree[2 * position + 2]);
		position = (position - 1) / 2;
	}
	tree[0] = getMax(tree[1], tree[2]);
}
