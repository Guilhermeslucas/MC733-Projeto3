* Felipe Dal Mas Eulálio RA: 155299
* Guilherme Lucas da Silva RA 155618 
* Vitor Falcão da Rocha RA: 157537

# Relatório - *Paillier Cryptosystem*

## Introdução
Com o intuito de utilizar os conhecimentos adiquiridos na atividade 4, foi proposto fazer a aceleração de algum algoritmo, com o uso de um processador MIPS multicore e periféricos. O aloritmo a ser acelerado foi o *Paillier Cryptosystem*, que encripta uma mensagem e depois decripta a mesma, fazendo diversas operações, principalemente a fim de achar números primos. 

## *Paillier Cryptosystem*

<!--## Paralelização
O código foi paralelizado com dois cores, para que cada um deles pudesse gerar um dos números primos necessários para a execução do algoritmo. -->

## Periféricos

<!--### Lock
Foi feito um periférico para executar um lock, a fim de simplificar o uso de multi processadores. O periférico é simples, quando algum valor era escrito nessa posição de memória, , dividido em duas funções, a ```writem```e a ```readm```, mostradas abaixo.

#### writem
``` c++
ac_tlm_rsp_status ac_tlm_peripheral::writem( const uint32_t &a , const uint32_t &d )
{
  cout << "addr: " << std::hex << a << ", data: " << d << endl;
  value = d;
  return SUCCESS;
}
```

#### readm
``` c++
ac_tlm_rsp_status ac_tlm_peripheral::readm( const uint32_t &a , uint32_t &d )
{
  d = value;
  value = 1;
  return SUCCESS;
}
```-->

### Criptográfico
