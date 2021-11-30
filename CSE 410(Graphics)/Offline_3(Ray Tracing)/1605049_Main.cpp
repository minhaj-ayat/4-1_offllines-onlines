#include "1605049_Header.h";

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
int rotatesphere;
double angle,angle2,angle3,angle4;
double bigrad, smallrad;
int slices,stacks;


int rec_level, image_size, number_of_objects, number_of_lights;
double floor_width, tile_width, fovY;
vector<Object *> objects;
vector<Light> lights;
bitmap_image image;


struct point
{
    double x,y,z;

    point(){}

    point(double a,double b,double c)
    {
        x = a; y = b;  z = c;
    }
};

struct point pos = point(0,-300,70);

struct point u = point(0,1,0);
struct point r = point(1,0,0);
struct point l = point(0,0,-1);

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



void readObject(string& obj, ifstream& sceneFile)
{
    string line;
    if(obj == "sphere" || obj == "sphere ")
    {
        double x,y,z, radius, red,green,blue, amb,diff,spec,rec;
        Vector3D center;
        int shine;
        for(int i=1; i<=5; i++)
        {
            if(i == 1)
            {
                getline(sceneFile,line);
                //cout<<"1st sphere center : "<<line<<endl;
                stringstream ss(line);
                ss>>x; ss>>y; ss>>z;
                center.setVector(x,y,z);
            }
            else if(i == 2)
            {
                getline(sceneFile,line);
                //cout<<"1st sphere radius : "<<line<<endl;
                stringstream ss(line);
                ss>>radius;
            }
            else if(i == 3)
            {
                getline(sceneFile,line);
                //cout<<"1st sphere color : "<<line<<endl;
                stringstream ss(line);
                ss>>red; ss>>green; ss>>blue;
            }
            else if(i == 4)
            {
                getline(sceneFile,line);
                //cout<<"1st sphere coeff : "<<line<<endl;
                stringstream ss(line);
                ss>>amb; ss>>diff; ss>>spec; ss>>rec;
            }
            else if(i == 5)
            {
                getline(sceneFile,line);
                //cout<<"1st sphere shine : "<<line<<endl;
                stringstream ss(line);
                ss>>shine;
            }
        }

        Object* temp = new Sphere(center,radius);
        temp->setColor(red,green,blue);
        temp->setCoefficients(amb,diff,spec,rec);
        temp->setShine(shine);
        //temp->print();

        objects.push_back(temp);
    }

    else if(obj == "triangle" || obj == "triangle ")
    {
        double x,y,z, radius, red,green,blue, amb,diff,spec,rec;
        Vector3D v1,v2,v3;
        int shine;
        for(int i=1; i<=6; i++)
        {
            if(i == 1 || i == 2 || i == 3)
            {
                getline(sceneFile,line);
                stringstream ss(line);
                ss>>x; ss>>y; ss>>z;
                if(i==1)
                    v1.setVector(x,y,z);
                else if(i==2)
                    v2.setVector(x,y,z);
                else if(i==3)
                    v3.setVector(x,y,z);
            }

            else if(i == 4)
            {
                getline(sceneFile,line);
                stringstream ss(line);
                ss>>red; ss>>green; ss>>blue;
            }
            else if(i == 5)
            {
                getline(sceneFile,line);
                stringstream ss(line);
                ss>>amb; ss>>diff; ss>>spec; ss>>rec;
            }
            else if(i == 6)
            {
                getline(sceneFile,line);
                stringstream ss(line);
                ss>>shine;
            }
        }

        Object* temp = new Triangle(v1,v2,v3);
        temp->setColor(red,green,blue);
        temp->setCoefficients(amb,diff,spec,rec);
        temp->setShine(shine);
        //temp->print();

        objects.push_back(temp);

    }

    else if(obj == "general" || obj == "general ")
    {
        double x,y,z,  A,B,C,D,E,F,G,H,I,J, len,wid,ht, red,green,blue, amb,diff,spec,rec;
        Vector3D cube_ref_point;
        int shine;
        for(int i=1; i<=5; i++)
        {
            if(i == 1)
            {
                getline(sceneFile,line);
                stringstream ss(line);
                ss>>A; ss>>B; ss>>C; ss>>D; ss>>E; ss>>F; ss>>G; ss>>H; ss>>I; ss>>J;
            }

            else if(i == 2)
            {
                getline(sceneFile,line);
                stringstream ss(line);
                ss>>x; ss>>y; ss>>z; ss>>len; ss>>wid; ss>>ht;
                cube_ref_point.setVector(x,y,z);
            }
            else if(i == 3)
            {
                getline(sceneFile,line);
                stringstream ss(line);
                ss>>red; ss>>green; ss>>blue;
            }
            else if(i == 4)
            {
                getline(sceneFile,line);
                stringstream ss(line);
                ss>>amb; ss>>diff; ss>>spec; ss>>rec;
            }
            else if(i == 5)
            {
                getline(sceneFile,line);
                stringstream ss(line);
                ss>>shine;
            }
        }

        Object* temp = new General(A,B,C,D,E,F,G,H,I,J,cube_ref_point,len,wid,ht);
        temp->setColor(red,green,blue);
        temp->setCoefficients(amb,diff,spec,rec);
        temp->setShine(shine);
        //temp->print();

        objects.push_back(temp);
    }
}



