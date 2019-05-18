#include <iostream>
//#include <ifstream>
//#include <ofstream>
using namespace std;

struct Node{
	// array de ponteiros para proxima letra
	// e um lista com os "ids" das páginas
	//int pages[4] = {1,2,3,4};
	int pages;
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
	void insert(string key, int pages){
		// "string key" é a palavra a ser inserida
		// "int pages" é lista de "ids" das paginas
		Node **pNode = &pRoot;
		for(char c: key){
			pNode = &((*pNode)->pChild[(*pNode)->getIndex(c)]);
			if(*pNode == nullptr) (*pNode) = new Node();
		}
		(*pNode)->pages = pages;
		cout << " inserindo " << key << "... Resolver problema do array" << endl;
	}
	int search(string key){
		// "string key" é a palavra buscada
		Node *pNode = pRoot;
		for(char c: key){
			pNode = pNode->pChild[pNode->getIndex(c)];
			cout << pNode->getIndex(c) << endl;
		}
		cout << "Pegando em " << key << ".. Resolver problema com array" << endl;
		return pNode->pages;
	}
	private:		
};

// lemos e processamos o "index.txt"
void open_file(){
	
}
int main(){
	Trie trie;
	cout << "Trie inicializada..." << endl;
	
	int pages = 1;
	trie.insert("banana", pages);
	trie.insert("batata", pages);
	trie.insert("bauru", pages);
	trie.insert("baiana", pages);
	trie.insert("baderna", pages);
	trie.insert("baluarte", pages);
	trie.insert("batata", pages);
	
	cout << "valor: " << trie.search("batata") << endl;
}
