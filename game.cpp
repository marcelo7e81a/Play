/*
 * jogo.cpp
 *
 *  Created on: 24 de mai de 2017
 *      Author: Marcelo
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define MENU_A    1  /* menu option identifiers */
#define MENU_B    2
#define MENU_C    3


int pontos = 0;
char placar[50] = "";

GLfloat moviNuvem = 10.0f;

GLfloat xArvore = 1.0f;

int randQuantArvore1 = 1;
int randQuantArvore2 = 2;
int randQuantArvore3 = 3;

int velocidade = 15;


GLfloat yPersonagem = 0.0f;
GLfloat xPersonagem = -0.5f;
GLfloat movimento = 0.0f;
int direcao = 1;

GLfloat pulo = 0.0f;
int ativarPulo = 0;
int chao = 1;

int pause = -1;



void circulo(double r, double g, double b, int inicio, int fim)
{
	float pi = 3.14159265358979323846;
	glBegin(GL_POLYGON);
		glColor3f(r, g, b);
		for(int i = inicio; i <fim; i++)
		{
			double x = (double) cos(i*2*pi/100);
			double y = (double) sin(i*2*pi/100);
			glVertex2f(x,y);
		}
	glEnd();
}


void perna(GLfloat movi)												//INICIO DESENHO PERSONAGEM
{
	GLfloat aux;

	glPushMatrix();
	glBegin(GL_QUADS);//perna
		glColor3f(0.3f, 0.3f, 0.9f);
		glVertex2f(xPersonagem-0.01, yPersonagem+0.04f);
		glVertex2f(xPersonagem, yPersonagem+0.04f);
																					//Movimenta só a parte de baixo do braço
		aux = xPersonagem;														//Grava a posição do personagem em um aux para poder voltar depois
		xPersonagem = xPersonagem + movi;										//Translata até a posição nova

		glVertex2f(xPersonagem, yPersonagem+0.01f);
		glVertex2f(xPersonagem-0.01, yPersonagem+0.01f);
	glEnd();

	glBegin(GL_QUADS);//tenis
		glColor3f(1.0f,0.0,0.0f);
		glVertex2f(xPersonagem-0.01,yPersonagem);
		glVertex2f(xPersonagem-0.01,yPersonagem+0.01f);
		glVertex2f(xPersonagem+0.01,yPersonagem+0.01f);
		glVertex2f(xPersonagem+0.01,yPersonagem);

		xPersonagem = aux;//Restaura a posição original
	glEnd();
	glPopMatrix();
}

void braco(GLfloat movi)
{
	GLfloat aux;

	glPushMatrix();
	glBegin(GL_QUADS);
		glColor3f(0.94f, 0.86f, 0.51f);
		glVertex2f(xPersonagem-0.01, yPersonagem+0.08f);
		glVertex2f(xPersonagem, yPersonagem+0.08f);
																					//Movimenta só a parte de baixo do braço
		aux = xPersonagem;														//Grava a posição do personagem em um aux para poder voltar depois
		xPersonagem = xPersonagem + movi;										//Translata até a posição nova

		glVertex2f(xPersonagem, yPersonagem+0.04f);
		glVertex2f(xPersonagem-0.01, yPersonagem+0.04f);

		xPersonagem = aux;														//Restaura a posição original
	glEnd();
	glPopMatrix();
}

void cabeca()
{
	glPushMatrix();																	//Cabeça
		glTranslatef(xPersonagem, yPersonagem+0.12, 0.0f);
		glScalef(0.02f, 0.02f, 0.0f);
		circulo(0.94f, 0.86f, 0.51f, 0, 100);
	glPopMatrix();

	glPushMatrix();																	//Olho
		glTranslatef(xPersonagem+0.01, yPersonagem+0.125, 0.0f);
		glScalef(0.003f, 0.003f, 0.0f);
		circulo(0.0f, 0.0f, 0.0f, 0, 100);
	glPopMatrix();
}

void personagem()
{
	glPushMatrix();
	glTranslatef(0.0f, pulo, 0.0f);

	braco(movimento*-1);													//Braço e perna esquerda || movimento inverso
	perna(movimento);

	glBegin(GL_QUADS);//Corpo
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(xPersonagem-0.01, yPersonagem+0.04);
		glVertex2f(xPersonagem-0.01, yPersonagem+0.1);
		glVertex2f(xPersonagem+0.01, yPersonagem+0.1);
		glVertex2f(xPersonagem+0.01, yPersonagem+0.04);
	glEnd();

	cabeca();

	perna(movimento*-1);													//Braço e perna direita	|| movimento inverso
	braco(movimento);

	glPopMatrix();
}																				//FIM DESENHO PERSONAGEM


