Aqui está a versão corrigida e atualizada do teu README.md. Removi os caracteres que causavam o erro de parsing (os traços iniciais que o sistema confundia com metadados YAML) e adicionei as instruções específicas para utilizares o teu Makefile.
Custom Multithreaded HTTP Server Framework (C)

Uma infraestrutura de servidor Web de baixo nível escrita inteiramente em C, desenhada para demonstrar conceitos avançados de sistemas distribuídos, gestão de memória e concorrência.
🚀 Funcionalidades

    Arquitetura Multithreaded: Implementação de um Thread Pool customizado que utiliza pthread_mutex e pthread_cond para gerir múltiplas requisições em paralelo de forma eficiente.

    Parsing de Protocolo HTTP: Motor de processamento manual de pedidos HTTP, capaz de extrair Request Lines, Headers e o corpo das mensagens.

Sistema de Routing Dinâmico: Registo de rotas flexível associado a funções de callback, utilizando dicionários para uma procura rápida de URIs.

Motor de Templates: Funcionalidade de renderização que permite combinar e servir múltiplos ficheiros HTML estáticos como resposta ao cliente.

Abstração de Sockets: Camada de abstração sobre a API de sockets do sistema operativo para facilitar a configuração de domínios, protocolos e portas.

🛠️ Estrutura do Projeto

    Server.c: Lógica de baixo nível para criação e configuração de sockets TCP/IP.

HttpServer.c: Orquestrador principal que gere o ciclo de vida das threads e o despacho de rotas.

HttpRequest.c: Parser responsável por transformar strings de rede em estruturas de dados organizadas.

    ThreadPool.c: Gestor de concorrência que mantém um conjunto ativo de threads prontas para processar novos pedidos.

    DataStructures/: Implementações manuais de Listas Ligadas, Filas, Árvores Binárias e Dicionários para suporte à framework.

⚙️ Como Compilar e Executar

O projeto inclui um Makefile para automatizar o processo de compilação de todos os módulos e estruturas de dados.

    Compilar o projeto:
    No terminal, dentro da pasta raiz do projeto, executa:
    Bash

make

Este comando utiliza o gcc com as flags -Wall -Wextra -g para gerar o executável chamado server.

Executar o servidor:
Bash

./server

Limpar ficheiros temporários (.o):
Bash

make clean


💻 Exemplo de Utilização

O servidor permite registar rotas de forma simples no teu ficheiro main.c:
C

struct HttpServer server = http_server_constructor();

// Registo de uma rota para a página inicial
server.register_routes(&server, home_page_handler, "/", 1, GET);

// Lançamento do servidor
server.launch(&server);
