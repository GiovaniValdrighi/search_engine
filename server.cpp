#include "lib/server_http.hpp"
#include "search_engine.cpp"

#include <algorithm>
#include <fstream>
#include <vector>

using namespace std;

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

int *res;
Trie trie;
int pos = 0;
int len_r = 0;

string show_more(int *&res, int len_r, int &pos){
	// abrimos mais 20 titulos a ser mostrados
	string aux;
	aux = "\n.. About " + to_string(len_r) + " results</br></br>";
	// exibimos os resultados a cada 020
	// geramos o link d abrir as paginas
	for(; pos < len_r; pos++){
		aux = aux + "<a href =javascript:query_link(\'cpp_server_open_page";
		aux = aux + to_string(pos) + "\'); > Title: ";
		aux = aux + get_title(pos, res) + "</a></br>";

		if(pos + 1 == len_r || (pos > 0 && (pos + 1) % 20 == 0)){
			aux += "</br></br><a href =javascript:query_more(\'cpp_server_show_more\'); >";
			aux += "... show 20 more >>";
			aux += "</a>";
			return aux;
		}
	}
	return aux;
}
string make_html(Trie trie, string query, int *&res, int &len_r, int &pos){
	string head = "";
	string aux = query + "####################";
	for(int i = 0; i < 20; i++) head = head + aux[i];

	if(head == "cpp_server_show_more"){
		// abrimos mais 20 titulos a ser mostrados
		return show_more(res, len_r, pos);
	}
	else if(head == "cpp_server_open_page"){
		// abrimos e retornamos o artigo de dadoId
		aux = "";
		for(int i = 20; i < query.size(); i++)
			aux = aux + query[i];

		cout << aux << endl;

		return "page: " + get_page(aux, res);
	}

	// o caso geral fazemos as buscas pela engine
	// recebemos e tratamos a entrada requisitada
	// separamos a contamos as palavras distintas
	aux = "";
	int size = 0;		// numero de palavras requisitadas
	string keys[30];	// array de palavras (keys) separadas
	query = query + ' ';
	for(char c: query){
		if((c == ' ' || c == '\t') && aux != ""){
			keys[size] = aux;
			aux = "";
			size++;
		}
		else aux += c;
	}

	// fazemos a busca das p�ginas e recebemos os
	// resultados diretamente em "res" e "len_r"

	auto start = chrono::steady_clock::now();
	trie.search_keys(keys, size, res, len_r, query);
	auto end = chrono::steady_clock::now();
	auto diff = end - start;

	// exibimos sugest�es de palavras semelhantes
	// caso em que o resultado da busca e vazio
	if(len_r == 0){
		aux = aux + "Sorry! No results were found.</br></br>";
                aux = aux + trie.sugest(query);

		return aux;
	}
	// exibimos os resultados e opcoes de links
	// caso em que o resultado da busca e cheio
	else{
		pos = 0;
		return show_more(res, len_r, pos);
	}

	return "Um erro inesperado ocorreu!";
}

int main() {
	// Carregamos a Trie contendo os dados
	trie = Trie("index.txt");

    // HTTP-server at port 8080 using 1 thread
    // Unless you do more heavy non-threaded processing in the resources,
    // 1 thread is usually faster than several threads
    HttpServer server;
    server.config.port = 8080;

    // GET-example for the path /query?text={text}
    // Responds with request-information
    server.resource["^/query$"]["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        // ADICIONAMOS OS NOSSOS C�DIGOS AQUI //
		stringstream stream;
        auto query_fields = request->parse_query_string();
        auto it = query_fields.find("text");
        // criamos html do div 'output'
        string html = make_html(trie, it->second, res, len_r, pos);
        cout << html;

        // Limpamos e enviamos o resultado
        stream << "{\"res\":\"";
        for(char c: html){
            if(c == "\n\t"[0]){stream << "</br>"; continue;}
            if(c == "\n\t"[1]){stream << "     "; continue;}
            stream << c;
        }
        stream << "\"}";
        // enviamos para o javascript
        response->write(stream);
    };

    // GET-example for the path /query?text={text}
    // Responds with request-information
    server.default_resource["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        auto path = "./web/"+request->path;

        SimpleWeb::CaseInsensitiveMultimap header;

        std::ifstream ifs(path, ifstream::in | ios::binary | ios::ate);
        auto length = ifs.tellg();
        ifs.seekg(0, ios::beg);
        stringstream stream;
        stream << ifs.rdbuf();

        header.emplace("Content-Length", to_string(length));
        response->write(header);
        response->write(stream.str().c_str(), length);
    };

    server.on_error = [](shared_ptr<HttpServer::Request> /*request*/, const SimpleWeb::error_code & /*ec*/) {
        // Handle errors here
        // Note that connection timeouts will also call this handle with ec set to SimpleWeb::errc::operation_canceled
    };

    thread server_thread([&server]() {
        // Start server
        server.start();
    });

    // Wait for server to start so that the client can connect
    this_thread::sleep_for(chrono::seconds(1));

    server_thread.join();
}
