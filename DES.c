#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* 初始置换表 */
const unsigned char IP_TAB[64] = {
	58,50,42,34,26,18,10, 2,
	60,52,44,36,28,20,12, 4,
	62,54,46,38,30,22,14, 6,
	64,56,48,40,32,24,16, 8,
	57,49,41,33,25,17, 9, 1,
	59,51,43,35,27,19,11, 3,
	61,53,45,37,29,21,13, 5,
	63,55,47,39,31,23,15, 7 
};

/* 逆初始置换表 */
const unsigned char RIP_TAB[64] = { 
	40, 8,48,16,56,24,64,32,
	39, 7,47,15,55,23,63,31,
	38, 6,46,14,54,22,62,30,
	37, 5,45,13,53,21,61,29,
	36, 4,44,12,52,20,60,28,
	35, 3,43,11,51,19,59,27,
	34, 2,42,10,50,18,58,26,
	33, 1,41, 9,49,17,57,25	
};

/* 扩展/置换(E表) */
const unsigned char E_TAB[48] = {
	32, 1, 2, 3, 4, 5,
	 4, 5, 6, 7, 8, 9,
	 8, 9,10,11,12,13,
	12,13,14,15,16,17,
	16,17,18,19,20,21,
	20,21,22,23,24,25,
	24,25,26,27,28,29,
	28,29,30,31,32, 1
};

/* 代换/选择(S盒) */
const unsigned char S_BOX[8][4][16]={
	{
		{14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7},
		{ 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8},
		{ 4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0},
		{15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13}
	},
	{
		{15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10},
		{ 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5},
		{ 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15},
		{13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9}
	},
	{
		{10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8},
		{13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1},
		{13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7},
		{ 1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12}
	},
	{
		{ 7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15},
		{13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9},
		{10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4},
		{ 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14}
	},
	{
		{ 2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9},
		{14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6},
		{ 4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14},
		{11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3}
	},
	{
		{12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11},
		{10,15, 4, 2, 7,12, 0, 5, 6, 1,13,14, 0,11, 3, 8},
		{ 9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6},
		{ 4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13}
	},
	{
		{ 4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1},
		{13, 0,11, 7, 4, 0, 1,10,14, 3, 5,12, 2,15, 8, 6},
		{ 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2},
		{ 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12}
	},
	{
		{13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7},
		{ 1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2},
		{ 7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8},
		{ 2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11}
	}
};

/* 置换(P盒) */
const unsigned char P_BOX[32] = {
	16, 7,20,21,29,12,28,17,
	 1,15,23,26, 5,18,31,10,
	 2, 8,24,14,32,27, 3, 9,
	 19,13,30, 6,22,11, 4,25
};

/* 置换选择1表 */
const unsigned char PC1_TAB[56] = {
	57,49,41,33,25,17, 9, 1,
	58,50,42,34,26,18,10, 2,
	59,51,43,35,27,19,11, 3,
	60,52,44,36,63,55,47,39,
	31,23,15, 7,62,54,46,38,
	30,22,14, 6,61,53,45,37,
	29,21,13, 5,28,20,12, 4
};

/* 移位表 */
const unsigned char ROL_TAB[16] = {
	 1, 1, 2, 2, 2, 2, 2, 2,
	 1, 2, 2, 2, 2, 2, 2, 1
};

/* 置换选择2表 */
const unsigned char PC2_TAB[48]={
	14,17,11,24, 1, 5, 3,28,
	15, 6,21,10,23,19,12, 4,
	26, 8,16, 7,27,20,13, 2,
	41,52,31,37,47,55,30,40,
	51,34,33,48,44,49,39,56,
	34,53,46,42,50,36,29,32	
};

void KeyByteToBit(bool out[64], unsigned char in[7])
{
	bool buf[56];
	memset(buf, 0, sizeof(buf));
	for (int i = 0; i < 56; i++) {
		buf[i] = (in[i / 8] >> (7 - i % 8)) & 1;
	}
	for (int i = 0, j = 0; i < 64 && j < 56; i++) {
		if (i % 8 == 7) continue;
		out[i] = buf[j++];
		out[i / 8 * 8 + 7] ^= out[i];
	}
}

void Permute(bool * out, bool * in, const unsigned char * tab, size_t num)
{
	for (size_t i = 0; i < num; i++) {
		out[i] = in[tab[i] - 1];
	}
}

