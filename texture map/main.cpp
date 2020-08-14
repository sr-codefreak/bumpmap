#include <GL/glut.h>
#include <GL/freeglut.h>
#include <fstream> 				///For file reading
#include <sstream>				///For converting string to integer
#include <bits/stdc++.h>
#define E 0.01					///E is the difference between consecutive parameters.

using namespace std; 

int mode;		/*<1 for planar and 2 for spherical surface*/

///Color array
float mycolors [10][4] = {
	{1.0,0.0,0.0,1.0},//red-0
	{0.0,1.0,0.0,1.0},//green-1
	{0.0,0.0,1.0,1.0},//blue-2
	{1.0,1.0,1.0,0.5},//white-3
	{0.0,0.0,0.0,1.0}, //black-4
	{1.0,0.65,0.0,1.0},//orange-5
	{0.6,0.6,0.6,1.0}, //gray-6
	{1,1,0,1},//yellow-7
	{0.1,1,0.1,1},
	{0,0.39,0,1}
};

double radius = 1;				/*<Radius of the sphere*/
int farray[101][101];			/*<farray values*/
map <pair<double,double>, double> fval, fu, fv;		
///Following are used for rotating and zooming.
double rotate_y=0; 
double rotate_x=0;
double rotate_z=0;
double zoom=1;


/**User defined string to integer function.
*/
int stoiUser(string s){		
	stringstream stream(s); 	///This uses a stream buffer
	int x = 0; 
	stream >> x; 			///Integer fetched from the buffer
	return x;
}


/**Reads the file and initializes farray[][].
*/
void generateFarray() {
	ifstream fin;
	fin.open("gravel.txt");
	string line;
	getline(fin, line); 				///The first line has the array-size and is thus ignored.
	
	int i=1,j=1;
	while (fin) { 						///Detection of end of file.
        getline(fin, line); 
        farray[i][j]=stoiUser(line);	///Parsing to integer.
        if(i==101){
        	break;
        	///Breaks out of when rows are over.
		}
        if(j==100){
        	///Goes to new row after columns are over.
        	j=0;
        	i++;
		}
		else 
			j++;
    } 
    
	fin.close();
}


/**Generates fval, fu and fv values from farray.
*/
void fvalGen(){
	for(double u=0; u<1; u+=0.01){
		for(double v=0; v<1; v+=0.01){
			///For integer indices
			int iu=100*u, iv=100*v;
			float du=100*u-iu, dv=100*v-iv, f00,f10,f01,f11,fu0,fu1;
			///Bilinear interpolation
			f00=farray[iu+1][iv+1];
			f10=farray[iu+2][iv+1];
			f01=farray[iu+1][iv+2];
			f11=farray[iu+2][iv+2];
			fu0=f00+du*(f10-f00);
			fu1=f01+du*(f11-f01);	
			fval[make_pair(u,v)] = fu0+dv*(fu1-fu0);
		}
	}
	///Calculation of derivative.
	for(double u=0; u<1; u+=0.01) {
		for(double v=0.01; v<=0.98; v+=0.01){
			fv[make_pair(u,v)]= (fval[make_pair(u,v+E)]-fval[make_pair(u,v-E)]) / (2*E);
		}
		///Border values are made same to their neighbours.
		fv[make_pair(u,0.00)] = fv[make_pair(u,0.01)];
		fv[make_pair(u,0.99)] = fv[make_pair(u,0.98)];
	}
	for(double v=0; v<1; v+=0.01) {
		for(double u=0.01; u<=0.98; u+=0.01){
			fu[make_pair(u,v)]= (fval[make_pair(u+E,v)]-fval[make_pair(u-E,v)]) / (2*E);
		}
		fu[make_pair(0.00,v)] = fu[make_pair(0.01,v)];
		fu[make_pair(0.99,v)] = fu[make_pair(0.98,v)];
	}	
}


/**Calculates cross product of vect_A and vect_B and stores it in cross_P.
*/
void crossProduct(double vect_A[], double vect_B[], double cross_P[]) { 
    cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1]; 
    cross_P[1] = vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2]; 
    cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0]; 
} 