void loadData()
{
    string line;
    ifstream sceneFile;
    sceneFile.open("scene.txt");
    int counter = 1;
    int objectCount = 0;

    while(getline(sceneFile, line)) {
        if(counter == 1)
        {
            stringstream ss(line);
            ss >> rec_level;
            counter++;
            cout<<"Level:"<<rec_level<<endl;
        }

        else if(counter == 2)
        {
            stringstream ss(line);
            ss >> image_size;
            counter++;
            cout<<"Pixels Size : "<<image_size<<endl;
            getline(sceneFile,line);
        }

        else if(counter == 3)
        {
            stringstream ss(line);
            ss >> number_of_objects;
            counter++;
            cout<<"Number of objects : "<<number_of_objects<<endl;
        }

        else if(objectCount < number_of_objects)
        {
            //cout<<"C4 Line: "<<line<<endl;
            readObject(line, sceneFile);
            objectCount++;
            getline(sceneFile,line);
        }

        else {
            stringstream ss(line);
            ss >> number_of_lights;
            cout<<"Number of lights : "<<number_of_lights<<endl;

            for(int i=0; i<number_of_lights; i++)
            {
                double x,y,z, red,green,blue;
                Vector3D lpos;
                getline(sceneFile,line);
                stringstream ss1(line);
                ss1 >> x; ss1 >> y; ss1 >> z;
                lpos.setVector(x,y,z);

                getline(sceneFile,line);
                stringstream ss2(line);
                ss2 >> red; ss2 >> green; ss2 >> blue;

                Light light(lpos,red,green,blue);
                lights.push_back(light);
            }
        }

    }

    Object* temp = new Floor(1000,20);
    temp->setColor(1,0,0);
    temp->setCoefficients(1,1,1,1);
    temp->setShine(1);
    //temp->print();

    objects.push_back(temp);

}

void printData()
{
    for (auto i = objects.begin(); i != objects.end(); ++i)
    {
        //cout<<"object\n";
        (*i)->print();
    }

    for (auto i = lights.begin(); i != lights.end(); ++i)
    {
        i->print();
    }
}

