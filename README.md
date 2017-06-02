# MC733-Projeto3 Aceleração de Código

Neste projeto iremos acelerar uma implementação do algoritmo de criptografia assimeétrica RSA. Esse algoritmo faz largo uso de exponenciação e aritmética modular, o que o torna interessante para este projeto, dado que o MIPS não tem suporte nativo a essas operações.

Além de acelerar através da implementação das operações de exponenciação e aritmética modular no periférico, também vamos paralelizar alguns trechos do algoritmo, como a busca pelos coeficientes utilizados na geração do par de chaves pública/privada.

Para comparar {single core, multicore} x {acelerado, não acelerado} iremos inicialmente gerar um par de chaves, depois iremos fornecer um texto ao algoritmo e ele então usará cifrar o texto com sua chave pública e depois decifrá-lo com a chave privada. Assim conseguiremos comparar o desempenho do algoritmo em diferentes configurações, utilizando tudo que ele oferece.
