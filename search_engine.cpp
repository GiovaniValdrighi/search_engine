#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct Node{
	// "int size" é o tamanho do array de ids
	// "string key" é a palavra a ser inserida
	// "int *pages" é ponteiro para o primeiro id do array
	int *pages;
	int size;
	Node *pChild[27];
	
	Node(){
		for(int i = 0; i < 26; i++) pChild[i] = nullptr; 
	}
	
	int getIndex(char c){
		// cada letra "c" retorna o índice no array
		int index = 0;
		for(char i: "abcdefghijklmnopqrstuvwxyz"){
			if(c == i) return index;
			index += 1;
		}
		return index;
	}
};
class Trie{
	protected:
		int *index;
		Node *pRoot;
	public:
	// constructor da Trie
	Trie(string archive){
		pRoot = new Node();
		
		// Carregando dados de "index.txt"
		ifstream myfile;
		myfile.open(archive);
		
		// criando strings que receberao valor
		stringstream sppliter;
		string key, size_s, array_s;
		if(myfile){
			string line;
			while(getline(myfile,line)){
				// iterando por linha
				sppliter << line;
				sppliter >> key;
				sppliter >> size_s;
				int size = stoi(size_s);
				
				
				// criando o array das colunas restantes
				// capacidade para até 100 milhoes de ids
				index = new int[10000000];
				for(int i=0; i < size; i++){
					sppliter >> array_s;
					index[i] = stoi(array_s);
				}
				sppliter.clear();
				insert(key, index, size);
			}
		}
		myfile.close();
	}
	
	void insert(string key, int *pages, int size){
		// "int size" é o tamanho do array de ids
		// "string key" é a palavra a ser inserida
		// "int *pages" é ponteiro para o primeiro id do array
		Node **pNode = &pRoot;
		for(char c: key){
			pNode = &((*pNode)->pChild[(*pNode)->getIndex(c)]); //caminho para baixo
			if(*pNode == nullptr) (*pNode) = new Node(); 		//verifico a existência
		}
		// defino atributos
		(*pNode)->pages = pages;
		(*pNode)->size = size;
		cout << "Inserindo " << key << endl;
	}
	
	void search(string key){
		// "string key" é a palavra buscada
		Node *pNode = pRoot;
		for(char c: key){
			pNode = pNode->pChild[pNode->getIndex(c)]; //caminho até o último nó
		}
		cout << "Pegando em node: " << key << endl;
		open_pages(pNode->pages, pNode->size);
	}
	private:
	
	void open_pages(int *pages, int size){
		// a função percorre o array de ids
		// "int size" é o tamanho do array de ids
		// "int *pages" é ponteiro para o primeiro id do array
		for(int i=0; i<size; i++){
			cout << pages[i] << " ";
		}
	}		
};

int main(){
	Trie trie = Trie("teste.txt");
	//open_file("teste.txt", trie);
	
	string input;
	while(1){
		cout << endl << "\nBuscar (0 para sair): ";
		cin >> input;
		if(input == "0") break;
		trie.search(input);
	}
	return 0;
}
