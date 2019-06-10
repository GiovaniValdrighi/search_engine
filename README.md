# search_engine
Aplicação de busca à termos na biblioteca da Wikipédia. O kernel da busca desenvolvido em C++, interface em terminal e pequena GUI utilizando de aplicação Web.
Indexamos as páginas em uma estrutura de acesso eficiente de tal modo que um usuário deve ser capaz de entrar com uma série de palavras a serem pesquisadas na biblioteca, retornando:
  * A quantidade de documentos correpondendo a pesquisa
  * A lista de títulos correpondentes
  * O tempo de busca 

Além disso, disponibilizamos um vídeo explicando o processo criativo, tal como os objetivos e as soluções utilizadas durante implementação através do [link](https://youtu.be/9pxUYCLP2rA). Esse é um projeto inicialmente de âmbito educacional.


## Para a execução do projeto, você deve:
* Clonar o repositório em seu computador.
* Fazer download do arquivo index.txt e colocá-lo dentro do diretório. O arquivo pode ser obtido [aqui](https://gvmail-my.sharepoint.com/:t:/g/personal/b36998_fgv_edu_br/ETnGkLykGjVFjgYiSW3sDXgBH-Tt77mAsgROrvCWJdx9RQ?e=5nxJgl).
* Fazer download do arquivo n_docs.zip, e extraí-lo dentro do diretório. O arquivo pode ser obtido [aqui](https://gvmail-my.sharepoint.com/:u:/g/personal/b36998_fgv_edu_br/EanvboSjztVDnVPk7N6qhUEBcceSrr33V1TwbtDkHH6fFA?e=tLt66p).
* Com o terminal, caminhar para dentro do diretório e executar o comando:
`g++ -std=c++11 terminal.cpp -o engine -I/usr/local/include`
* Após a compilação, executar o comando:
`./engine`
* Após aguardar a construção da árvore, siga as instruções do terminal para realizar a pesquisa.

## Para a execução do projeto com o servidor, você deve:
A aplicação para servidor utiliza a biblioteca Boost v 1.69.
(Pule as 3 primeiras etapas se já foram feitas).
* Clonar o repositório em seu computador.
* Fazer download do arquivo index.txt e colocá-lo dentro do diretório. O arquivo pode ser obtido [aqui](https://gvmail-my.sharepoint.com/:t:/g/personal/b36998_fgv_edu_br/ETnGkLykGjVFjgYiSW3sDXgBH-Tt77mAsgROrvCWJdx9RQ?e=5nxJgl).
* Fazer download do arquivo n_docs.zip, e extraí-lo dentro do diretório. O arquivo pode ser obtido [aqui](https://gvmail-my.sharepoint.com/:u:/g/personal/b36998_fgv_edu_br/EanvboSjztVDnVPk7N6qhUEBcceSrr33V1TwbtDkHH6fFA?e=tLt66p).
* Com o terminal, caminhar para dentro do diretório e executar o comando:
`g++ -std=c++11 server.cpp -o engine_server -I/usr/local/include`
* Após a compilação, executar o comando:
`./engine_server`
* Após aguardar a construção da árvore, acesse no seu navegador a página `http://localhost:8080/index.html` e faça sua pesquisa.
