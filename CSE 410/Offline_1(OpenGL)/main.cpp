#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<math.h>
#include<time.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

#define slength 100
#define konst 4
#define rdeg 4

using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
int rotatesphere;
double angle,angle2,angle3,angle4;
double bigrad, smallrad;
int slices,stacks;
double xv,yv,zv;
double dx[5],dy[5];
bool pause;
bool created[5];
bool entered[5];
const clock_t btime = clock();
double et;
int all;

struct point
{
	double x,y,z;

	point(){}

	point(double a,double b,double c)
	{
	    x = a; y = b;  z = c;
	}

	void setpoint(double a,double b,double c)
	{
	    x = a; y = b;  z = c;
	}
};

struct point positions[5];
double speeds[5];
double angles[5];
int incount[5];


struct point pos = point(0,0,200);
struct point n;
struct point d;
struct point rf;

struct point u = point(0,1,0);
struct point r = point(1,0,0);
struct point l = point(0,0,1);

struct point addpoint(struct point p1, struct point p2)
{
    struct point r = point(p1.x+p2.x, p1.y+p2.y, p1.z+p2.z);
    return r;
};

struct point subpoint(struct point p1, struct point p2)
{
    struct point r = point(p1.x-p2.x, p1.y-p2.y, p1.z-p2.z);
    return r;
};

struct point scaler_mult(double k,struct point p1)
{
    struct point r = point(k*p1.x, k*p1.y, k*p1.z);
    return r;
};

struct point rotate_vector(struct point v, struct point axis, struct point other, double deg, bool add)
{
    struct point p1 = scaler_mult(cos(deg*(pi/180.0)),v);
    struct point p2 = scaler_mult(sin(deg*(pi/180.0)),other);

    if (add)
        return addpoint(p1,p2);
    else
        return subpoint(p1,p2);
};

double vecval(struct point v)
{
    return sqrt(v.x*v.x +v.y*v.y + v.z*v.z);
}

double dist(struct point p1, struct point p2)
{
    return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
}


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 1000,0,0);
			glVertex3f(-1000,0,0);

			glVertex3f(0,-1000,0);
			glVertex3f(0, 1000,0);

			glVertex3f(0,0, 1000);
			glVertex3f(0,0,-1000);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    glColor3f(0.0,1.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}

void drawmySquare(double a)
{
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);{
        glVertex3f( a/2,a/2,0);
        glVertex3f(-a/2,a/2,0);

        glVertex3f( a/2,a/2,0);
        glVertex3f(a/2,-a/2,0);

        glVertex3f( -a/2,a/2,0);
        glVertex3f(-a/2,-a/2,0);

        glVertex3f( -a/2,-a/2,0);
        glVertex3f(a/2,-a/2,0);
    }glEnd();
}


void drawmyCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        //glColor3f(1,0,0);
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawupperSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
		    glColor3f(j%2,j%2,j%2);
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawlowerSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    glColor3f(j%2,j%2,j%2);
			glBegin(GL_QUADS);{
			    //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCyllinder(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
		    glColor3f(j%2,j%2,j%2);
			glBegin(GL_QUADS);{
			    //upper hemisphere
				/*glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);*/
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawinvertSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		r = 2*radius - r;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    glColor3f(j%2,j%2,j%2);
			glBegin(GL_QUADS);{
			    //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void mydraw()
{
    drawmySquare(slength);
    glColor3f(1,0,0);
    drawmyCircle(bigrad,slices);

    glColor3f(1,0.5,0);
    /*for(int c=0;c<5;c++)
    {
        glPushMatrix();
        glTranslatef(positions[c].x,  positions[c].y,  0);
        drawmyCircle(smallrad,slices);
        glPopMatrix();
    }*/

    et = (clock() - btime)/CLOCKS_PER_SEC;
    for(int c=0;c<5;c++)
    {
        if(et >= c)
        {
            //cout<<"hhh";
            glPushMatrix();
            glTranslatef(positions[c].x,  positions[c].y,  0);
            drawmyCircle(smallrad,slices);
            glPopMatrix();
            created[c] = 1;
        }
    }
}

void drawSS()
{
    glColor3f(1,0,0);
    drawSphere(20,100,20);


    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '0':
			drawgrid=1-drawgrid;
			break;

        case 'p':
            pause = 1 - pause;
            //dx = 0;
            //dy = 0;
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
            for(int c=0;c<5;c++)
            {
                speeds[c] -= 0.001;
                if(speeds[c]<=0.001)
                    speeds[c] = 0.001;
            }
			break;
		case GLUT_KEY_UP:		// up arrow key
            for(int c=0;c<5;c++)
            {
                speeds[c] += 0.001;
                if(speeds[c]>=0.1)
                    speeds[c] = 0.1;
            }
			break;

		case GLUT_KEY_RIGHT:

			break;
		case GLUT_KEY_LEFT:

			break;

		case GLUT_KEY_PAGE_UP:

			break;
		case GLUT_KEY_PAGE_DOWN:

			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(pos.x,pos.y,pos.z,    0,0,0,	u.x,u.y,u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();
    mydraw();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
    for(int c=0; c<5;c++)
    {
        d.setpoint(dx[c] , dy[c], 0);
        n.setpoint(0-positions[c].x , 0-positions[c].y, 0);
        double nv = vecval(n);
        n.setpoint((0-positions[c].x)/nv, (0-positions[c].y)/nv, 0);
        r = subpoint(d, scaler_mult(2*(dx[c]*n.x + dy[c]*n.y),n));
        double nar = acos((n.x*r.x + n.y*r.y)/(nv*vecval(r)));

        if((positions[c].x+smallrad >= slength/2 || positions[c].x-smallrad <= -slength/2) && (positions[c].y+smallrad >= slength/2 || positions[c].y-smallrad <= -slength/2))
            angles[c] = 180+angles[c];
        if(positions[c].x+smallrad >= slength/2 || positions[c].x-smallrad <= -slength/2)
            angles[c] = 180-angles[c];
        if(positions[c].y+smallrad >= slength/2 || positions[c].y-smallrad <= -slength/2)
            angles[c] = -angles[c];

        if(sqrt(positions[c].x * positions[c].x + positions[c].y * positions[c].y)+smallrad <= bigrad)
        {
            entered[c] = 1;
            if((sqrt(positions[c].x * positions[c].x + positions[c].y * positions[c].y)+smallrad)>bigrad-1 &&
               (sqrt(positions[c].x * positions[c].x + positions[c].y * positions[c].y)+smallrad)<bigrad+1 && nar<=pi/2)
            {
                angles[c] += nar*(180/pi);
            }

            for(int j=0; j<5; j++)
            {
                double dst,nv2,nar2;
                struct point nn,dd,rr;
                if(c!=j && entered[j])
                {
                    dst = dist(positions[c],positions[j]);
                    if(2*smallrad > dst-0.1 && 2*smallrad < dst + 0.1)
                    {
                        nn = subpoint(positions[c],positions[j]);
                        nv2 = vecval(nn);
                        nn.setpoint(nn.x/nv2, nn.y/nv2, 0);
                        rr = subpoint(d, scaler_mult(2*(dx[c]*nn.x + dy[c]*nn.y),nn));
                        nar2 = acos((nn.x*rr.x + nn.y*rr.y)/(nv2*vecval(rr)));
                        if(nar2<=pi/2)
                            angles[c] += nar2*(180/pi);

                        nn = subpoint(positions[j],positions[c]);
                        nv2 = vecval(nn);
                        nn.setpoint(nn.x/nv2, nn.y/nv2, 0);
                        rr = subpoint(d, scaler_mult(2*(dx[j]*nn.x + dy[j]*nn.y),nn));
                        nar2 = acos((nn.x*rr.x + nn.y*rr.y)/(nv2*vecval(rr)));
                        if(nar2<=pi/2)
                            angles[j] += nar2*(180/pi);
                    }
                }
            }
        }

        if(!pause && created[c])
        {
            dx[c] = speeds[c]*cos(angles[c]*(pi/180));
            dy[c] = speeds[c]*sin(angles[c]*(pi/180));

            positions[c].x += dx[c];
            positions[c].y += dy[c];
        }
    }
	//angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=0;
	rotatesphere = 0;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	angle2=0;
	angle3=0;
	angle4=0;
	pause = 0;

    all = 0;
    et = 0;
	bigrad = 35;
	smallrad = 6;
	slices = 30;
	xv=0;yv=0;zv=0;
	for(int k =0; k<5; k++)
    {
        positions[k].x = -slength/2 + smallrad;
        positions[k].y = -slength/2 + smallrad;
        speeds[k] =  0.005;
        angles[k] =  ((int) rand()%90);
        incount[k] = 0;
        dx[k] = 0;
        dy[k] = 0;
        created[k] = 0;
        entered[k] = 0;
        //cout<<xspeeds[k]<<","<<yspeeds[k]<<"\n";
    }

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(400,	1,	1,	10000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
	//et = (clock() - btime)/CLOCKS_PER_SEC;
	//cout<<et;
}

int main(int argc, char **argv){
    srand(time(0));
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
