#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdbool.h>
#include <iostream>
#include <cmath>

DWORD volume = 0x80008000;

void setVolume(DWORD newVolume) {
    volume = newVolume;
    waveOutSetVolume(0, volume); // Ajusta el volumen global
}

float timeElapsed = 0.2f;
using namespace std;
float angleX = 0.0f; // Ángulo de rotación en el eje X
float angleY = 0.0f; // Ángulo de rotación en el eje Y
float angleZ = 0.0f; // Ángulo de rotación en el eje Z
float zoom = 5.0f;   // Control de zoom (distancia de la cámara)
bool pausa = false; 
float posX = 0.0f, posZ = 0.0f;

float moveSpeed = 25.0f; // Velocidad de movimiento

// visivilidad 
bool manzana1 = true; // Estado del objeto que colisiona
bool panel1 = false;   // Estado para mostrar el cuadrado

bool manzana2 = true; // Estado del objeto que colisiona
bool panel2 = false;   // Estado para mostrar el cuadrado

bool manzana3 = true; // Estado del objeto que colisiona
bool panel3 = false;   // Estado para mostrar el cuadrado

bool manzana4 = true; // Estado del objeto que colisiona
bool panel4 = false;   // Estado para mostrar el cuadrado

bool panel5 = false;   // Estado para mostrar el cuadrado

GLfloat angulo=0.0;

float posY = 0.0f;
float saltoVelocidad = 30.0f;
float gravity = 25.0f;
bool isJumping = false;

int contpuntos = 0; // contador de puntos

int olasmov = 0; // movimiento del agua
int estado = 0;

void colision1(){
    if (!panel1 && manzana1 && posX > -480 && posX < -460 && posZ > 420 && posZ < 800) {
        manzana1 = false; // Hacer desaparecer el cubo objetivo
        panel1 = true;     // Mostrar el cuadrado
        contpuntos ++;
    	std::cout << "Presiona una ESC para continuar." << std::endl;
    }
    if (!panel2 && manzana2 && posX > 200 && posX < 500 && posZ > -10 && posZ < 20) {
        manzana2 = false; // Hacer desaparecer el cubo objetivo
        panel2 = true;     // Mostrar el cuadrado
    	std::cout << "Presiona una ESC para continuar." << std::endl;
    	contpuntos ++;
    }
    if (!panel3 && manzana3 && posX > -980 && posX < -800 && posZ > 280 && posZ < 330) {
        manzana3 = false; // Hacer desaparecer el cubo objetivo
        panel3 = true;     // Mostrar el cuadrado
        contpuntos ++;
    	std::cout << "Presiona una ESC para continuar." << std::endl;
    }
    if (!panel4 && manzana4 && posX >  -980 && posX < -800 && posZ > -230 && posZ < -170) {
        manzana4 = false; // Hacer desaparecer el cubo objetivo
        panel4 = true;     // Mostrar el cuadrado
    	std::cout << "Presiona una ESC para continuar." << std::endl;
    	contpuntos ++;
    }
    if (!panel5 && posX > -500 && posX < -400 && posZ > -230 && posZ < -170) {
        panel5 = true;     // Mostrar el cuadrado
    	std::cout << " " << std::endl;
    }
}

void timer(int valor){
	
	if(estado == 0) olasmov += 1;
	else olasmov -= 1;
	
    if (isJumping) {
        posY += saltoVelocidad;  // Actualizar la posición en Y
        saltoVelocidad -= gravity; // Reducir la velocidad por la gravedad

        // Cuando el cubo regrese al suelo
        if (posY <= 0.0f) {
            posY = 0.0f;       // Corregir la posición en el suelo
            isJumping = false;  // Terminar el salto
            saltoVelocidad = 50.0f; // Reiniciar la velocidad inicial
        }
    }
    
    if(olasmov == 50) estado = 1;
    if(olasmov == 0) estado = 0;
    
	colision1();
    glutPostRedisplay();            // Redibujar la escena
    glutTimerFunc(20, timer, 0);
}

typedef struct{
	GLubyte *dibujo;
	GLuint bpp;
	GLuint largo;
	GLuint ancho;
	GLuint ID;
} textura;

textura tagua;
textura tgrass;
textura ttierra;
textura tpozo;
textura troca;
textura twood;
textura tdwood;
textura tapple;
textura tMP;
textura tdoce2;
textura tbrick;
textura tciudad;
textura tDA;
textura tM;
textura tMSC;
textura tfinal;
textura tnfinal;