void capture()
{
    image.setwidth_height(image_size,image_size);
    for(int i=0;i<image_size;i++){
        for(int j=0;j<image_size;j++){
            image.set_pixel(i,j,0,0,0);
        }
    }
    struct point tpos(-l.x,-l.y,-l.z);

    double planeDistance = (image_size/2.0)/(tan((fovY/2)*(3.1416/180)));
    struct point t1 = addpoint(pos,scaler_mult(planeDistance,l));
    struct point t2 = subpoint(t1,scaler_mult((image_size/2.0),r));
    struct point topleft = addpoint(t2,scaler_mult((image_size/2.0),u));

    double du = image_size/image_size;
    double dv = image_size/image_size;

    struct point t3 = addpoint(topleft,scaler_mult(0.5*du,r));
    topleft = subpoint(t3,scaler_mult(0.5*dv,u));
    //cout<<topleft.x<<"::"<<topleft.y<<"::"<<topleft.z<<endl;
    struct point topleft2(topleft.x,topleft.y,topleft.z);

    int nearest;
    Object nearestObj;
    double t, tMin = 10000;
    double *dummy;
    bool Hit;
    cout<<"Size:"<<objects.size()<<endl;
    for(int i=0;i<image_size;i++)
    {
        for(int j=0;j<image_size;j++)
        {
            struct point a = subpoint(scaler_mult(i*du,r),scaler_mult(j*dv,u));
            struct point curPixel = addpoint(topleft,a);
            a = subpoint(curPixel,pos);
            Ray ray(pos.x,pos.y,pos.z,a.x,a.y,a.z);

            Hit = false;
            tMin = 10000;
            for (int p = 0; p < objects.size(); p++)
            {
                Object *o = objects.at(p);
                t = (*o).intersect(ray,dummy,0);
                if(t>0 && t < tMin)
                {
                    tMin = t;
                    nearest = p;
                    Hit = true;

                    /*if(t<350)
                        cout<<"WHY???????"<<endl;*/
                }
                else if(t!=-1) {
                    Hit = true;
                }
            }
            if(!Hit)
            {
                image.set_pixel(i,j,0,0,0);
            }
            else
            {
                //cout<<"in else"<<endl;
                Object *o2 = objects.at(nearest);
                double *col = new double[3];
                tMin = (*o2).intersect(ray,col,1);
                image.set_pixel(i,j,col[0]*255,col[1]*255,col[2]*255);
                //delete[] col;
            }
        }
    }
    image.save_image("test.bmp");
    cout<<"DONE!!"<<endl;
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
    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);{
        glVertex3f( a, a,2);
        glVertex3f( a,-a,2);
        glVertex3f(-a,-a,2);
        glVertex3f(-a, a,2);
    }glEnd();
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
    double h,rd;
    //generate points
    for(i=0;i<=stacks;i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        rd=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0;j<=slices;j++)
        {
            points[i][j].x=rd*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=rd*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0;i<stacks;i++)
    {
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
    //glColor3f(1,1,1);
    glPushMatrix();
    {
        glRotatef(angle,0,1,0);
        drawupperSphere(bigrad,slices,stacks);
        //drawlowerSphere(50,20,15);
    }

    {
        glRotatef(angle2,1,0,0);
        drawlowerSphere(bigrad,slices,stacks);
    }

    {
        glTranslatef(0,0,-(bigrad+smallrad));
        glRotatef(angle3,1,0,0);
        glRotatef(angle4,0,0,1);
        drawupperSphere(smallrad,slices,stacks);
        for(int k=1;k<=10;k++)
        {
            drawCyllinder(smallrad,slices,stacks);
            glTranslatef(0,0,-(smallrad));
        }
        drawinvertSphere(smallrad,slices,stacks);
    }

    {
        glPopMatrix();
        glColor3f(1,1,0);
        glTranslatef(0,0,-(2*bigrad+11*smallrad+400));
        drawSquare(250);
    }
}

