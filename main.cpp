// main.cpp
#include <iostream>
#include <fstream>
#include <locale.h>
#include <windows.h>
#include "DES.cpp"

using namespace std;

void banner();
void menuPrincipal(bitset<64>);

void encriptar(bitset<64>);
void desencriptar(bitset<64>);

void encriptar_archivo(bitset<64>, string);
void desencriptar_archivo(bitset<64>, string);

string k("52013141");

bitset<64> charsToBitset(const char s[8])
{
	bitset<64> bits;
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 8; j++)
			bits[i * 8 + j] = ((s[7 - i] >> j) & 1);
	return bits;
}

char* BitsetToChars(const bitset<64> bits)
{
	static char s[8];
	bitset<8> c;
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			c[j] = bits[i * 8 + j];
		}
		s[7 - i] = c.to_ulong();
	}
	return s;
}

int main()
{
	// Establecer el idioma a español
    setlocale(LC_ALL, "es_ES.UTF-8"); // Cambiar locale - Suficiente para máquinas Linux
    SetConsoleCP(1252); // Cambiar STDIN -  Para máquinas Windows
    SetConsoleOutputCP(1252); // Cambiar STDOUT - Para máquinas Windows
    
    string k("52013191");
    bitset<64> key = charsToBitset(k.c_str());
    
	menuPrincipal(key);
	return 0;
}

void banner(){
	cout << "\n\t*********************************************" << endl;
    cout << "\t\tAlgoritmo de encriptación DES" << endl;
    cout << "\t© Zharick Lisbeth Alba - Julio Cesar Calderón"<< endl;
    cout << "\t*********************************************" << endl;
}

void menuPrincipal(bitset<64> key){
	int opcion;
    bool repetir = true;
    do
    {
        system("cls");
        // Texto del menú que se vera cada vez
        banner();
		cout << "\n\tLLave generada: \n\t" << key << endl;
		
        cout << "\n\t\t\tMenú de Opciones" << endl;
        cout << "\t\t\t----------------" << endl;
        cout << "\n\t1. Encriptar palabra" << endl;
        cout << "\t2. Desencriptar palabra" << endl;
        cout << "\n\t3. Encriptar .txt" << endl;
        cout << "\t4. Desencriptar .txt" << endl;
        cout << "\n\t5. Encriptar .jpg" << endl;
        cout << "\t6. Desencriptar .jpg" << endl;
        cout << "\n\t0. SALIR" << endl;

        cout << "\n\tIngrese una opción: ";
        cin >> opcion;

        switch (opcion)
        {
			case 1:
				encriptar(key);
				break;

			case 2:
				desencriptar(key);
				break;
			
			case 3:
				encriptar_archivo(key, "txt");
				break;
				
			case 4:
				desencriptar_archivo(key, "txt");
				break;
				
			case 5:
				encriptar_archivo(key, "jpg");
				break;
				
			case 6:
				desencriptar_archivo(key, "jpg");
				break;

			case 0:
				system("cls");
				banner();
				cout << "\n\t\t\t    ¡Gracias!" << endl;
				cout << "\t\t\t----------------" << endl;
				cout << "\n\t© Zharick Lisbeth Alba - Julio Cesar Calderón"<< endl;
				cout << "\t\t\t UCompensar2022" << endl;
				repetir = false;
			break;
		}
	} while (repetir);
}

void encriptar(bitset<64> llave){
	system("cls");
	banner();
	cout << "\n\t\t\tEncriptar Frase" << endl;
	cout << "\t\t\t-----------------" << endl;
	
	DES des;
	
	char frase[256];
	cout << "\n\tIngrese la frase a encriptar: ";
	cin.ignore();
	cin.getline(frase,256);
    string palabra(frase);
    
	bitset<64> plain = charsToBitset(palabra.c_str());
	bitset<64> cifrado = des.encrypt(plain, llave);

	cout << "\n\tFrase cifrada:\n\t" << cifrado << endl;

	string decifrado = BitsetToChars(des.decrypt(cifrado, llave));
	cout << "\n\tDecifrado: " << decifrado;

	cout << "\n\t\e[1mCopia los bits obtenidos de la Frase cifrada y comprueba...\e[0m" << endl;
	cout << "\n\t";
	system("pause>nul");
}