GLfloat velocidadCubo = 500; 
GLfloat angCubo = 0.0; 
GLfloat ang = 0.0;

textura prueba; // planeta tierra guía

int cargaTGA(char const* nombre, textura *imagen){
  GLubyte cabeceraTGA[12]={0,0,2,0,0,0,0,0,0,0,0,0};
  GLubyte compararTGA[12];// 0,0,2,0,0,0,0,0,0,0,0,0
  GLubyte cabecera[6];//244,1,244,1,32,40
  GLuint bytesporpunto;
  GLuint tamanoimagen;
  GLuint temp,i;
  GLuint tipo=GL_RGBA;
  
   FILE *archivo = fopen(nombre,"rb");
     if (
	     archivo == NULL ||
	     fread(compararTGA,1,sizeof(compararTGA),archivo) != sizeof(compararTGA) ||
	     memcmp(cabeceraTGA,compararTGA,sizeof(compararTGA)) != 0  ||
	     fread(cabecera,1,sizeof(cabecera), archivo) != sizeof(cabecera)
	 ){
     	if(archivo ==NULL){
     		printf("No se encontro el archivo %\n", nombre);
     		return 0;
		 }else {
		 	fclose(archivo);
		 	return 0;
		 }
	}

	imagen->largo= 256 * cabecera[1] + cabecera[0]; 
	//   (256*1) + 244= 256 +244 =500
	imagen->ancho= 256 * cabecera[3] + cabecera[2];
	//   (256*1) + 244= 256 +244=500
	
	if(
	  imagen->largo <=0  ||
	  imagen->ancho <=0  ||
	  (cabecera[4] != 24 && cabecera[4] !=32 )
	){
	   printf("Datos invalidos\n");	
	   fclose(archivo);
	   return 0;
	}
	
	imagen->bpp=cabecera[4];
	bytesporpunto = cabecera[4]  / 8; //     32 /8 =4
	
	tamanoimagen= imagen->largo * imagen->ancho * bytesporpunto;
	//500 * 500 * 4;   = 1000000
	//1000000 /1024 = 976,56  ==977 kb
	
	imagen->dibujo = (GLubyte *)malloc(tamanoimagen);
	
	if(
	   imagen->dibujo == NULL ||
	   fread(imagen->dibujo,1,tamanoimagen,archivo) != tamanoimagen
	){
	   	if(imagen->dibujo != NULL){
	   		printf("Error leyendo imagen\n");
	   		free(imagen->dibujo);
	   		
		   }else {
		   	printf("Error asignando memoria\n");
		   }
		   fclose(archivo);
		   return 0;
	}
	
	// TGA :  BGRA             RGBA
	
	for(i=0; i < (int)tamanoimagen; i+=bytesporpunto){
		temp=imagen->dibujo[i];
		imagen->dibujo[i] = imagen->dibujo[i+2];
		 imagen->dibujo[i+2]=temp;
	}
	
	fclose(archivo);
	
	glGenTextures(1,&imagen[0].ID);
	glBindTexture(GL_TEXTURE_2D,imagen[0].ID);
	if(imagen->bpp == 24)  tipo=GL_RGB;
	glTexImage2D(GL_TEXTURE_2D,0,tipo,imagen[0].ancho, imagen[0].largo,0,tipo,GL_UNSIGNED_BYTE,imagen[0].dibujo);
	glTexParameterf(GL_TEXTURE_2D ,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D ,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	return 1;
}


void init(){
	GLfloat light_position[]={1,1,1,0};
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	glShadeModel(GL_SMOOTH);
	glClearColor(0.7f, 0.7f, 1.0f, 1.0f);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	//Carga de textura
	if(!cargaTGA("agua.tga", &tagua)){
		printf("Error cargando textura agua\n");
		exit(0);
	}
	if(!cargaTGA("grass.tga", &tgrass)){
		printf("Error cargando textura grass\n");
		exit(0);
	}
    if(!cargaTGA("tierra.tga", &ttierra)){
		printf("Error cargando textura tierra\n");
		exit(0);
	}
    if(!cargaTGA("pozo.tga", &tpozo)){
		printf("Error cargando textura pozo\n");
		exit(0);
	}
	if(!cargaTGA("roca.tga", &troca)){
		printf("Error cargando textura pozo\n");
		exit(0);
	}
	
	if(!cargaTGA("wood.tga", &twood)){
		printf("Error cargando textura madera\n");
		exit(0);
	}
	if(!cargaTGA("dwood.tga", &tdwood)){
		printf("Error cargando textura madera oscura\n");
		exit(0);
	}
	if(!cargaTGA("apple.tga", &tapple)){
		printf("Error cargando textura manzana\n");
		exit(0);
	}	
	if(!cargaTGA("MachuPicchu.tga", &tMP)){
		printf("Error cargando textura MP\n");
		exit(0);
	}

	if(!cargaTGA("doce.tga", &tdoce2)){
		printf("Error cargando textura doce2\n");
		exit(0);
	}
	if(!cargaTGA("ladrillo.tga", &tbrick)){
		printf("Error cargando textura brick\n");
		exit(0);
	}
	if(!cargaTGA("ciudad.tga", &tciudad)){
		printf("Error cargando textura ciudad\n");
		exit(0);
	}
	if(!cargaTGA("DoceAngulos.tga", &tDA)){
		printf("Error cargando textura DA\n");
		exit(0);
	}
	if(!cargaTGA("moray.tga", &tM)){
		printf("Error cargando textura M\n");
		exit(0);
	}
	if(!cargaTGA("montaña.tga", &tMSC)){
		printf("Error cargando textura MSC\n");
		exit(0);
	}
	if(!cargaTGA("final.tga", &tfinal)){
		printf("Error cargando textura final\n");
		exit(0);
	}
	if(!cargaTGA("nofinal.tga", &tnfinal)){
		printf("Error cargando textura final\n");
		exit(0);
	}
		
	if(!cargaTGA("tTi.tga", &prueba)){
		printf("Error cargando textura ti\n");
		exit(0);
	}
}




// ########################## CONTROL DE SONIDO ########################## // 

void increaseVolume() {
    // Incrementa el volumen (máximo es 0xFFFF)
    DWORD left = (volume & 0xFFFF0000) >> 16;
    DWORD right = (volume & 0xFFFF);
    if (left < 0xFFFF && right < 0xFFFF) {
        left += 0x1000; // Incrementa por pasos
        right += 0x1000;
        volume = (left << 16) | right;
        setVolume(volume);
    }
}

void decreaseVolume() {
    // Decrementa el volumen (mínimo es 0x0000)
    DWORD left = (volume & 0xFFFF0000) >> 16;
    DWORD right = (volume & 0xFFFF);
    if (left > 0x0000 && right > 0x0000) {
        left -= 0x1000; // Decrementa por pasos
        right -= 0x1000;
        volume = (left << 16) | right;
        setVolume(volume);
    }
}


// ########################## AMBIENTE ########################## // 
// t = tamaño // a = textura //
void cubo(int t, textura a){

    glPushMatrix();
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,a.ID);
	
	glBegin(GL_QUADS); 
		glTexCoord2f(0.0,0.0); glVertex3d(-t, t, t);
	    glTexCoord2f(1.0,0.0); glVertex3d( t, t, t);
	    glTexCoord2f(1.0,1.0); glVertex3d( t,-t, t);
	    glTexCoord2f(0.0,1.0); glVertex3d(-t,-t, t);
	    
	    glTexCoord2f(0.0,0.0); glVertex3d( t, t, t);
	    glTexCoord2f(1.0,0.0); glVertex3d( t, t,-t);
	    glTexCoord2f(1.0,1.0); glVertex3d( t,-t,-t);
	    glTexCoord2f(0.0,1.0); glVertex3d( t,-t, t);
	    
	    glTexCoord2f(0.0,0.0); glVertex3d( t, t,-t);
	    glTexCoord2f(1.0,0.0); glVertex3d(-t, t,-t);
	    glTexCoord2f(1.0,1.0); glVertex3d(-t,-t,-t);
	    glTexCoord2f(0.0,1.0); glVertex3d( t,-t,-t);
	    
	    glTexCoord2f(0.0,0.0); glVertex3d(-t, t,-t);
	    glTexCoord2f(1.0,0.0); glVertex3d(-t, t, t);
	    glTexCoord2f(1.0,1.0); glVertex3d(-t,-t, t);
	    glTexCoord2f(0.0,1.0); glVertex3d(-t,-t,-t);
	    
	    glTexCoord2f(0.0,0.0); glVertex3d(-t, t,-t);
	    glTexCoord2f(1.0,0.0); glVertex3d( t, t,-t);
	    glTexCoord2f(1.0,1.0); glVertex3d( t, t, t);
	    glTexCoord2f(0.0,1.0); glVertex3d(-t, t, t);
	    
	    glTexCoord2f(0.0,0.0); glVertex3d(-t,-t, t);
	    glTexCoord2f(1.0,0.0); glVertex3d( t,-t, t);
	    glTexCoord2f(1.0,1.0); glVertex3d( t,-t,-t);
	    glTexCoord2f(0.0,1.0); glVertex3d(-t,-t,-t);
	glEnd();

	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

// camino de tierra
void path_grass(){
	glPushMatrix();
	glTranslated(-120,0,0);
	cubo(30, tgrass);
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(-60,0,0);
	cubo(30, ttierra);
	glLoadIdentity();
	glPopMatrix();
	
	glPushMatrix();
	cubo(30, ttierra);
	glLoadIdentity();
	glPopMatrix();

	glPushMatrix();
	glTranslated(60,0,0);
	cubo(30, ttierra);
	glLoadIdentity();
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(120,0,0);
	cubo(30, tgrass);
	glPopMatrix();

}

// pozo hueco
void pozo(){

    glPushMatrix();
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
		int t = 150;
		
	glBindTexture(GL_TEXTURE_2D,tpozo.ID);
	glBegin(GL_QUADS); 
		glTexCoord2f(0.0,0.0); glVertex3d(-t, t, t);
	    glTexCoord2f(1.0,0.0); glVertex3d( t, t, t);
	    glTexCoord2f(1.0,1.0); glVertex3d( t,-t, t);
	    glTexCoord2f(0.0,1.0); glVertex3d(-t,-t, t);
	    
	    glTexCoord2f(0.0,0.0); glVertex3d( t, t, t);
	    glTexCoord2f(1.0,0.0); glVertex3d( t, t,-t);
	    glTexCoord2f(1.0,1.0); glVertex3d( t,-t,-t);
	    glTexCoord2f(0.0,1.0); glVertex3d( t,-t, t);
	    
	    glTexCoord2f(0.0,0.0); glVertex3d( t, t,-t);
	    glTexCoord2f(1.0,0.0); glVertex3d(-t, t,-t);
	    glTexCoord2f(1.0,1.0); glVertex3d(-t,-t,-t);
	    glTexCoord2f(0.0,1.0); glVertex3d( t,-t,-t);
	    
	    glTexCoord2f(0.0,0.0); glVertex3d(-t, t,-t);
	    glTexCoord2f(1.0,0.0); glVertex3d(-t, t, t);
	    glTexCoord2f(1.0,1.0); glVertex3d(-t,-t, t);
	    glTexCoord2f(0.0,1.0); glVertex3d(-t,-t,-t);
	    
	    glTexCoord2f(0.0,0.0); glVertex3d(-t,-t, t);
	    glTexCoord2f(1.0,0.0); glVertex3d( t,-t, t);
	    glTexCoord2f(1.0,1.0); glVertex3d( t,-t,-t);
	    glTexCoord2f(0.0,1.0); glVertex3d(-t,-t,-t);
	glEnd();

	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

// plataforma de rocas
void rocas(){
	glPushMatrix();
	cubo(150, troca);
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(-350,0,0);
	cubo(150, troca);
	glPopMatrix();
	
	// despues de la cascada
	glPushMatrix();
	glTranslated(-750,0,0);
	cubo(150, troca);
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(- 750, 0, -350);
	cubo(150, troca);
	glPopMatrix();

	glPushMatrix();
	glTranslated(- 1100, 0, -350);
	cubo(150, troca);
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(- 1450, 0, -350);
	cubo(150, troca);
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(- 1450, 0, -700);
	cubo(150, troca);
	glPopMatrix();
	
}

// palos del muelle / madera oscura
void dmadera(){
	glTranslated(0,-250,0);
	
		for ( int i= 0; i < 15; i++)
	{
		int x = 20;
		glTranslated(0,x++,0);
		
	    glPushMatrix();
		cubo(10, tdwood);
		glLoadIdentity();
		glPopMatrix();
	
		glPushMatrix();
		glTranslated(0,10,0);
		cubo(10, tdwood);
		glLoadIdentity();
		glPopMatrix();
	}
}

// muelle / madera clara
void madera(){
	
	for ( int i= 0; i < 3; i++)
	{
		int x = 120;
		glTranslated(0,0,x++);
		
	    glPushMatrix();
		cubo(60, twood);
		glLoadIdentity();
		glPopMatrix();
		
		glPushMatrix();
		glTranslated(188,0,0);
		dmadera();
		glPopMatrix();
		
		glPushMatrix();
		glTranslated(120,0,0);
		cubo(60, twood);
		glLoadIdentity();
		glPopMatrix();
			
		glPushMatrix();
		glTranslated(-70,0,0);
		dmadera();
		glPopMatrix();
	}

}

// ########################## IMAGENES ########################## // 

void doceang(){
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tdoce2.ID);
		
	glBegin(GL_QUADS); 
		glTexCoord2f(0.0,0.0); glVertex3d( 800, 450, 300);
	    glTexCoord2f(1.0,0.0); glVertex3d(-800, 450, 300);
	    glTexCoord2f(1.0,1.0); glVertex3d(-800,-450, 300);
	    glTexCoord2f(0.0,1.0); glVertex3d( 800,-450, 300);
	glEnd();


	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void ciudadplano(){
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tciudad.ID);
		
	glBegin(GL_QUADS); 
		glTexCoord2f(0.0,0.0); glVertex3d( 1200, 675, 300);
	    glTexCoord2f(1.0,0.0); glVertex3d(-1200, 675, 300);
	    glTexCoord2f(1.0,1.0); glVertex3d(-1200,-675, 300);
	    glTexCoord2f(0.0,1.0); glVertex3d( 1200,-675, 300);
	glEnd();

	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}


// ########################## MANZANAS ########################## // 

void manzana(){
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glRotatef(90 ,0,0,1);
	glRotatef(angulo ,1,0,0);
	
	if (manzana1) {
        glPushMatrix();
		GLUquadric *qobj = gluNewQuadric();
		gluQuadricTexture(qobj,GL_TRUE);
		glBindTexture(GL_TEXTURE_2D,tapple.ID);
		gluSphere(qobj,50,50,50); 
		gluDeleteQuadric(qobj);
        glPopMatrix();
    }  
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);	
}