void arvore(int quantidade_arvore, GLfloat posInicial)
{
	GLfloat posX = xArvore + posInicial;

	for(int qntd = 1; qntd <= quantidade_arvore; qntd++)						//Desenha uma sequencia de árvores passada por parâmetro
	{
		glBegin(GL_QUADS);//Tronco
			//
			glColor3f(0.5f,0.3f,0.0f);
			glVertex2f(posX-0.01f, 0.0f);
			glVertex2f(posX-0.01f, 0.08f);
			glVertex2f(posX+0.01f, 0.08f);
			glVertex2f(posX+0.01f, 0.0f);
		glEnd();

			glBegin(GL_TRIANGLES);//Folhas1
			glColor3f(0.0f,0.5f,0.0f);
			glVertex2f(posX-0.05f,0.06f);
			glVertex2f(posX, 0.12f);
			glVertex2f(posX+0.05f,0.06f);
		glEnd();

			glBegin(GL_TRIANGLES);//Folhas2
			glColor3f(0.0f,0.6f,0.0f);
			glVertex2f(posX-0.04f,0.08f);
			glVertex2f(posX, 0.14f);
			glVertex2f(posX+0.04f,0.08f);
		glEnd();

		glBegin(GL_TRIANGLES);//Folhas3
			glColor3f(0.0f, 0.7f, 0.0f);
			glVertex2f(posX-0.02f, 0.12f);
			glVertex2f(posX, 0.16f);
			glVertex2f(posX+0.02f, 0.12f);
		glEnd();

		posX += 0.1;							//Posição das arvores
	}
}


void nuvem(GLfloat tx, GLfloat ty)
{
	GLfloat movi = moviNuvem + tx;//Movimento da nuvem
	glPushMatrix();
		glScalef(0.15f,0.15f,0.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glTranslatef(movi, ty, 0.0f);
		circulo(1.0f, 1.0f, 1.0f, 0, 100);
	glPopMatrix();
}


void cenario()
{
	glBegin(GL_QUADS);						//grama
		glColor3f(0.1f, 0.5f, 0.1f);
		glVertex2f(-2.0f, -1.0f);

		glColor3f(0.3f, 0.8f, 0.3f);
		glVertex2f(-2.0f, 0.0f);
		glVertex2f(2.0f, 0.0f);

		glColor3f(0.3f, 0.5f, 0.3f);
		glVertex2f(2.0f, -1.0f);
	glEnd();

	glPushMatrix();	//sol
		glScalef(0.2f, 0.2f, 0.2f);
		glTranslatef(4.0f, 4.0f, 0.0f);
		circulo(1.0f, 1.0f, 0.3f, 0, 100);
	glPopMatrix();

	glPushMatrix();
		nuvem(0.0,4.5);
		nuvem(1.0,4.5);
		nuvem(1.6,4.5);
	glPopMatrix();

	personagem();

	arvore(randQuantArvore1,0);//Sequências das arvores(qntdArvore, posinicial)
	arvore(randQuantArvore2,1);
	arvore(randQuantArvore3,2);

	arvore(randQuantArvore1,3);
	arvore(randQuantArvore2,5);
	arvore(randQuantArvore3,4);
}


void DesenhaTexto(char *string, GLfloat xPos, GLfloat yPos)
{
  	glPushMatrix();
        // Posição no universo onde o texto será colocado
        glRasterPos2f(xPos,yPos);

        // Exibe caracter a caracter
        while(*string)
             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*string++);

	glPopMatrix();
}


void desenha()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.69f,0.77f,0.87f,1.0f);

	cenario();
	DesenhaTexto(placar,-1.0f,0.85f);

	glFlush();
}
int colisao()
{
	int resultado = 0;//0 = sem colisão

	if(pulo <= 0.08f )
	{
		if(xPersonagem >= xArvore-0.05 && xPersonagem <= xArvore+(randQuantArvore1*0.1) +0.01)	//0.5 = tamanho da arvore para os lados
		{resultado = 1;}
		if(xPersonagem >= xArvore+1-0.05 && xPersonagem <= (xArvore+1)+(randQuantArvore2*0.1) +0.01)
		{resultado = 1;}
		if(xPersonagem >= xArvore+2-0.05 && xPersonagem <= (xArvore+2)+(randQuantArvore3*0.1) +0.01)
		{resultado = 1;}
		if(xPersonagem >= xArvore+3-0.05 && xPersonagem <= (xArvore+3)+(randQuantArvore1*0.1) +0.01)
		{resultado = 1;}
		if(xPersonagem >= xArvore+5-0.05 && xPersonagem <= (xArvore+5)+(randQuantArvore2*0.1) +0.01)
		{resultado = 1;}
		if(xPersonagem >= xArvore+4-0.05 && xPersonagem <= (xArvore+4)+(randQuantArvore3*0.1) +0.01)
		{resultado = 1;}
	}

	return resultado;
}

