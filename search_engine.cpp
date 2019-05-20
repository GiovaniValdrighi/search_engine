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
			pNode = &((*pNode)->pChild[(*pNode)->getIndex(c)]); //caminho para baixo
			if(*pNode == nullptr) (*pNode) = new Node(); 		//verifico a existência
		}
		// defino atributos
		(*pNode)->pages = pages;
		(*pNode)->size = size;
		cout << "Inserindo " << key << endl;
	}
	void search_keys(string *keys, int size){
		// "keys" uma lista de palavras requisitadas
		// "size" é o numero de palavras na requisicao
		int *res;
		int len_res;
		search(keys[0], len_res, res);
		open_pages(res, len_res);
		for(int i = 1; i < size; i++){
			// desejamos receber o retono e fazer a
			// intercessao eficiante entre as listas
			// supomos as listas de inteiros ordenadas
			
		}
	}
	void search(string key, int &len, int *&res){
		// "string key" é a palavra buscada
		// buscamos iteradamente até o ultimo node
		Node *pNode = pRoot;
		for(char c: key){
			pNode = pNode->pChild[pNode->getIndex(c)]; //caminho até o último nó
		}
		cout << "Pegando em node: " << key << endl;
		
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
	Trie trie = Trie("teste.txt");
	//open_file("teste.txt", trie);
	string s[3] = {"giovani"};
	trie.search_keys(s, 1);
	
	//string input;
	//while(1){
	//	cout << endl << "\nEnter your query (0 - exit): ";
	//	cin >> input;
	//	if(input == "0") break;
	//	trie.search(input);
	//}
	return 0;
}