void manzanadoce(){
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glRotatef(90 ,0,0,1);
	glRotatef(angulo ,1,0,0);
	
	if (manzana2) {
        glPushMatrix();
		GLUquadric *qobj = gluNewQuadric();
		gluQuadricTexture(qobj,GL_TRUE);
		glBindTexture(GL_TEXTURE_2D,tapple.ID);
		gluSphere(qobj,50,50,50); 
		gluDeleteQuadric(qobj);
        glPopMatrix();
    }  
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);	
}

void manzanatres(){
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glRotatef(90 ,0,0,1);
	glRotatef(angulo ,1,0,0);
	
	if (manzana3) {
        glPushMatrix();
		GLUquadric *qobj = gluNewQuadric();
		gluQuadricTexture(qobj,GL_TRUE);
		glBindTexture(GL_TEXTURE_2D,tapple.ID);
		gluSphere(qobj,50,50,50); 
		gluDeleteQuadric(qobj);
        glPopMatrix();
    }  
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);	
}

void manzanacuatro(){
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glRotatef(90 ,0,0,1);
	glRotatef(angulo ,1,0,0);
	//glTranslated(dist,0,0);
	
	if (manzana4) {
        glPushMatrix();
		GLUquadric *qobj = gluNewQuadric();
		gluQuadricTexture(qobj,GL_TRUE);
		glBindTexture(GL_TEXTURE_2D,tapple.ID);
		gluSphere(qobj,50,50,50); 
		gluDeleteQuadric(qobj);
        glPopMatrix();
    }  
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);	
}


