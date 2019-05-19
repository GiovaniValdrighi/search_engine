#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct Node{
	//pages é um ponteiro para o id do array de ids
	//size é o tamanho do array de ids
	//pChild[] é o array de ponteiros para filhos
	int *pages;
	int size;
	Node *pChild[27];
	
	Node(){
		for(int i = 0; i<26; i++) pChild[i] = nullptr; 
	}
	
	int getIndex(char c){
		// cada letra "c" retorna o índice no array
		int index = 0;
		for(char i: "abcdefghijklmnopqrstuvwxyz"){
			if(c == i) return index;
			index += 1;
		}
	}	
};

class Trie{
	protected:
		Node *pRoot;
	public:
	// constructor da Trie
	Trie(){
		pRoot = new Node();
	}
	
	void insert(string key, int *pages, int size){
		// "string key" é a palavra a ser inserida
		// "int *pages" é o ponteiro para o primeiro id do array de ids
		// "int size" é o tamanho do array de ids
		Node **pNode = &pRoot;
		for(char c: key){
			pNode = &((*pNode)->pChild[(*pNode)->getIndex(c)]); //caminho para baixo
			if(*pNode == nullptr) (*pNode) = new Node(); //verifico a existência
		}
		//defino atributos
		(*pNode)->pages = pages; 
		(*pNode)->size = size;
		cout << " inserindo " << key << endl;
	}
	
	void search(string key){
		// "string key" é a palavra buscada
		Node *pNode = pRoot;
		for(char c: key){
			pNode = pNode->pChild[pNode->getIndex(c)]; //caminho até o último nó
		}
		cout << "Pegando em node: " << key << endl;
		open_pages(pNode->pages, pNode->size);
		return;
	}
	private:
	
	void open_pages(int *pages, int size){
		//a função percorre o array de ids
		//pages é um ponteiro apontando para o primeiro elemento do array
		//size é o tamanho do array de ids
		for(int i=0; i<size; i++){
			cout << *pages << ", ";
			pages++;
		}
	}		
};

// lemos e processamos o "index.txt"
void open_file(string archive){
	//abrindo a file
	ifstream myfile;
	myfile.open(archive);
	//criando as strings que vão receber valor
	stringstream sppliter;
	string word, size_s, array_s;
	if(myfile){
		string line;
		while(getline(myfile,line)){
			sppliter << line;
			sppliter >> word;
			sppliter >> size_s;
			sppliter >> array_s;
			sppliter.clear();
			int size = stoi(size_s);
			
		}
	}
	myfile.close();
}
int main(){
	Trie trie;
	cout << "Trie inicializada..." << endl;
	ifstream myfile;
	myfile.open("teste.txt");
	stringstream sppliter;
	string a, b, c, line;
	getline(myfile,line);
	sppliter << line;
	sppliter >> a;
	sppliter >> b;
	sppliter >> c;
	cout << a << endl;
	cout << b << endl;
	cout << c << endl;
	
	return 0;
}