void desencriptar(bitset<64> llave){
	system("cls");
	banner();
	cout << "\n\t\t\t Desencriptar Palabra" << endl;
	cout << "\t\t\t----------------------" << endl;
	DES des;
	
	bitset<64> decifrado;
	cout << "\n\tIngrese los bit de la frase encriptada: ";
	cin >> decifrado;	
	
	string cifrado = BitsetToChars(des.decrypt(decifrado, llave));
	cout << "\n\tFrase: " << cifrado << endl;
	
	cout << "\n\t";
	system("pause>nul");
}

void encriptar_archivo(bitset<64> llave, string tipo){
	system("cls");
	banner();
	if(tipo == "txt") {
		cout << "\n\t\t\tEncriptar .txt" << endl;
	}else cout << "\n\t\t\tEncriptar .jpg" << endl;	
	cout << "\t\t\t-----------------" << endl;
	
	DES des;
	
	ifstream in;
	ofstream out;
	if(tipo == "txt") {
		cout << "\n\tEncriptaremos el archivo \'D://DES/archivo/frase.txt\'" << endl;
		in.open("D:/DES/archivo/frase.txt", ios::binary);
		out.open("D://DES/archivo/frase_cifrada.txt", ios::binary);
	}else{
		cout << "\n\tEncriptaremos la imagen \'D://DES/imagen/logo.jpg\'" << endl;
		in.open("D:/DES/imagen/logo.jpg", ios::binary);
		out.open("D://DES/imagen/logo_cifrado.txt", ios::binary);
	}
	
	bitset<64> file_data;
	if(in.is_open()){
		while(in.read((char*)&file_data, sizeof(file_data))){
			bitset<64> cipher = des.encrypt(file_data, llave);
			out.write((char*)&cipher, sizeof(cipher));
			file_data.reset(); // establecido en 0
		}
		cout << "\n\t";
		Sleep(3000);
		if(tipo == "txt") {
			cout << "\n\t¡Finalizado! \n\tPuedes leer el archivo cifrado en \'D://DES/archivo/frase_cifrada.txt\'." << endl;
		} else cout << "\n\t¡Finalizado! \n\tPuedes leer el archivo cifrado en \'D://DES/imagen/logo_cifrado.txt\'." << endl;
		in.close();
		out.close();
	}else {
		if(tipo == "txt") {
			cout << "No se puede abrir el archivo \'D://DES/archivo/frase.txt\'"<< endl;
		}else cout << "No se puede abrir la imagen \'D://DES/imagen/logo.jpg\'"<< endl;
	} 
	
	cout << "\n\t";
	system("pause>nul");
}

void desencriptar_archivo(bitset<64> llave, string tipo){
	system("cls");
	banner();
	if(tipo == "txt") {
		cout << "\n\t\t\tDesencriptar .txt" << endl;
	}else cout << "\n\t\t\ttDesencriptar .jpg" << endl;
	
	cout << "\t\t\t-----------------" << endl;
	
	DES des;
	
	ifstream in;
	ofstream out;
	if(tipo == "txt") {
		cout << "\n\tDesencriptaremos el archivo \'D://DES/archivo/frase_cifrado.txt\'..." << endl;
		in.open("D:/DES/archivo/frase_cifrada.txt", ios::binary);
		out.open("D://DES/archivo/frase_descifrada.txt", ios::binary);
	}else{
		cout << "\n\tDesencriptaremos el archivo \'D://DES/imagen/logo_cifrado.jpg\'" << endl;
		in.open("D:/DES/imagen/logo_cifrado.txt", ios::binary);
		out.open("D://DES/imagen/logo_descifrado.jpg", ios::binary);
	}
	
	
	bitset<64> file_data;
	if(in.is_open()){
		while(in.read((char*)&file_data, sizeof(file_data))){
			bitset<64> plain = des.decrypt(file_data, llave);
			out.write((char*)&plain, sizeof(plain));
			file_data.reset();  // establecido en 0
		}
		cout << "\n\t";
		Sleep(3000);
		if(tipo == "txt") {
			cout << "\n\t¡Finalizado! \n\tPuedes leer el archivo decifrado en \'D://DES/archivo/frase_descifrada.txt\'." << endl;
		} else cout << "\n\t¡Finalizado! \n\tPuedes abrir la imagen decifrada en \'D://DES/imagen/logo_descifrado.jpg\'." << endl;
		
		in.close();
		out.close();
	}else{
		if(tipo == "txt") {
			cout << "No se puede abrir el archivo \'D://DES/archivo/frase_cifrada.txt\'"<< endl;
		}else cout << "No se puede abrir el archivo \'D://DES/imagen/logo_cifrado.txt\'"<< endl;
	}
	
	cout << "\n\t";
	system("pause>nul");
}