// ########################## PINGÜINO ########################## // 

void glutcube(int tam, float r, float g, float b){
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
	GLfloat mat_ambient[]={0.7,0.7,0.7,0};
	GLfloat mat_diffuse[]={r,g,b,1};
	GLfloat mat_specular[]={1,1,1,1};
	GLfloat mat_shininess[]={100.0f};
	
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS, mat_shininess);
	
	glutSolidCube(tam);
}

void dobleglutcube(int tam, float r, float g, float b){
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
	glPushMatrix();
	glTranslated(0,-10,0);
	glutcube(tam, r, g,b);
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(0, 10,0);
	glutcube(tam, r, g, b);
	glPopMatrix();
}

void pingu(){
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
	dobleglutcube(55, 0.129, 0.129, 0.118);
	
	// pancita
	glPushMatrix();
		glTranslated(0,-10,15);
		glutcube(45, 1, 1, 1);
	glPopMatrix();
	
	//pico
	glPushMatrix();
		glTranslated(0, 19, 28);
		glScalef(0.5f,0.5f,1.5f);
		glRotatef(90, 1, 0, 0); 
		glRotatef(90, 0, 0, 1); 
		dobleglutcube(20, 0.969, 0.761, 0.075);
	glPopMatrix();
	
	// ojo1
	glPushMatrix();
		glTranslated(-14, 28, 27);
		glutcube(8, 1, 1, 1);
	glPopMatrix();
	
	// ojo2
	glPushMatrix();
		glTranslated(14, 28, 27);
		glutcube(8, 1, 1, 1);
	glPopMatrix();
	
	//pata1
	glPushMatrix();
		glTranslated(-18, -34, 38);
		glScalef(0.5f,0.4f,1.0f);
		glRotatef(90, 1, 0, 0); 
		glRotatef(90, 0, 0, 1); 
		dobleglutcube(20, 0.969, 0.761, 0.075);
	glPopMatrix();
	
	//pata2
	glPushMatrix();
		glTranslated(18, -34, 38);
		glScalef(0.5f,0.4f,1.0f);
		glRotatef(90, 1, 0, 0); 
		glRotatef(90, 0, 0, 1); 
		dobleglutcube(20, 0.969, 0.761, 0.075);
	glPopMatrix();
	
	//aleta1
	glPushMatrix();
		glTranslated(-30, -10, 0);
		glScalef(0.5f,0.5f,0.5f);
		glRotatef(90, 0, 1, 0); 
		glRotatef(20, 1, 0, 0);
		dobleglutcube(40, 0.129, 0.129, 0.118);
	glPopMatrix();

	//aleta2
	glPushMatrix();
		glTranslated(30, -10, 0);
		glScalef(0.5f,0.5f,0.5f);
		glRotatef(90, 0, 1, 0); 
		glRotatef(-20, 1, 0, 0);
		dobleglutcube(40, 0.129, 0.129, 0.118);
	glPopMatrix();
}