void newdraw()
{
    for (auto i = objects.begin(); i != objects.end(); ++i)
    {
        (*i)->draw();
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
            capture();
            break;

        case '7':
            drawgrid=1-drawgrid;
            break;

        case '1':
            l = rotate_vector(l,u,r,-rdeg,false);
            r = rotate_vector(r,u,l,-rdeg,true);
            cout<<pos.x<<","<<pos.y<<","<<pos.z<<endl;
            break;

        case '2':
            l = rotate_vector(l,u,r,rdeg,false);
            r = rotate_vector(r,u,l,rdeg,true);
            cout<<pos.x<<","<<pos.y<<","<<pos.z<<endl;
            break;

        case '3':
            l = rotate_vector(l,r,u,rdeg,true);
            u = rotate_vector(u,r,l,rdeg,false);
            cout<<pos.x<<","<<pos.y<<","<<pos.z<<endl;
            break;

        case '4':
            l = rotate_vector(l,r,u,-rdeg,true);
            u = rotate_vector(u,r,l,-rdeg,false);
            cout<<pos.x<<","<<pos.y<<","<<pos.z<<endl;
            break;

        case '5':
            r = rotate_vector(r,l,u,rdeg,false);
            u = rotate_vector(u,l,r,rdeg,true);
            cout<<pos.x<<","<<pos.y<<","<<pos.z<<endl;
            break;

        case '6':
            r = rotate_vector(r,l,u,-rdeg,false);
            u = rotate_vector(u,l,r,-rdeg,true);
            cout<<pos.x<<","<<pos.y<<","<<pos.z<<endl;
            break;


        case 'q':
            angle+=5;
            if(angle >= 45) angle = 45;
            break;

        case 'w':
            angle-=5;
            if(angle <= -45) angle = -45;
            break;

        case 'e':
            angle2+=5;
            if(angle2 >= 45) angle2 = 45;
            break;

        case 'r':
            angle2-=5;
            if(angle2 <= -45) angle2 = -45;
            break;

        case 'a':
            angle3+=5;
            if(angle3 >= 35) angle3 = 35;
            break;

        case 's':
            angle3-=5;
            if(angle3 <= -35) angle3 = -35;
            break;

        case 'd':
            angle4+=5;
            break;

        case 'f':
            angle4-=5;
            break;

        default:
            break;
    }
}


void specialKeyListener(int key, int x,int y){
    switch(key){
        case GLUT_KEY_DOWN:		//down arrow key
            pos = subpoint(pos,scaler_mult(konst,l));
            cout<<pos.x<<","<<pos.y<<","<<pos.z<<endl;
            break;
        case GLUT_KEY_UP:		// up arrow key
            pos = addpoint(pos,scaler_mult(konst,l));
            cout<<pos.x<<","<<pos.y<<","<<pos.z<<endl;
            break;

        case GLUT_KEY_RIGHT:
            pos = subpoint(pos,scaler_mult(konst,r));
            break;
            cout<<pos.x<<","<<pos.y<<","<<pos.z<<endl;
        case GLUT_KEY_LEFT:
            pos = addpoint(pos,scaler_mult(konst,r));
            cout<<pos.x<<","<<pos.y<<","<<pos.z<<endl;
            break;

        case GLUT_KEY_PAGE_UP:
            pos = addpoint(pos,scaler_mult(konst,u));
            break;
        case GLUT_KEY_PAGE_DOWN:
            pos = subpoint(pos,scaler_mult(konst,u));
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
    gluLookAt(pos.x,pos.y,pos.z,    pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);


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

    newdraw();
    //drawSS();
    //mydraw();

    //drawCircle(30,24);

    //drawCone(20,50,24);

    //drawSphere(30,24,20);




    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate(){
    //angle+=0.05;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init(){
    //codes for initialization
    drawgrid=0;
    drawaxes=1;
    rotatesphere = 0;
    cameraHeight=150.0;
    cameraAngle=1.0;
    angle=0;
    angle2=0;
    angle3=0;
    angle4=0;
    bigrad = 50;
    smallrad = 20;
    slices = 70;
    stacks = 70;

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
    fovY = 400;
    gluPerspective(fovY,	1,	1,	10000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

int main(int argc, char **argv){
    cout << setprecision(5) <<fixed;
    loadData();
    printData();
    glutInit(&argc,argv);
    glutInitWindowSize(image_size, image_size);
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
