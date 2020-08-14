#include <GL/glut.h>
#include <GL/freeglut.h>
#include <bits/stdc++.h>
using namespace std; 

//array which defines various colors
float mycolors [10][4] = {
	{1.0,0.0,0.0,1.0},//red-0
	{0.0,1.0,0.0,1.0},//green-1
	{0.0,0.0,1.0,1.0},//blue-2
	{1.0,1.0,1.0,1.0},//white-3
	{0.0,0.0,0.0,1.0}, //black-4
	{1.0,0.65,0.0,1.0},//orange-5
	{0.6,0.6,0.6,1.0}, //gray-6
	{1,1,0,1},//yellow-7
	{0.1,1,0.1,1},
	{0,0.39,0,1}
};

//related to zoom and rotate
double rotate_y=0; 
double rotate_x=0;
double rotate_z=0;
double zoom=1;
float pos[200][7];

/**assigns normal vector to a point
*/
void changeNormal(float x1, float y1, float *n1){
	glBegin(GL_POINTS);
	glNormal3f(n1[0],n1[1],n1[2]);
	glVertex3f(n1[0],n1[1],0);
	glEnd();	
}

/** gives angle between the two vectors u and v
*/
float angleBetween(float *u, float *v){
	return acos((u[0]*v[0]+u[1]*v[1]+u[2]*v[2])/(sqrt(u[0]*u[0]+u[1]*u[1]+u[2]*u[2])*sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2])));
}

/**draws a bump at the origin with r as radius and m as the other constraint
*/
void Bumper(float r, float m){
	float x,y,z,n1[3];
	for(x=-r;x<=r;x+=m){
		for(y=-r;y<=r;y+=m){
			z=sqrt(r*r-x*x-y*y);
			if(z>m)	{			
				n1[0]=x;
				n1[1]=y;
				n1[2]=z;					

				changeNormal(x,y,n1);
			}
		}
	}
	
}

/**Finds cross product of vect_A and vect_B and stores it in cross_P
*/
void crossProduct(float vect_A[], float vect_B[], float cross_P[]) { 
    cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1]; 
    cross_P[1] = vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2]; 
    cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0]; 
} 

/**Draws all the bumps related symmetrically on a sphere
*/
void symmetricBumper(){
	float r=1,m=0.3,angle,v[3],u[3]={0,0,1},n[3];
	glutSolidSphere(r,100,20);
	for(v[0]=-r;v[0]<=r;v[0]+=m){
		for(v[1]=-r;v[1]<=r;v[1]+=m){		
			v[2]=sqrt(r*r-v[0]*v[0]-v[1]*v[1]);
			crossProduct(u,v,n);
			angle=angleBetween(u,v);
			glPushMatrix();			
			glTranslatef(v[0],v[1],v[2]);
			glRotatef(angle*180.0/3.14,n[0],n[1],n[2]);
			Bumper(0.05,0.001);		
			glPopMatrix();
			
			v[2]=-sqrt(r*r-v[0]*v[0]-v[1]*v[1]);
			crossProduct(u,v,n);
			angle=angleBetween(u,v);
			glPushMatrix();			
			glTranslatef(v[0],v[1],v[2]);
			glRotatef(angle*180.0/3.14,n[0],n[1],n[2]); 
			Bumper(0.05,0.001);		
			glPopMatrix();
		}
	}
}

/**Generates a random number between a and b
*/
int random(int a, int b){
	return (a+rand()%b);
}

/**Generates random postions and stores it in pos[][] array
*/
void genpos(){
	float r=1;
	for(int i=0;i<100;++i){
		pos[i][0]=(float(random(0,2000))/1000)-1.0;
		pos[i][1]=(float(random(0,2000))/1000)-1.0;
		pos[i][2]=sqrt(r*r-pos[i][0]*pos[i][0]-pos[i][1]*pos[i][1]);
		pos[i][3]=(float(random(0,1000))/10000);
		pos[i][4]=(float(random(0,2000))/1000)-1.0;
		pos[i][5]=(float(random(0,2000))/1000)-1.0;
		pos[i][6]=(float(random(0,2000))/1000)-1.0;
	}
	for(int i=100;i<200;++i){
		pos[i][0]=(float(random(0,2000))/1000)-1.0;
		pos[i][1]=(float(random(0,2000))/1000)-1.0;
		pos[i][2]=-1*sqrt(r*r-pos[i][0]*pos[i][0]-pos[i][1]*pos[i][1]);
		pos[i][3]=(float(random(0,1000))/10000);
		pos[i][4]=(float(random(0,2000))/1000)-1.0;
		pos[i][5]=(float(random(0,2000))/1000)-1.0;
		pos[i][6]=(float(random(0,2000))/1000)-1.0;
	}
}

/**tries to make a bumped orange
*/
void orange(){
	float r=1,m=0.3,angle,u[3]={0,0,1},n[3];
	glutSolidSphere(r,100,20);
	for(int i=0;i<200;++i){		
		crossProduct(u,pos[i],n);
		angle=angleBetween(u,pos[i]);cout<<angle<<"\n";
		glPushMatrix();			
		glTranslatef(pos[i][0],pos[i][1],pos[i][2]);
		glRotatef(angle*180.0/3.14,n[0],n[1],n[2]);
		glScalef(pos[i][4],pos[i][5],0);
		Bumper(pos[i][3],0.001);
		cout<<pos[i][3];
		glPopMatrix();
	}
}

/**Keyboard controls to rotate and zoom
*/
void processNormalKeys(unsigned char key, int x, int y) {
	switch(key){
		case 'a': rotate_y-=1;break;
		case 's': rotate_x+=1;break;
		case 'w': rotate_x-=1;break;
		case 'd': rotate_y+=1;break;
		///Figure is reversed if zoom becomes negative.
		case '-': if(zoom>0.01)	zoom-=0.01;break;	
		case '+': zoom+=0.01;break;	
		case 'z': rotate_z+=1;break;
		case 'x': rotate_z-=1;break;
		case 'q': rotate_x=rotate_y=rotate_z=0; 
				  zoom=1; break;
	};		
	glutPostRedisplay();	
}

/**initialises light and others
*/
void init(){
	
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mycolors[5]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mycolors[5]);
    glMaterialf(GL_FRONT, GL_SHININESS, 30);

    glLightfv(GL_LIGHT0, GL_AMBIENT, mycolors[7]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, mycolors[5]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, mycolors[6]);
    glLightfv(GL_LIGHT0, GL_POSITION, mycolors[3]);

    glEnable(GL_LIGHTING);                // so the renderer considers light
    glEnable(GL_LIGHT0);                  // turn LIGHT0 on
    glEnable(GL_DEPTH_TEST);              // so the renderer considers depth
	glEnable(GL_NORMALIZE);
	
}

/**Function is recalled and displays the output
*/
void displayer(){	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity;	
	glPushMatrix();
	gluLookAt(0,0,3,0,0,0,0,1,0);
	///Tranformations	
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	glRotatef(rotate_z, 0.0, 0.0, 1.0);
	glScalef(zoom,zoom,zoom);
	symmetricBumper();
	//orange();
	glPopMatrix();	
	glFlush();
	glutSwapBuffers();
}

/**reshape setup
*/
void reshape(GLint w, GLint h){
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 0.1f, 500.0);
	glMatrixMode(GL_MODELVIEW);	
	
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("BUMP MAPPING");
	init();
	glEnable(GL_DEPTH_TEST);
			
	glutDisplayFunc(displayer);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(processNormalKeys);
	
	genpos();
	glutMainLoop();
	return 0;
}