/**Normal perturbation calcualtions.
*/
void normalPerturbation(){
	///For each pair of parameters.
	for(double u=0; u<1; u+=0.01){
		for(double v=0; v<1; v+=0.01){
			///D = ( Fu (N x Pv) - Fv (N x Pu) ) / INI
			double N[3], Pu[3], Pv[3], a[3], b[3], newN[3], modN;
			if(mode==2){
				///Parametric functions for the spherical surface.
				double x = radius*u*v;
				double y = radius*sqrt(1-u*u)*v;
				double z = radius*sqrt(1-v*v);
				N[0] = x, N[1] = y, N[2] = z;
				modN = sqrt(x*x+y*y+z*z);
				Pu[0] = radius*v;
				Pu[1] = ((-1)*radius*u*v) / (sqrt(1-u*u));
				Pu[2] = 0;
				Pv[0] = radius*u;
				Pv[1] = radius*sqrt(1-u*u);
				Pv[2] = ((-1)*radius*v) / (sqrt(1-v*v));
			}
			else {
				///We are drawing a square of side 0.5 units.
				double x=u/2;
				double y=v/2;
				double z=0;
				N[0] = x, N[1] = y, N[2] = z;
				modN = sqrt(x*x+y*y+z*z);
				Pu[0] = 1;
				Pu[1] = 0;
				Pu[2] = 0;
				Pv[0] = 0;
				Pv[1] = 1;
				Pv[2] = 0;
			}
			crossProduct(N, Pv, a);
			crossProduct(N, Pu, b);
			///newN calculated
			for(int i=0; i<3; i++) {
				newN[i] = N[i] + (fu[make_pair(u,v)]*a[i] - fv[make_pair(u,v)]*b[i]) / modN;
			}
			double modNewN = sqrt(newN[0]*newN[0] + newN[1]*newN[1] + newN[2]*newN[2]);
			///Making it a unit normal.
			for(int i=0; i<3; i++) {
				newN[i] /= modNewN;
			}
			///Drawing points and assigning normals.
			glBegin(GL_POINTS);
			glNormal3f(newN[0], newN[1], newN[2]);
			glVertex3f(N[0], N[1], N[2]);
			glEnd();
		}
	}
}


/**Keyboard controls to rotate and zoom.
*/
void processNormalKeys(unsigned char key, int x, int y) {

	switch(key){
		case 'a': rotate_y-=3;break;
		case 's': rotate_x+=3;break;
		case 'w': rotate_x-=3;break;
		case 'd': rotate_y+=3;break;
		///Figure is reversed if zoom becomes negative.
		case '-': if(zoom>0.01)	zoom-=0.03;break;	
		case '+': zoom+=0.03;break;	
		case 'z': rotate_z+=3;break;
		case 'x': rotate_z-=3;break;
		case 'q': rotate_x=rotate_y=rotate_z=0; 
				  zoom=1; break;
	};
	
	glutPostRedisplay();	
}


/**Initialises light and others.
*/
void init(){
	
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mycolors[3]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mycolors[5]);
    glMaterialf(GL_FRONT, GL_SHININESS, 30);

    glLightfv(GL_LIGHT0, GL_AMBIENT, mycolors[6]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, mycolors[3]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, mycolors[5]);
    glLightfv(GL_LIGHT0, GL_POSITION, mycolors[3]);

    glEnable(GL_LIGHTING);                // so the renderer considers light
    glEnable(GL_LIGHT0);                  // turn LIGHT0 on
    glEnable(GL_DEPTH_TEST);              // so the renderer considers depth
	glEnable(GL_NORMALIZE);				//to set normals
    glShadeModel(GL_SMOOTH);			//setting shading model
}


/**Function is recalled and displays the output.
*/
void displayer(){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glPushMatrix();
	gluLookAt(0,0,3,0,0,0,0,1,0);
	
	///Tranformations
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);
	glRotatef(rotate_z, 0.0, 0.0, 1.0);
	glScalef(zoom,zoom,zoom);
    
	glPushMatrix();
	normalPerturbation();
	
	///Drawing coordinate axis for easier viewing.
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(2,0,0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0,2,0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0,0,2);
	glEnd();
	
	glPopMatrix();
	
	glPopMatrix();
	
	glFlush();
	glutSwapBuffers();
}


/**Reshape setup.
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
	cout<<"1. Planar surface\n2. Spherical surface\n";
	cout<<"Input mode\n";
	cin>>mode;	
	glutCreateWindow("BUMP MAPPING");
	init();
	glEnable(GL_DEPTH_TEST);
		
		
	generateFarray();
	fvalGen();
		
	glutDisplayFunc(displayer);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(processNormalKeys);
		
	
	glutMainLoop();
	return 0;
}


