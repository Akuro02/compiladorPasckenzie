# CompiladorPasckenzie

## Autores
- Marco Antonio de Camargo - 10418309
- Nicolas Henriques de Almeida - 10418357
<p>Sala 06G - Mackenzie</p>

## Objetivo
O objetivo desse trabalho é implementar as fases de análise léxica e sintática de um compilador para linguagem  Pasckenzie (baseada na Linguagem Pascal).

---
## Funcionamento
- O usuario inicia o programa, indicando um arquivo escrito na linguagem Pasckenzie.
- O programa lê esse arquivo e conferir se está lexicamente e sintaticamente correto
    - Uma mensagem de confirmação será enviada caso não haja erros
    - Caso contrário, será exibida uma mensagem de erro indicando o tipo e a localização do problema.

---

## Compilação
### Usando makefile
```
make pasckenzie
```
- Isso irá criar o executável `compilador`
### Usando gcc manualmente
```
gcc -Wall -Wno-unused-result -g -Og pasckenzie.c -o compilador
```

## Execução
```
./compilador <nomeDoArquivo>
```
- Programa lê o arquivo indicado em `<nomeDoArquivo>` que deve estar escrito na linguagem Pasckenzie.


## Testes
- Após compilação, pode-se realizar um teste do programa utilizando um arquivo de exemplo
```
make test
```
- Isso irá executar o executável `compilador` utilizando como entrada o arquivo `programaTeste.txt`, que contem um exemplo de código sintaticamente correto na linguagem Pasckenzie.