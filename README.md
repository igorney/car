<h1 align="center"> 🚗 CARRINHO DA COLETA! 🚗</h1>

<h4 align="center"> 	
	Universidade Federal do ABC
</h4>
<h4 align="center"> 	
	Projeto da disciplina <b>Computação Gráfica</b>.
</h4>

--- 

## Descrição do Projeto
<p>Construção de um projeto 2D interativo utilizando a biblioteca ABCg criada para o curso de Computação Gráfica  e as primitivas que são disponibilizadas pela biblioteca OpenGL. Trata-se de um jogo em que a movimentação do carro - objeto princial, faz a coleta dos itens da tela durante o tempo de dez (10) segundos. Ao final deste tempo, o jogo retorna a quantidade de itens que foram coletados. </p>

--- 

## Construção do Projeto 
<p> O projeto está organizado nas seguintes classes: 
<li> openglwindow: classe que fará a chamada das funções membros das outras classes. 
<li> car: classe que representa o carro, com todos seus atributos e funções. </li>
<li> items: classe que representa as formas do jogo, com todos seus atributos e funções. </li>
Além disso, temos a classe gamedata que contém as informações do estado do jogo.
O projeto também possui com os arquivos:
<li>  Inconsolata-UltraCondensedBlack.ttf: arquivo da fonte utilizada na mensagem de saída do jogo com a quantidade de objetos coletados. </li>
<li> /assets/objects.frag e /assets/objects.vert: arquivos com o vertex e fragment shader do carro e dos itens. </li>

No arquivo <b>CMakeLists.txt</b> declara-se o nome do projeto e os executaveis (<b>.cpp</b>).

As classe <b>openglwindow.cpp</b> é onde ocorre a chamada para as funções membros que encontramos nas outras classes: initializeGL, paintGL e terminateGL. 

Na função <b>paintUI</b> da classe <b>openglwindow.cpp</b> é onde ocorre a configuração da janela com o nome do jogo "Carrinho da Coleta" bem como a caixa para escolha da cor do fundo. Nela também é onde encontramos a impressao da variável contadora de itens coletados no jogo e sua exibição.

A classe <b>gamedata</b> possui os estados 'Playing' e 'Win'. Ao final do tempo do jogo, você sempre passa para o estado 'Win', a mudança é na quantidade de itens coletados. Este controle (da quantidade de itens) é feito atraves de uma variável que é incrementada a cada colisão. Além disso, nesta classe também temos as ações possíveis para o carro: Up, Down, Right e Left. 

O input dessas ações (Up, Down, Right e Left) localiza-se na função <b>handleEvent</b> da classe <b>openglwindow.cpp</b> e permite a utilização do mouse ou do teclado para o controle do jogo. 

A implementação das condições do jogo baseiam-se em duas funções principais: <b>checkCollisions </b>, que faz a coleta dos itens e <b>checkWinCondition</b> que faz o controle do tempo (dez segundos) alterando o estado para 'Win' ao término do tempo. 

A função <b>update</b> faz a checagem das colisões e condição de vencedor enquanto o estado é 'Playing'. Quando o estado é diferente disto e o tempo é superior a cinco (5) segundos, tempo em que é exibida a mensagem com a quantidade de objetos coletados, ocorre a chamada da função <b>restart</b> e a reinicialização da variável contadora. 

A função <b>restart</b> faz a inicialização do jogo com o estado 'Playing' e configuração inicial. 

Nas classes <b>items.cpp</b> e <b>car.cpp</b> é onde ocorre a definição dos objetos (do carro e das formas), bem como as cores, movimento de rotação, translação e velocidade. 

Nas classes <b>.hpp</b> é onde ocorre a declaração das variáveis e atributos. 

A classe <b>main</b> é a responsável pela inicialização da aplicação. Também é nela onde incluimos os cabeçavelhos das bibliotecas que permite que utilizemos suas classes e funções e onde configuramos a janela da aplicação.

---

## Como jogar
Para começar, escolha a sua cor preferida para o fundo da tela. \
Inicie o jogo, utilizando o mouse ou o teclado. \
 Utilizando o mouse: 
<li>Tecla direita: mover para cima. </li>
<li>Tecla esquerda: tender a parada. </li> 


Utilizando o teclado: 
Você pode utilizar o conjunto de teclas (w,s,a,d) ou as setas nas quatro direções:
<li>Tecla 'w' ou seta para cima: mover para cima.</li>
<li>Tecla 'a' ou seta esquerda: mover para esquerda.</li>
<li>Tecla 'd' ou seta direita: mover para direita.</li>
<li>Tecla 's' ou seta para baixo ou tecla espaço: tender a parada</li>

Agora é só divertir-se coletando a maior quantidade de itens que conseguir. \

<b>Dicas</b>: 
<li> Mantenha seu carro sempre na região visivel. </li>
<li> Mesmo após pressionar a tecla que tende a parada, os itens continuam a se mover. </li>
<li>O jogo é reiniciado a cada dez (10) segundos e é exibido na tela a quantidade de itens coletados na partida. </li> 

---

## Autores

<img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/48994130?v=4" width="100px;" alt=""/>
 <br />
 <b>Igor Santos Borges de Alencar RA: 11201811861 </b> 

---

 <img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/63355502?s=400&u=96d53188071a061d643b78620ba76d09c2e3bfb9&v=4" width="100px;" alt=""/>
 <br />
 <b>Jacqueline Marinho RA: 11201812013</b> 
 
 ---
 

### Professores

<i>Bruno Augusto Dorta Marques \
Harlen Costa Batagelo </i> 












</p>

</p>