// ########################## CHAKANA ########################## // 

void chakana(){
	glRotatef(angulo*3 ,0,1,0);
	
	float r =  0.878, g =  0.75, b = 0.345; // arriba
	glutcube(70, r, g, b);
	
	// segundo nivel
	
	glPushMatrix();							// abajo medio
	glTranslated(0, -70, 0);
    glutcube(70, r, g, b);
	glPopMatrix();
	
	glPushMatrix();							// derecha
	glTranslated(- 70, -70, 0);
    glutcube(70, r, g, b);
	glPopMatrix();
	
	glPushMatrix();							// izquierda
	glTranslated(  70, -70, 0);
    glutcube(70, r, g, b);
	glPopMatrix();
	
	// tercer nivel
	
	glPushMatrix();							// derecha
	glTranslated(- 70, -140, 0);
    glutcube(70, r, g, b);
	glPopMatrix();
	
	glPushMatrix();							// izquierda
	glTranslated(  70, -140, 0);
    glutcube(70, r, g, b);
	glPopMatrix();
	
	glPushMatrix();							// derecha
	glTranslated(- 140, -140, 0);
    glutcube(70, r, g, b);
	glPopMatrix();
	
	glPushMatrix();							// izquierda
	glTranslated(  140, -140, 0);
    glutcube(70, r, g, b);
	glPopMatrix();
	
	// cuarto nivel
	
	glPushMatrix();							// abajo medio
	glTranslated(0, -210, 0);
    glutcube(70, r, g, b);
	glPopMatrix();
	
	glPushMatrix();							// derecha
	glTranslated(- 70, -210, 0);
    glutcube(70, r, g, b);
	glPopMatrix();
	
	glPushMatrix();							// izquierda
	glTranslated(  70, -210, 0);
    glutcube(70, r, g, b);
	glPopMatrix();
	
	// quinto nivel
	
	glPushMatrix();							// izquierda
	glTranslated(  0, -280, 0);
    glutcube(70, r, g, b);
	glPopMatrix();
}

