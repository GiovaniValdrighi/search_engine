#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct Node{
	//pages � um ponteiro para o id do array de ids
	//size � o tamanho do array de ids
	//pChild[] � o array de ponteiros para filhos
	int *pages;
	int size;
	Node *pChild[27];
	
	Node(){
		for(int i = 0; i<26; i++) pChild[i] = nullptr; 
	}
	
	int getIndex(char c){
		// cada letra "c" retorna o �ndice no array
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
		// "string key" � a palavra a ser inserida
		// "int *pages" � o ponteiro para o primeiro id do array de ids
		// "int size" � o tamanho do array de ids
		Node **pNode = &pRoot;
		for(char c: key){
			pNode = &((*pNode)->pChild[(*pNode)->getIndex(c)]); //caminho para baixo
			if(*pNode == nullptr) (*pNode) = new Node(); //verifico a exist�ncia
		}
		//defino atributos
		(*pNode)->pages = pages; 
		(*pNode)->size = size;
		cout << " inserindo " << key << endl;
	}
	
	void search(string key){
		// "string key" � a palavra buscada
		Node *pNode = pRoot;
		for(char c: key){
			pNode = pNode->pChild[pNode->getIndex(c)]; //caminho at� o �ltimo n�
		}
		cout << "Pegando em node: " << key << endl;
		open_pages(pNode->pages, pNode->size);
		return;
	}
	private:
	
	void open_pages(int *pages, int size){
		//a fun��o percorre o array de ids
		//pages � um ponteiro apontando para o primeiro elemento do array
		//size � o tamanho do array de ids
		for(int i=0; i<size; i++){
			cout << *pages << ", ";
			pages++;
		}
	}		
};

// lemos e processamos o "index.txt"
void open_file(string archive, Trie trie){
	//abrindo a file
	ifstream myfile;
	myfile.open(archive);
	//criando as strings que v�o receber valor
	stringstream sppliter;
	string key, size_s, array_s;
	if(myfile){
		string line;
		while(getline(myfile,line)){
			//iterando por linha
			sppliter << line;
			sppliter >> key;
			sppliter >> size_s;
			int size = stoi(size_s);
			//criando o array das colunas restantes
			int pages[size];
			int array;
			for(int i=0; i < size; i++){
				sppliter >> array_s;
				cout << array_s << endl;
				array = stoi(array_s);
				pages[i] = array;
			}
			sppliter.clear();
			int *p = pages;
			trie.insert(key, p, size);
		}
	}
	myfile.close();
}

int main(){
	Trie trie;
	open_file("teste.txt", trie);
	trie.search("fredson");
	return 0;
}
