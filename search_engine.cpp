#include <string>
#include <time.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
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
	
	void print_sugest(string input){
		vector<string> v = suggestion(input, 3);
		cout << "Did you mean:" << endl;
		for(const auto& x: v){
			cout << "\t "<< x << endl;
		}
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
	
	private:
};

string get_page(string page, int *res){
	// carregamos a página do artigo desejado
	string text = "";
	string aux = "";
	ifstream file;
	
	file.open("docs/doc_" + to_string(res[stoi(page) - 1]));
	if(file) while(getline(file,aux)) text += "\n" + aux;
	file.close();
	
	return text;
}

string get_title(int page, int *res){
	// carregamos o titulo do artigo desejado
	string title = "";
	ifstream file;
	
	file.open("docs/doc_" + to_string(res[page]));
	if(file) getline(file,title);
	file.close();
	
	return title;
}

void show_menu(Trie trie){
	int size;			// numero de palavras requisitadas
	int *res;			// ponteiro para o array com ids
	int len_r;			// comprimento do array das paginas
	string aux;			
	string query;		// linha de entrada da requisicao
	string keys[30];	// array de palavras (keys) separadas
	
	string title = "";
	ifstream file;
	
	// recebemos e tratamos a entrada requisitada
	// separamos a contamos as palavras distintas
	size = 0;
	aux = "";
	cout << "\nEnter your query: ";
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
	// caso em que o resultado da busca e vazio
	if(len_r == 0){
		cout << "Sorry! No results were found." << endl;
		trie.print_sugest(query);
	}
	// exibimos os resultados e opcoes de usuario
	// caso em que o resultado da busca e cheio
	else{
		cout << "\n.. About " << len_r << " results";
		cout << " (" << time << " seconds)" << endl;
		
		for(int i = 0; i <= len_r; i++){
			// carregamos o titulo do artigo desejado
			cout << "[" << i + 1 << "] " << get_title(i, res) << endl;
			
			// exibimos resultaods em intervalos de 20
			if(i + 1 == len_r || i > 0 && (i + 1) % 20 == 0){
				cout << "\nDo you want to open any result?" << endl;
				cout << "    result number to open" << endl;
				cout << "    n  - No, thanks." << endl;
				if(i != len_r) cout << "    m  - 20 more." << endl;
				cout << "Option: ";
				cin >> aux;
				
				if(aux == "n") break;
				if(aux == "m") continue;
				// carregamos a página do artigo desejado
				cout << get_page(aux, res) << endl;
				break;
			}
		}
	}
	cin.ignore();
}

int main(){
	// o programa segue os padroes sugeridos
	Trie trie = Trie("index.txt");
	while(true) show_menu(trie);
	
	return 0;
}
