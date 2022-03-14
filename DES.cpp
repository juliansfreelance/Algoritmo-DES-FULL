// DES.cpp
#include "DES.hpp"

// Tabla de reemplazo inicial
const int DES::ip[64] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9,  1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};


/* --------------------- Tabla de reemplazo para la generación de claves --------------------- */

// Mesa de reemplazo PC-1
const int DES::PC_1[56] = {
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};
// Mesa de reemplazo de compresión PC-2
const int DES::PC_2[48] = {
	14, 17, 11, 24,  1,  5,
	3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};


/* --------------------- Tabla de reemplazo para la función redonda Feistel --------------------- */

// Expandir tabla de reemplazo
const int DES::E[48] = {
	32,  1,  2,  3,  4,  5,
	4,  5,  6,  7,  8,  9,
	8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1
};
// P mesa de reemplazo
const int DES::P[32] = {
	16,  7, 20, 21,
	29, 12, 28, 17,
	1, 15, 23, 26,
	5, 18, 31, 10,
	2,  8, 24, 14,
	32, 27,  3,  9,
	19, 13, 30,  6,
	22, 11,  4, 25 };
// 8 S-BOX
int DES::S_BOX[8][4][16] = {
	{
		{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
		{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
		{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
		{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
	},
	{
		{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
		{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
		{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
		{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
	},
	{
		{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
		{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
		{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
		{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
	},
	{
		{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
		{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
		{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
		{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
	},
	{
		{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
		{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
		{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
		{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
	},
	{
		{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
		{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
		{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
		{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
	},
	{
		{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
		{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
		{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
		{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
	},
	{
		{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
		{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
		{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
		{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
	}
};


DES::DES(){}

// cifrado
bitset<64> DES::encrypt(const bitset<64>& plain, const bitset<64>& key){

	bitset<32> left; // graba la mitad superior
	bitset<32> right; // grabar la segunda mitad
	bitset<32> nextLeft; // Como una variable temporal intermedia para 16 rondas de iteración
	bitset<64> current; // Registre el resultado de cada reemplazo

	// Paso 1: reemplazo inicial de IP
	current = IP(plain);

	// Obtener L y R
	for(int i = 0; i < 32; i++)
		left[i] = current[i + 32];
	for(int i = 0; i < 32; i++)
		right[i] = current[i];

	// Paso 2: 16 iteraciones T
	generateSubKeys(key);// Generar subclave
	for(int i = 0; i < 16; i++){
		nextLeft = right;
		right = left ^ f(right, subKeys[i]);
		left = nextLeft;
	}

	// El tercer paso: reemplazo de intercambio
	for(int i = 0; i < 32; i++)
		current[i] = left[i];
	for(int i = 0; i < 32; i++)
		current[i + 32] = right[i];

	// El cuarto paso: reemplazo inverso de IP_1
	return IP_1(current);
}

// descifrar
bitset<64> DES::decrypt(const bitset<64>& plain, const bitset<64>& key){

	bitset<32> left; // graba la mitad superior
	bitset<32> right; // grabar la segunda mitad
	bitset<32> nextLeft; // Como una variable temporal intermedia para 16 rondas de iteraci�n
	bitset<64> current; // Registre el resultado de cada reemplazo

	// Paso 1: reemplazo inicial de IP
	current = IP(plain);

	// Obtener L y R
	for(int i = 0; i < 32; i++)
		left[i] = current[i + 32];
	for(int i = 0; i < 32; i++)
		right[i] = current[i];

	// Paso 2: 16 iteraciones T
	generateSubKeys(key);// Generar subclave
	for(int i = 0; i < 16; i++){
		nextLeft = right;
		right = left ^ f(right, subKeys[15 - i]); // La secuencia de programaci�n de subclaves se invierte cuando se cifra
		left = nextLeft;
	}

	// El tercer paso: reemplazo de intercambio
	for(int i = 0; i < 32; i++)
		current[i] = left[i];
	for(int i = 0; i < 32; i++)
		current[i + 32] = right[i];

	// El cuarto paso: reemplazo inverso de IP_1
	return IP_1(current);
}

// initial permutation
bitset<64> DES::IP(const bitset<64>& plain){
	bitset<64> M;
	for(int i = 0; i < 64; i++)
		M[i] = plain[ip[i] - 1];
	return M;
}

// Reemplazo inverso de IP
bitset<64> DES::IP_1(const bitset<64>& bits){
	bitset<64> cipher;
	// Construir la permutaci�n inversa de ip
	int ip_1[64];
	for(int i = 0; i < 64; i++)
		ip_1[ip[i] - 1] = i + 1;
	for(int i = 0; i < 64; i++)
		cipher[i] = bits[ip_1[i] - 1];
	return cipher;
}

// función redonda Feistel
bitset<32> DES::f(const bitset<32>& R, const bitset<48>& subKey){
	bitset<48> current;
	bitset<32> output;
	// El primer paso: reemplazo de expansión E 32-> 48
	for(int i = 0; i < 48; i++)
		current[i] = R[E[i] - 1];

	// Paso 2: XOR con la subclave
	current = current ^ subKey;

	// El tercer paso: dividido en 8 grupos, despu�s del reemplazo de S-BOX 6-> 4
	for(int i = 0; i < 8; i++){
		int head = 47 - i * 6; // El primer �ndice de cada grupo
		int row = current[head] * 2 + current[head - 5];
		int col = current[head - 1] * 8 + current[head - 2] * 4 + current[head - 3] * 2 + current[head - 4];
		bitset<4> result(S_BOX[i][row][col]);
		for(int j = 0; j < 4; j++)
			output[31 - i * 4 - j] = result[3 - j];
	}

	// Paso 4: reemplazo de P
	bitset<32> temp = output;
	for(int i = 0; i < 32; i++)
		output[i] = temp[P[i] - 1];
	return output;
}

// Función de desplazamiento a la izquierda del bucle
void leftShift(bitset<28>& bits, int shift){
	bitset<28> temp = bits;
	for (int i = 0; i < 28; ++i)
		bits[i] = temp[(i - shift + 28) % 28];
}

// Generar subclave
void DES::generateSubKeys(const bitset<64>& key){
	bitset<56> current;
	bitset<28> left;
	bitset<28> right;

	// El primer paso: reemplazo de PC-1 64-> 56
	for (int i = 0; i < 56; i++)
		current[55 - i] = key[64 - PC_1[i]];

	// Obtenga 28 d�gitos antes y despu�s
	for (int i = 0; i < 28; ++i)
		left[i] = current[i + 28];
	for (int i = 0; i < 28; ++i)
		right[i] = current[i];

	// El segundo paso: 16 rondas de desplazamiento a la izquierda y permutación de compresión para obtener 16 subclaves respectivamente
	int shiftBits[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1}; // El número de d�gitos desplazados a la izquierda
	for(int round = 0; round < 16; round++){
		// Girar a la izquierda
		leftShift(left, shiftBits[round]);
		leftShift(right, shiftBits[round]);

		// fusionar
		for (int i = 0; i < 28; ++i)
			current[i + 28] = left[i];
		for (int i = 0; i < 28; ++i)
			current[i] = right[i];

		// Compresi�n y reemplazo para obtener una subclave de 48 bits
		for(int j = 0; j < 48; j++)
			subKeys[round][47 - j] = current[56 - PC_2[j]];
	}
}

DES::~DES(){}