void S_Change(bool out[32], bool in[48])
{
	char row, column, value;
	for (int i = 0; i < 8; i++) {
		row = in[i * 6 + 5] * 2 + in[i * 6];
		column = in[i * 6 + 4] * 8 + in[i * 6 + 3] * 4 + in[i * 6 + 2] * 2 + in[i * 6 + 1];
		value = S_BOX[i][row][column];
		out[i * 4] = (value >> 3) & 1;
		out[i * 4 + 1] = (value >> 2) & 1;
		out[i * 4 + 2] = (value >> 1) & 1;
		out[i * 4 + 3] = value & 1;
	}
}

void InitSubKey(bool sub_key[16][48], bool key[64])
{
	bool buf[56];
	bool tmp[28];
	bool * C = buf;
	bool * D = buf + 28;
	
	// 置换选择(PC-1)
	Permute(buf, key, PC1_TAB, 56);
	// 子密钥生成	
	for (int i = 0; i < 16; i++) {
		int len = ROL_TAB[i];
		// C左循环移位
		memcpy(tmp, C + len, 28 - len);
		memcpy(tmp + 28 - len, C, len);
		memcpy(C, tmp, 28);
		// D左循环移位
		memcpy(tmp, D + len, 28 - len);
		memcpy(tmp + 28 - len, D, len);
		memcpy(D, tmp, 28);
		// 置换选择(PC-2)
		Permute(sub_key[i], buf, PC2_TAB, 48);
	}
}

void DES(bool data[64], bool key[64], bool mode)
{
	bool buf[64];
	bool * L0 = buf;
	bool * R0 = buf + 32;
	bool L1[32], R1[32];
	bool E[48], S[32];
	bool sub_key[16][48];
	
	// 密钥初始化
	InitSubKey(sub_key, key);
	// 初始置换
	Permute(buf, data, IP_TAB, 64);
	// 16轮加/解密迭代
	for (int i = 0; i < 16; i++) {
		memcpy(L1, R0, 32);
		// 扩展/置换(E表)
		Permute(E, R0, E_TAB, 48);
		// 48位异或
		for (int j = 0; j < 48; j++) {
			if (mode) E[j] ^= sub_key[i][j]; // 加密
			else E[j] ^= sub_key[15 - i][j]; // 解密
		}
		// 代换/选择(S盒)
		S_Change(S, E);
		// 置换(P盒)
		Permute(R1, S, P_BOX, 32);
		// 32位异或
		for (int j = 0; j < 32; j++) {
			R1[j] ^= L0[j];
		}
		memcpy(L0, L1, 32);
		memcpy(R0, R1, 32);
	}
	// 左右交换
	memcpy(L0, R1, 32);
	memcpy(R0, L1, 32);
	// 逆初始置换
	Permute(data, buf, RIP_TAB, 64);
}

void DES3(bool data[64], bool key[3][64], bool mode)
{
	if (mode) {
		DES(data, key[0], true);
		DES(data, key[1], false);
		DES(data, key[2], true);
	} else {
		DES(data, key[2], false);
		DES(data, key[1], true);
		DES(data, key[0], false);
	}
}

int main()
{
	unsigned char text[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
	bool data[64];
	for (int i = 0; i < 64; i++) {
		data[i] = (text[i / 8] >> (i % 8)) & 1;
	}
	printf("明文(16进制)：");
	for (int i = 0; i < 8; i++) {
		printf("%02X", text[i]);
	}
	printf("\n");

	/* 加密 */	
	bool key[3][64];
	for (int i = 0; i < 3; i++) {
		unsigned char tmp[8];
		printf("加密密钥%d:", i + 1);
		scanf("%s", tmp);
		KeyByteToBit(key[i], tmp);
	}
	DES3(data, key, true);
	
	printf("密文(16进制)：");
	memset(text, 0, 8);
	for (int i = 0; i < 64; i++) {
		text[i / 8] |= (unsigned char)data[i] << (i % 8);
	}
	for (int i = 0; i < 8; i++) {
		printf("%02X", text[i]);
	}
	printf("\n");

	/* 解密 */
	for (int i = 0; i < 3; i++) {
		unsigned char tmp[8];
		printf("解密密钥%d:", i + 1);
		scanf("%s", tmp);
		KeyByteToBit(key[i], tmp);
	}
	DES3(data, key, false);
	
	printf("明文(16进制)：");
	memset(text, 0, 8);
	for (int i = 0; i < 64; i++) {
		text[i / 8] |= (unsigned char)data[i] << (i % 8);
	}
	for (int i = 0; i < 8; i++) {
		printf("%02X", text[i]);
	}
	printf("\n");

	return 0;
}
