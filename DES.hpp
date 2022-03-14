// DES.hpp
#include <bitset>
using std::bitset;

class DES
{
public:
	DES();
	bitset<64> encrypt(const bitset<64>& plain, const bitset<64>& key);
	bitset<64> decrypt(const bitset<64>& plain, const bitset<64>& key);
	~DES();
private:
	bitset<48> subKeys[16];
	static const int ip[64]; // Tabla de reemplazo inicial

	// Tabla de sustitución utilizada para la generación de claves
	static const int PC_1[56]; // Mesa de reemplazo PC-1
	static const int PC_2[48]; // Mesa de reemplazo de compresión PC-2

	// La tabla de sustitución utilizada por la función redonda Feistel
	static const int E[48]; // Expandir tabla de reemplazo
	static const int P[32]; // P mesa de reemplazo
	static int S_BOX[8][4][16]; // 8 S-BOX

	// initial permutation
	bitset<64> IP(const bitset<64>& plain);
	// Reemplazo inverso de IP
	bitset<64> IP_1(const bitset<64>& bits);
	// función redonda Feistel
	bitset<32> f(const bitset<32>& right, const bitset<48>& subKey);
	// Generar subclave
	void generateSubKeys(const bitset<64>& key);
};