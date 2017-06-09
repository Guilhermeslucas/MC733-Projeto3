# MC733-Projeto3 Aceleração de Código

Neste projeto iremos acelerar uma implementação do Sistema Criptográfico Paillier. Esse algoritmo faz largo uso de exponenciação e aritmética modular, o que o torna
interessante para este projeto, dado que o MIPS não tem suporte nativo a essas operações.

Além de acelerar através da implementação das operações de exponenciação e aritmética modular no periférico, usando a lib GMP do C, que irá permanecer sempre no periférico, 
também vamos paralelizar alguns trechos do algoritmo, como a busca pelos primos usados para gerar o par de chaves e a encriptação/decriptação.

Para comparar {single core, multicore} x {acelerado, não acelerado} iremos inicialmente gerar um par de chaves, depois iremos fornecer um texto ao algoritmo e ele então usará cifrar o
texto com sua chave pública e depois decifrá-lo com a chave privada. Assim conseguiremos comparar o desempenho do algoritmo em diferentes configurações, utilizando tudo que ele oferece.
