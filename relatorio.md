* Felipe Dal Mas Eulálio RA: 155299
* Guilherme Lucas da Silva RA 155618 
* Vitor Falcão da Rocha RA: 157537

# Relatório - *Paillier Cryptosystem*

## Introdução

Com o intuito de utilizar os conhecimentos adiquiridos na atividade 4, foi proposto fazer a aceleração de algum algoritmo, com o uso de um processador MIPS multicore e periféricos. O aloritmo a ser acelerado foi o *Paillier Cryptosystem*, que encripta uma mensagem e depois decripta a mesma, fazendo diversas operações, principalmente a fim de achar números primos. 

## *Paillier Cryptosystem*
<!--FALCON'S PLACE TO SHINE-->

## Paralelização

O código foi paralelizado com dois cores, para que cada um deles pudesse gerar um dos números primos necessários para a execução do algoritmo.

## Periféricos

### Lock

Foi feito um periférico para executar um lock, a fim de simplificar o uso de nas implementação multicore do projeto. O periférico é simples, cuja implementação já fora feita na atividade 4.

### Criptográfico
Ao executar os testes de encriptação e desencriptação da chave **MC733**, notamos que esse processo era extremamente demorado, levando vários minutos para conseguir achar os dois primos necessários no processo, encriptar e desencriptar uma mensagem. Assim, partimos para a aceleração dessa parte do código. Como o código do periférico roda em **C++** que é uma linguagem conhecida por ser extremamente rápida, o programa rodou infinitamente mais rápido. Esse código era chamado a partir de uma escrita de memória no periférico, que disparava o código do acelerador.

## Resultados
### Execução base

Foi executado todo o código do algoritmo de Paillier dentro do MIPS, com isso os resultados obtidos estão a seguir:

| Tempo 	| Instruções |
|:-------:|:----------:|
|7m37.774s| 29139738154|

### Código paralelizado

Ao tentar executar o código, foram encontrados alguns problemas que não foram possíveis de serem resolvidos. Ao executar o simulador com 512MB de memória, ocorria um *overflow* na memória. Ao tentar subir esse valor para 1024MB, o *program counter* não conseguia acessar as posições de memória necessárias, dessa forma o código fica disponibilizado no diretório, no arquivo ```paillierMulticore.c```, apesar de não ter sido executado.

### Código acelerado com o periférico
Após realizar os testes com o periférico, notamos uma grande melhora nos resultados, pelo simples fato de alterar a arquitetura que esse código está rodando.

| Tempo 	| Instruções |
|:-------:|:----------:|
| 0m1.458s| 		70		|

## Conclusões
Esse projeto deixou claro como podemos melhorar muito o desempenho de programas criando periféricos específicos para uma função. Sabendo disso, existem vários hardwares feitos para tarefas muito específicas, como o que fizemos nesse projeto, encriptando e decriptando senhas, aumentando muito a segurança e rapidez de transações que necessitam desse tipo de operação.     Infelizmente, não conseguimos executar o código paralelo para coletar alguns resultados interessantes, devido a algumas limitações do simulador.