void Timer(int value)//Move o cenário
{
	if((pause < 0) && (colisao() == 0))//PAUSA O JOGO
	{
		xArvore -= 0.012f;//Distância movimnto

		if(direcao == 1)//Move os braços/pernas para frente
		{
			movimento += 0.003;//Movimento de translação
		if(movimento > 0.02f)//Limite do movimento
			direcao = -1;//Ao chegar no limite inverte o processo
		}
		if(direcao == -1)//Move os braços/pernas para tras
		{
			movimento -= 0.003;
		if(movimento < -0.02f)
			direcao = 1;
		}
		if(ativarPulo == 1)	//Controla o movimento para cima
		{
			chao = 0;
			pulo += 0.015f;	//Incrementa o movimento de translação do pulo
		if(pulo >= 0.35f)//Ao chegar no limite, desativa o pulo
			ativarPulo = 0;
		}

		if(ativarPulo == 0 && pulo > 0.0f)//Controla o movimento para baixo
			pulo -= 0.008f;

		if(pulo <= 0)//Verifica se o personagem esta no chao para poder ativar o pulo
		{
			chao = 1;
			pulo = 0.0f;
		}

		moviNuvem -= 0.01f;
		if(moviNuvem < -12.0f)
		{
			moviNuvem = 10.0f;
		}

		if(xArvore < -6.4)//Verifica se é o fim da sequência
		{
			xArvore = 1.0f;//Zera as sequências de árvores e recomeça da posição inicial
			do									//Gera quantidades aleatórias de árvores para cada sequência
			{
				randQuantArvore1 = rand()%5;
				randQuantArvore2 = rand()%5;
				randQuantArvore3 = rand()%5;
			}
			while(randQuantArvore1 == 0 || randQuantArvore2 == 0 || randQuantArvore3 == 0);		//Evita sequências sem árvores
		}
		glutPostRedisplay();
		sprintf(placar, "%d", pontos);
		pontos = pontos + 1;
	}										//FIM PAUSE

    glutTimerFunc(velocidade,Timer, 1);		//Tempo de movimento do cenário
}

void teclasEspeciais(int key, int x, int y)
{
    if(key == GLUT_KEY_UP)						//Controla o movimento para cima
    {
    	if(chao == 1)							//Só permite pular se o personagem estiver no chao
    		ativarPulo = 1;
    }
    if(key == GLUT_KEY_DOWN )
    {
    	ativarPulo = 0;
    	if(ativarPulo == 0 && pulo > 0.0f)		//Controla o movimento para baixo
    		pulo -= 0.05f;
    }
    glutPostRedisplay();
}


void teclado(unsigned char key, int x, int y)
{
    switch (key) {

    	    case 27://Fecha o jogo
                glutDestroyWindow(0);
                exit(0);
                break;

            case 'r':// Reinicia o jogo
            	xArvore = -6.4f;
            	if(pause == 1)
            		pause = pause * -1;

            	pulo = 0;
            	ativarPulo = 0;
            	chao = 1;
            	pontos = 0;
            	moviNuvem = 10.0f;
            break;

            case 'p':  // Pausa o jogo
            	pause = pause * -1;
            	break;

    }
    glutPostRedisplay();
}


void mainMenu(int item)//Menu do botão direito do mouse
{

 switch (item)
 {
 	 case MENU_A :
 		pause = pause * -1;
 	 break;
 	 case MENU_B:
 		xArvore = -6.4f;
		if(pause == 1)
			pause = pause * -1;

		pulo = 0;
		ativarPulo = 0;
		chao = 1;
		pontos = 0;
		moviNuvem = 10.0f;
 	 break;
 	 case MENU_C:
 		glutDestroyWindow(0);
 		exit(0);
 	 break;
 }
 glutPostRedisplay();
 return ;
}


int main(int argc, char** argv)
{
	 glutInit(&argc, argv);
     glutCreateWindow("Computação Gráfica Noturno");
     glutInitWindowSize(800,800);
     glutInitWindowPosition(100,100);
     glutDisplayFunc(desenha);
     glutSpecialFunc(teclasEspeciais);
     glutKeyboardFunc(teclado);
     glutTimerFunc(33, Timer, 1);

     glutCreateMenu (mainMenu);
     glutAddMenuEntry ("Pausa (p)", MENU_A);
     glutAddMenuEntry ("Reiniciar (r)", MENU_B);
     glutAddMenuEntry ("Fechar a janela (esc)", MENU_C);
     glutAttachMenu(GLUT_RIGHT_BUTTON);

     glutMainLoop();
     return 0;
}
