# CompiladorPasckenzie

## Autores
- Marco Antonio de Camargo - 10418309
- Nicolas Henriques de Almeida - 10418357
<p>Sala 06G - Mackenzie</p>

## Objetivo
O objetivo desse trabalho é implementar as fases de análise léxica, sintática, semântica e criação de código intermediário de um compilador para linguagem  Pasckenzie (baseada na Linguagem Pascal).

---
## Funcionamento
- O usuario inicia o programa, indicando um arquivo escrito na linguagem Pasckenzie.
- O programa lê esse arquivo e conferir se está lexicamente, sintaticamente e semânticamente correto
    - Caso sejam encontrados erros, será exibida uma mensagem de erro indicando o tipo e a localização do problema.
- Caso não existam erros, será impresso no console a tradução, em MEPA, do código. 

---

## Compilação
### Usando makefile
```
make pasckenzie
```
- Isso irá criar o executável `compilador`
### Usando gcc manualmente
```
gcc -Wall -Wno-unused-result -g -Og pasckenzie.c -o compilador -lm
```

## Execução
```
./compilador <Arquivo>
```
- Programa lê o arquivo indicado em `<Arquivo>` que deve estar escrito na linguagem Pasckenzie.


## Testes
- Após compilação, pode-se realizar um teste do programa utilizando um arquivo de exemplo
```
make test
```
- Isso irá executar o executável `compilador` utilizando como entrada o arquivo `programaTeste.txt`, que contem um exemplo de código sintaticamente correto na linguagem Pasckenzie.