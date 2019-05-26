#include <string>
#include <time.h>
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
	void search_keys(string *keys, int size, int *&res, int &len_r){
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
		}
		
	}
	void search(string key, int &len, int *&res){
		// "string key" é a palavra buscada
		// buscamos iteradamente até o ultimo node
		Node *pNode = pRoot;
		for(char c: key){
			if(pNode == nullptr){len= 0; return;}
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
	// o programa segue os padroes sugeridos
	Trie trie = Trie("index.txt");
	
	int size;			// numero de palavras requisitadas
	int *res;			// ponteiro para o array com ids
	int len_r;			// comprimento do array das paginas
	string aux;			
	string query;		// linha de entrada da requisicao
	string keys[30];	// array de palavras (keys) separadas
	
	while(true){
		// recebemos e tratamos a entrada requisitada
		// separamos a contamos as palavras distintas
		size = 0;
		aux = "";
		cout << "Enter your query: ";
		getline(cin, query);
		query += ' ';
		for(auto c: query){
			if((c == ' ' || c == '\t') && aux != ""){
				keys[size] = aux;
				aux = "";
				size++;
			}
			else aux += c;
		}
		
		// fazemos a busca das páginas e recebemos os 
		// resultados diretamente em "res" e "len_r"
		float time = clock();
		trie.search_keys(keys, size, res, len_r);
		time = (clock() - time) / CLOCKS_PER_SEC;
		
		// exibimos sugestões de palavras semelhantes
		if(len_r == 0){
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			cout << "Sorry! No results were found.";
			/////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		// exibimos os resultados e opcoes de usuario
		else{
			cout << "\n.. About " << len_r << " results";
			cout << " (" << time << " seconds)" << endl;
			
			for(int i = 0; i <= len_r; i++){
				// exibimos resultaods em intervalos de 20
				if(i == len_r || i > 0 && i % 20 == 0){
					cout << "\nDo you want to open any result?" << endl;
					cout << "    result number to open" << endl;
					cout << "    n  - No, thanks." << endl;
					if(i != len_r) cout << "    m  - 20 more." << endl;
					cout << "Option: ";
					cin >> aux;
					
					if(aux == "n") break;
					if(aux == "m") continue;
					//aux = open_page(stoi(aux));
					/////////////////////////////////////////////////////////////////////////////////////////////////////////
					aux = "Imagine a pagina\nImagine a pagina\n\n\n\n\n\n";
					/////////////////////////////////////////////////////////////////////////////////////////////////////////
					cout << aux;
					break;
				}
				//aux = open_title(res[i]);
				/////////////////////////////////////////////////////////////////////////////////////////////////////////
				aux = "imagine o titulo";
				/////////////////////////////////////////////////////////////////////////////////////////////////////////
				cout << "[" << i + 1 << "] " << aux << endl;
			}
		}
		
		cin.ignore();
	}
	return 0;
}
