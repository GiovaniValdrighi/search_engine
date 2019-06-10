#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
using namespace std;

struct Node{
	// "int size" é o tamanho do array de ids
	// "string key" é a palavra a ser inserida
	// "int *pages" é ponteiro para o primeiro id do array
	int *pages;
	int size;
	Node *pChild[37];
	
	Node(){
		size = 0;
		pages = nullptr;
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
	Trie(){pRoot = new Node();}
	Trie(string archive){
		pRoot = new Node();
		
		// Carregando dados de "index.txt"
		ifstream myfile;
		myfile.open(archive);
		
		// criando strings que receberao valor
		int counter = 0;
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
				counter ++;
				if(counter % 20000 == 0) cout << "Loaded " << counter << " words." << endl;
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
	void search_keys(string *keys, int size, int *&res, int &len_r, string &query){
		// "keys" uma lista das palavras requisitadas
		// "size" é numero de palavras na requisicao
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
			if(len_r == 0){ query = key; return; }
		}	
	}
	
	void search(string key, int &len, int *&res){
		// "string key" é a palavra buscada
		// buscamos iteradamente até o ultimo node
		Node *pNode = pRoot;
		for(char c: key){
			pNode = pNode->pChild[pNode->getIndex(c)];
			if(pNode == nullptr){len= 0; return;}
		}
		
		res = pNode->pages;
		len = pNode->size;
	}
	
	vector<string> suggestion(string key, int maxCost){
		//crio a primeira linha para o Leveshestein
		int currentRow[key.size()+1];
		for(int p = 0; p <= key.size()+1; p++) currentRow[p] = p;
		
		//crio o vetor de resultados, e a array palavra atual
		vector<string> result_words;
		int cur_word[key.size()+3];
		int pos = 0;
		
		//transformo a key em um array de inteiros
		int key_sz = key.size();
		int key_ar[key_sz];
		for(int i=0; i<key_sz;i++){
			key_ar[i] = get_index(key[i]);
		}
		//chamo a função recursiva para cada um dos filhos
		if (pRoot->pChild[key_ar[0]] != nullptr) recursive_sug(pRoot->pChild[key_ar[0]], key_ar[0], key_ar, key_sz, currentRow, result_words, cur_word, pos, maxCost);
		for( int i = 0; i < 37; i++){
			if ((pRoot->pChild[i] != nullptr) && (i != key_ar[0])){
				recursive_sug(pRoot->pChild[i], i, key_ar, key_sz, currentRow, result_words, cur_word, pos, maxCost);
			}
		}
		return result_words;
	}
	
	void recursive_sug(Node *pNode, int cur_letter, int key_ar[], int key_sz, int previousRow[],vector<string>& result_words, int cur_word[], int pos, int maxCost){
		//casos triviais
		if((pos == key_sz+3) || (result_words.size()>=6)){
			return;
		}
		
		cur_word[pos] = cur_letter;
		//calculo a nova linha de Leveshestein
		int columns = key_sz+1;
		int currentRow[columns];
		currentRow[0] = previousRow[0]+1;
		int replace_cost;
		int mini = 1000;
		for(int column=1; column<=columns; column++){
			if (key_ar[column-1] != cur_letter){
				replace_cost = 2;
			}else{
				replace_cost = 0;
			}
			currentRow[column] = min(currentRow[column-1]+1, min(previousRow[column]+1, previousRow[column-1]+replace_cost));
			if (currentRow[column] < mini) mini = currentRow[column];
		}
		
		
		//se o último elemento da linha do Leveshstein for menor que maxCost
		//e se esse pNode tiver pages, adiciono a palavra ao resultado
		if((currentRow[columns] <= maxCost) && (pNode->pages != nullptr)){
			result_words.push_back(get_string(cur_word, pos));	
		}
		
		//se o custo minimo da coluna for menor que o custo máximo
		//caminho para baixo nos filhos
		pos++;
		if(mini <= maxCost){
			for(int i = 0; i < 37; i++){
				if(pNode->pChild[i] != nullptr){
					recursive_sug(pNode->pChild[i], i, key_ar,key_sz, currentRow, result_words, cur_word, pos, maxCost);
				}
			}
		}	
	}
	
	string sugest(string input){
		vector<string> v = suggestion(input, 3);
		string aux = "Did you mean:\n" ;
		for(const auto& x: v){
			aux = aux + "     " + x + "\n";
		}
		return aux;
	}
	
	int get_index(char c){
		// cada letra "c" retorna o índice no array
		int index = 0;
		for(char i: "aeioubcdfghjklmnpqrstvwxyz0123456789"){
			if(c == i) return index;
			index += 1;
		}
		return index;
	}

	string get_string(int ar[], int pos){
		string alph = "aeioubcdfghjklmnpqrstvwxyz0123456789";
		string w;
		for(int i=0; i<=pos; i++){
			w = w + alph[ar[i]];
		}
		return w;
	}
};

string get_page(string page, int *res){
	// carregamos a página do artigo desejado
	string text = "";
	string aux = "";
	ifstream file;
	
	file.open("n_docs/doc_" + to_string(res[stoi(page) - 1] / 10000) + "/doc_" + to_string(res[stoi(page) - 1]));
	if(file) while(getline(file,aux)) text += "\n" + aux;
	file.close();
	
	return text;
}

string get_title(int page, int *res){
	// carregamos o titulo do artigo desejado
	string title = "";
	ifstream file;
	
	file.open("n_docs/doc_" + to_string(res[page] / 10000) + "/doc_" + to_string(res[page]));
	if(file) getline(file,title);
	file.close();
	
	return title;
}