// ############################################################## // 

float anguloZPengu =0.0f;
bool MiraDerecha =false,MiraIZquierda = false,MiraDelante=false,MiraAtras=false;


void update(int value) {
    timeElapsed += 1.0f; // Incrementar el tiempo para la animación
    glutPostRedisplay(); // Solicitar redibujado
    glutTimerFunc(16, update, 0); // Llamar a esta función cada 16ms (~60 FPS)
}

void ola(){
	glTranslated(260, 530 + 400, 100);
    
    const int gridSize = 25; // Número de cubos por lado
    const int cubeSize = 80; // Tamaño de cada cubo pequeño
    textura tagua; // Textura del agua
    tagua.ID = 1; 

    // cubos pequeños en rejilla
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            // posición base de cada cubo
            float x = (i - gridSize / 2) * cubeSize * 1.1f - 1080; // posición inicial
            float z = (j - gridSize / 2) * cubeSize * 1.1f + 400;

            // frecuencia y amplitud ajustables
            float y = sin((i + j) * 0.3f + timeElapsed*0.5f) * 30.0f - 1100; // Altura dinámica tipo ola

            // Dibujar el cubo en su posición
            glPushMatrix();
            glTranslated(x, y, z);
            cubo(cubeSize / 2, tagua);
            glPopMatrix();
        }
    }
}


