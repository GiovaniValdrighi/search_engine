#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

struct Node{
	// "int size" é o tamanho do array de ids
	// "string key" é a palavra a ser inserida
	// "int *pages" é ponteiro para o primeiro id do array
	int *pages;
	int size;
	Node *pChild[37];
	
	Node(){
		for(int i = 0; i < 37; i++) pChild[i] = nullptr; 
	}
	
	int getIndex(char c){
		// cada letra "c" retorna o índice no array
		int index = 0;
		for(char i: "aeioubcdfghjklmnpqrstvwxyz0123456789"){
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
				index = new int[size];
				for(int i=0; i < size; i++){
					sppliter >> array_s;
					index[i] = stoi(array_s);
				}
				sppliter.clear();
				insert(key, index, size);
			}
		}
		myfile.close();
		cout << "... Loading index done!" << endl;
	}
	
	void insert(string key, int *pages, int size){
		// "int size" é o tamanho do array de ids
		// "string key" é a palavra a ser inserida
		// "int *pages" é ponteiro para o primeiro id do array
		Node **pNode = &pRoot;
		for(char c: key){
			pNode = &((*pNode)->pChild[(*pNode)->getIndex(c)]); // caminho para baixo 
			if(*pNode == nullptr) (*pNode) = new Node(); 		// verifico a existência
		}
		// defino atributos
		(*pNode)->pages = pages;
		(*pNode)->size = size;
	}
	void intersection(int *&A, int &len_a, int *B, int len_b){
		// interceccao entre as listas A e B
		int len_r = 0;
		
		int i = 0;
		int j = 0;
		while(i < len_a && j < len_b){
			if(A[i] == B[j]){
				A[len_r] = A[i];
				
				i++;
				j++;
				len_r++;
			}
			else if(A[i] > B[j]) j++;
			else i++;
		}
		len_a = len_r;
	}
	void search_keys(string *keys, int size){
		// "keys" uma lista das palavras requisitadas
		// "size" é numero de palavras na requisicao
		int *res;
		int len_r;
		search(keys[0], len_r, res);
		
		int *aux;
		int len_a;
		string key;
		for(int i = 1; i < size; i++){
			// interseccionamos entre listas a cada laço
			// supomos as listas de inteiros ordenadas
			key = keys[i];
			search(key, len_a, aux);
			
			intersection(res, len_r, aux, len_a);
		}
		cout << "busca concluida" << endl;
		open_pages(res, len_r);
	}
	void search(string key, int &len, int *&res){
		// "string key" é a palavra buscada
		// buscamos iteradamente até o ultimo node
		Node *pNode = pRoot;
		for(char c: key){
			pNode = pNode->pChild[pNode->getIndex(c)];
		}
		
		res = pNode->pages;
		len = pNode->size;
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
	Trie trie = Trie("index.txt");
	
	string search;
	cin >> search;
	string s[1] = {search};
	trie.search_keys(s, 1);
	
	return 0;
}
