#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

bool tab[SHRT_MAX];

void InitTab()
{
	memset(tab, 0, sizeof(tab));
	tab[0] = true;
	tab[1] = true;
	for (int i = 2; i < SHRT_MAX; i++) {
		if (tab[i]) continue;
		for (int j = i * 2; j < SHRT_MAX; j += i) {
			tab[j] = true;
		}
	}
}

int GetPrime(int min, int max)
{
	int num;
	do {
		num = rand() % (max - min) + min;
	} while (tab[num]);
	return num;
}

int GetP()
{
	return GetPrime(2, CHAR_MAX);
}

int GetQ()
{
	return GetPrime(2, CHAR_MAX);
}

int GetN(int P, int Q)
{
	return P * Q;
}

int GetF(int P, int Q)
{
	return (P - 1) * (Q - 1);
}

int GetE(int F)
{
	int E;
	int x, y, z;
	do {
		E = GetPrime(2, F - 1);
		x = F;
		y = E;
		while (y != 0) {
			z = y;
			y = x % y;
			x = z;
		}
	} while (x != 1);
	return E;
}

int GetD(int E, int F)
{
	int D = 1;
	while (E * D % F != 1) {
		D++;
	}
	return D;
}

int RSA(int data, int key, int mod) {
	int res = 1;
	for (int i = 0; i < key; i++) {
		res = (res * data) % mod;
	}
	return res;
}

int main()
{
	InitTab();
	srand(time(NULL));
	int P = GetP();
	printf("P = %d\n", P);
	int Q = GetQ();
	printf("Q = %d\n", Q);
	int N = GetN(P, Q);
	printf("N = %d\n", N);
	int F = GetF(P, Q);
	printf("F = %d\n", F);
	int E = GetE(F);
	printf("E = %d\n", E);
	int D = GetD(E, F);
	printf("D = %d\n", D);

	printf("PK: %d, %d\n", N, E);
	printf("SK: %d\n", D);

	int M;
	printf("初始明文：");
	scanf("%d", &M);
	int C = RSA(M, E, N);
	printf("密文：%d\n", C);
	M = RSA(C, D, N);
	printf("解密明文：%d\n", M);
	
	return 0;
}