void panel(textura tex) {
	glMatrixMode(GL_PROJECTION); 
  	glPushMatrix();
  	glLoadIdentity();                  // Reiniciar la matriz de proyección
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);  // Configurar un espacio 2D ortogonal
  	
	glMatrixMode(GL_MODELVIEW);        // Cambiar a la matriz de modelo/vista
    glPushMatrix();                    // Guardar la configuración actual
    glLoadIdentity();                  // Reiniciar la matriz de modelo/vista
    
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	float t = 1.0;
	glBindTexture(GL_TEXTURE_2D,tex.ID);
	glBegin(GL_QUADS); 
		glTexCoord2f(0.0,0.0); glVertex3d(-t, t, t);
	    glTexCoord2f(1.0,0.0); glVertex3d( t, t, t);
	    glTexCoord2f(1.0,1.0); glVertex3d( t,-t, t);
	    glTexCoord2f(0.0,1.0); glVertex3d(-t,-t, t);
	    
	glEnd();

	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}



// ########################## DISPLAY ########################## // 

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Aplicamos las rotaciones al objeto compuesto
    glRotatef(angleX, 1.0, 0.0, 0.0); // Rotación en X
    glRotatef(angleY, 0.0, 1.0, 0.0); // Rotación en Y
    glRotatef(angleZ, 0.0, 0.0, 1.0); // Rotación en Z
    
    //AGUA
	glPushMatrix();
	glTranslated(olasmov, olasmov, olasmov);
    ola();
	glPopMatrix();
	
	glPushMatrix();
	glScalef(0.7f,0.7f,0.7f);   
	glTranslated(1000, 450, 130);
	doceang();
	glPopMatrix();
	
	glPushMatrix();
	glScalef(0.85f,0.85f,0.85f);   
	glTranslated(-970, 600, 1160);
	ciudadplano();
	glPopMatrix();
	
	glPushMatrix();
	glTranslated(450, 330, 620);
	cubo(300, tbrick);
	glTranslated(0, 0, 600);
	cubo(300, tbrick);
	glPopMatrix();
	
	// Segundo panel
	if (panel2) {
        panel(tDA); // Dibujar cuadrado si está activado
    } else {
        glPushMatrix();
		glTranslated(500, 100, 130);
		manzanadoce();
		glPopMatrix();
    }
	// Primer panel
	if (panel1) {
        panel(tMP); // Dibujar cuadrado si está activado
    } else {
        glPushMatrix();
		glTranslated(-1000, 100, 1100);
		manzana();
		glPopMatrix();
    }
    // Tercer panel
	if (panel3) {
        panel(tM); // Dibujar cuadrado si está activado
    } else {
        glPushMatrix();
		glTranslated(-1715, 100, 725);
		manzanatres();
		glPopMatrix();
    }  
    // Cuarto panel
	if (panel4) {
        panel(tMSC); // Dibujar cuadrado si está activado
    } else {
        glPushMatrix();
		glTranslated(-1715, 100, -300);
		manzanacuatro();
		glPopMatrix();
    }
    
    
    // Chakana quinto panel
	if (panel5) {
		if (contpuntos == 4){
			panel(tfinal);
		}
		else {
			panel(tnfinal);
			posX = 0;
			posZ = 0;
		}
    } else {
    	panel5 = false; 
    	glPushMatrix();
    	glScalef(0.8f,0.8f,0.8f);
   		glTranslated(- 1100, 390, -380);
    	glTranslated(0, olasmov, 0);
		chakana();
		glPopMatrix();
    }

	
	
	glPushMatrix(); 
	glScalef(2.0f,2.0f,2.0f);   
	glTranslatef(10, 50, 50);
    glTranslatef(posX, posY, posZ);
    glRotatef(anguloZPengu,0.0f,50.0f,0.0f);
	pingu();
	glPopMatrix();
	

	glPopMatrix();
    glPushMatrix();
	for ( int i= 0; i < 15; i++)
	{
		int x = 60;
		glTranslated(0,0,x++);
		
		int r = i % 2; 
		if(r == 0)
		glTranslated(30,0,0);
		else
		glTranslated(-30,0,0);
		
	    path_grass(); // igresamos modelamiento
	}
	glPopMatrix();
	
	
	glPopMatrix();
    glPushMatrix();
    glTranslated(100,0,170);
    glRotatef(90,0,1,0);
	for ( int i= 0; i < 15; i++)
	{
		int x = 60;
		glTranslated(0,0,x++);
		
		int r = i % 2; 
		if(r == 0)
		glTranslated(30,0,0);
		else
		glTranslated(-30,0,0);
		
	    path_grass(); // igresamos modelamiento
	}
	glPopMatrix();

	// pozo
    glPushMatrix();
    glTranslated(30,-110,1081);
	pozo();
	
	// plataforma de rocas
	glTranslated(-290,0,0);
	rocas();
	glPopMatrix();
	
	// muelle
	glTranslated(-1760,0,-235);
	madera();
	
	glRotatef(90, 0, 1, 0);
	glTranslated(350,0,-120);
	madera();
	
	glTranslated(0,0,-10);
	madera();
	glPopMatrix();
	
	glTranslated(0,0,-10);
	madera();
	glPopMatrix();
	
    glutSwapBuffers(); // Intercambiamos los buffers para mostrar en pantalla
}

