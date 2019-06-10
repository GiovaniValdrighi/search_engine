#include "search_engine.cpp"

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
	auto start = chrono::steady_clock::now();
	trie.search_keys(keys, size, res, len_r, query);
	auto end = chrono::steady_clock::now();
	auto diff = end - start;
	
	// exibimos sugestões de palavras semelhantes
	// caso em que o resultado da busca e vazio
	if(len_r == 0){
		cout << "Sorry! No results were found." << endl;
		cout << trie.sugest(query);
	}
	// exibimos os resultados e opcoes de usuario
	// caso em que o resultado da busca e cheio
	else{
		cout << "\n.. About " << len_r << " results in ";
		cout << fixed << showpoint << setprecision(10) << chrono::duration <double> (diff).count() << " seconds." << endl;
		
		for(int i = 0; i < len_r; i++){
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
	Trie trie = Trie("teste.txt");
	//trie.save("trie.srlz");
	//cout << "Salvo com sucesso!" << endl;
	
	//Trie trie;
	//trie.load("trie.srlz");
	//cout << "Lido com sucesso!" << endl;
	
	//show_menu(trie);
	while(true) show_menu(trie);
	
	return 0;
}
