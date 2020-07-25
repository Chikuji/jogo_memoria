// --- Mapeamento de Hardware ---

#define  led1    10             //LED1 ligado no pino digital 10
#define  led2    11             //LED2 ligado no pino digital 11
#define  led3    12             //LED3 ligado no pino digital 12
#define  botao1   5             //Botão 1 ligado no pino digital 5
#define  botao2   6             //Botão 2 ligado no pino digital 6
#define  botao3   7             //Botão 3 ligado no pino digital 7
#define  piezo    8             //Transdutor ligado no pino digital 8

// --- Constantes Auxiliares ---

#define  nota_C4  262           //Frequência nota Dó  262Hz
#define  nota_E4  330           //Frequência nota Mí  330Hz
#define  nota_G4  392           //Frequência nota Sol 392Hz

// --- Funções Auxiliares ---

void output1();						//aciona LED1
void output2();						//aciona LED2
void output3();						//aciona LED3
void start();						//função que sinaliza o inicio ou novo nivel
void gameOver();					//termina o jogo 
void complete();					//verifica e avança um nivel
void check(byte number);			//verificar se o botão pressionado é o correto

// --- Variáveis Globais ---

byte memory[20];                  	//vetor com a quantidade máxima da dificuldade
byte count = 0x00;                	//variável para contagem dos botões pressionados
int level = 0x01;                 	//variável para indicar os níveis 
boolean wait = 0x00; 				//variável para aguardar o pressionar de um botão

// --- Configurações Iniciais ---

void setup()				
{      
 pinMode(led1, OUTPUT);           	//LED1 é saída digital
 pinMode(led2, OUTPUT);           	//LED2 é saída digital
 pinMode(led3, OUTPUT);           	//LED3 é saída digital
 pinMode(botao1, INPUT_PULLUP);   	//Botão 1 é entrada com resistor de pull-up interno
 pinMode(botao2, INPUT_PULLUP);   	//Botão 2 é entrada com resistor de pull-up interno
 pinMode(botao3, INPUT_PULLUP);   	//Botão 3 é entrada com resistor de pull-up interno
 pinMode(piezo, OUTPUT);          	//Transdutor é saída digital 
 digitalWrite(led1, LOW);         	//Desliga LED1 
 digitalWrite(led2, LOW);         	//Desliga LED2
 digitalWrite(led3, LOW);			//Desliga LED3
 noTone(piezo);                    	//Desliga piezo    
}//end setup

// --- Sinaliza o início de um novo nível ---

void start()  				
{  
  for(int j=0; j<2; j++)  					//inicio do jogo ou mudança de fase repete 2 vezes
  {
    digitalWrite(led1, HIGH); 
    digitalWrite(led2,  LOW);
    digitalWrite(led3,  LOW);
    delay(100);
    digitalWrite(led1,  LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3,  LOW);
    delay(100);
    digitalWrite(led1,  LOW); 
    digitalWrite(led2,  LOW);
    digitalWrite(led3, HIGH);
    delay(100);    
  } //end for  
  digitalWrite(led3, LOW); 				//apaga led3 (último a ser acionado)
  delay(500); //aguarda 500ms         
} //end start 

// --- Loop Infinito | Corpo do jogo ---

void loop() 				
{
     start();                  			//chama função de inicialização
	int rand = random(1,4); 			// gera valores randômicos entre 1 e 3
    memory[level-1] = rand;        		// vetor de memória recebe um número aleatório	
  for(int i = 0; i < level; i++)
  {         
    rand = memory[i];   	
    switch(rand)                     	//Switch para demonstrar qual botão deve ser pressionado
    {
       case 1: output1(); break;  		//caso num igual a 1, pisca led1
       case 2: output2(); break;  		//caso num igual a 2, pisca led2
       case 3: output3(); break;  		//caso num igual a 3, pisca led3      
    } //end switch    
    wait = 0x01;                     	//seta variável wait      
  } //end for 
  while(wait)                        	//aguarda até que um botão seja pressionado
  {  
        if(!digitalRead(botao1))     	//botão 1 pressionado?
        {                            	//sim...
            output1();               	//aciona LED1  
            check(0x01);             	//verifica se foi pressionado corretamente
        } //end if B1        
        if(!digitalRead(botao2))     	//botão 2 pressionado?
        {                            	//sim...        
            output2();               	//aciona LED2            
            check(0x02);             	//verifica se foi pressionado corretamente
        } //end if B2        
        if(!digitalRead(botao3))     	//botão 3 pressionado?
        {                            	//sim...        
            output3();               	//aciona LED            
            check(0x03);             	//verifica se foi pressionado corretamente
        } //end if B3          
  } //end while    
} //end loop 


void output1()  			// --- aciona LED1 com nota Dó
{
   digitalWrite(led1, HIGH);
   tone(piezo, nota_C4);
   delay(300);
   digitalWrite(led1, LOW);
   noTone(piezo);
   delay(300);   
} //end output1
void output2()  			// --- aciona LED2 com nota Mí
{
   digitalWrite(led2, HIGH);
   tone(piezo, nota_E4);
   delay(300);
   digitalWrite(led2, LOW);
   noTone(piezo);
   delay(300);   
} //end output2
void output3()  			// --- aciona LED3 com nota Sol
{
   digitalWrite(led3, HIGH);
   tone(piezo, nota_G4);
   delay(300);
   digitalWrite(led3, LOW);
   noTone(piezo);
   delay(300);
} //end output3

// --- Função que indica conclusão do nível ---

void complete()          	
{    
        if(level == count)  			//level é igual a count?
        {                   			//sim...
          wait  = 0x00;     			//limpa wait para invalidar laço while
          count = 0;     				//reinicia o contador
          level++;          			//avança para o próximo nível        
		}         
} //end complete

// --- Função que verificar se o botão pressionado é o correto ---

void check(byte number)  	
{
   if(memory[count] == number)    		//verifica posição de memória
   {          
         count++;                 		//Incrementa contador        
         complete();              		//Chama função que indica nível completo         
   } //end if
   
   else gameOver(); 					//se o botão for errado, reinicia
} //end check

// --- Função para reiniciar o jogo em caso de erro ---

void gameOver()    		
{   
   level = 0x01;   						//reinicia no nível 1
   wait  = 0x00;   						//limpa wait
   count = 0x00;   						//reinicia contador
   for(int i = 0; i < 20; i++) memory[i] = 0; //Limpa a memoria
   start();        						//chama função de início