void reshape( int largo,int ancho){
		glViewport(0,0,(GLsizei)largo,(GLsizei)ancho);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		int t=2000;
		glOrtho(-t,t,-t,t,-t,t);
    
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
}







void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 's': case 'S': 
		angleX -= 5.0f; 
		
		break;
		case 27: panel1 = false; panel2 = false; panel3 = false; panel4 = false; panel5 = false; break;
        case 'w': case 'W': angleX += 5.0f; break;
        case 'd': case 'D': angleY -= 5.0f; break;
        case 'a': case 'A': angleY += 5.0f; break;
        case 'q': case 'Q': angleZ += 5.0f; break;
        case 'e': case 'E': angleZ -= 5.0f; break;
        case 'r': case 'R': panel1 = false; panel2 = false; panel3 = false; panel4 = false; panel5 = false; manzana1 = true; manzana2 = true; manzana3 = true; manzana4 = true; posX = 0; posZ = 0; panel5 = false; contpuntos = 0; break;
        case 'b': case 'B': moveSpeed = 25.0f; break;
		case ' ': if (!isJumping) {          // Solo iniciar salto si no está saltando
            	isJumping = true;
        }break;
case 'c': case 'C' :moveSpeed += 10.0f;
break;
        
    }
    glutPostRedisplay();
}

// ########################## MOVIMIENTO Y VOLUMEN ########################## // 

void teclasEspeciales(int key, int x, int y) {
	
    switch (key) {
        case GLUT_KEY_UP:
            posZ += moveSpeed; // adelante pingüino
			anguloZPengu =0.0f;
            break;
        case GLUT_KEY_DOWN:
            posZ -= moveSpeed; // atras pingüino
            anguloZPengu =180.0f;
            break;
        case GLUT_KEY_LEFT:
            posX += moveSpeed; // izquierda pingüino
            anguloZPengu =90.0f;
            break;
        case GLUT_KEY_RIGHT:
            posX -= moveSpeed; // derecha pingüino
            anguloZPengu =270.0f;
            break;
        case GLUT_KEY_PAGE_UP: // más volúmen
        	increaseVolume();
            break;
        case GLUT_KEY_PAGE_DOWN: // menos volúmen
        	decreaseVolume();
            break;
    }
    glutPostRedisplay(); // Redibujar escena
}

// rotacion de manzana
void idle(){
	angulo +=0.5;
	display();
}


int main(int argc, char** argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600,600);
	glutInitWindowPosition(100,100);
	glutCreateWindow("PROYECTO FINAL");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(20, timer, 0);
	
    glutKeyboardFunc(keyboard);        // Manejo de teclas regulares
	glutSpecialFunc(teclasEspeciales); // Manejo de teclas especiales

	PlaySound(TEXT("COMPGRAF"), NULL, SND_FILENAME | SND_ASYNC);
	glutIdleFunc(idle);
	glutMainLoop();
	
	
	return 0;
}
