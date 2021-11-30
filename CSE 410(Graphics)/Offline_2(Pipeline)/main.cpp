#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
#include <stack>
#include <vector>
#include <cmath>
#include <iomanip>
#include<algorithm>
#include "bitmap_image.hpp"


using namespace std;


class Point
{
public:
    vector<vector<double>> p;
    Point() {
        p = {
                {1},
                {1},
                {1},
                {1}
        };
    }

    Point(double x,double y,double z) {
        p = {
                {x},
                {y},
                {z},
                {1}
        };
    }

    void setPoint(double x,double y,double z) {
        p = {
                {x},
                {y},
                {z},
                {1}
            };
    }

    void setPoint(Point pt) {
        p = {
                {pt.p[0][0]},
                {pt.p[1][0]},
                {pt.p[2][0]},
                {pt.p[3][0]}
        };
    }

    vector<vector<double>> getPoint() {
        return p;
    }

    void printPoint() {
        for (int i = 0; i < p.size()-1; i++)
        {
            for (int j = 0; j < p[i].size(); j++)
            {
                cout << p[i][j] << " ";
            }
        }
        cout << endl;
    }

    void writePoint(ofstream& stage) {
        for (int i = 0; i < p.size()-1; i++)
        {
            for (int j = 0; j < p[i].size(); j++)
            {
                stage << p[i][j] << " ";
            }
        }
        stage << endl;
    }
};


class Vec
{
public:
    double x,y,z;

    Vec() {
        x=0;y=0;z=0;
    }

    Vec(double x1,double y1,double z1)
    {
        x=x1; y=y1; z=z1;
    }

    Vec subtract(Vec b)
    {
        Vec r(this->x - b.x, this->y - b.y, this->z - b.z);
        return r;
    }

    Vec add(Vec b)
    {
        Vec r(this->x + b.x, this->y + b.y, this->z + b.z);
        return r;
    }

    double dot(Vec b)
    {
        return this->x*b.x + this->y*b.y + this->z*b.z;
    }

    Vec cross(Vec b)
    {
        Vec r;
        r.x = this->y*b.z - this->z*b.y;
        r.y = this->z*b.x - this->x*b.z;
        r.z = this->x*b.y - this->y*b.x;
        return r;
    }

    double val()
    {
       return sqrt(x*x + y*y + z*z);
    }

    void normalize()
    {
        double v = this->val();
        if(v != 1)
        {
            this->x /= v;
            this->y /= v;
            this->z /= v;
        }
    }
};


class Triangle
{
public:
    Point points[3];
    int color[3];

    Triangle()
    {

    }

    void setTriangle(Point p0, Point p1, Point p2)
    {
        points[0].setPoint(p0);
        points[1].setPoint(p1);
        points[2].setPoint(p2);
        color[0] = rand()%255+1;
        color[1] = rand()%255+1;
        color[2] = rand()%255+1;
    }

    double maxY()
    {
        return max({points[0].p[1][0], points[1].p[1][0], points[2].p[1][0]});
    }

    double minY()
    {
        return min({points[0].p[1][0], points[1].p[1][0], points[2].p[1][0]});
    }

    double maxX()
    {
        return max({points[0].p[0][0], points[1].p[0][0], points[2].p[0][0]});
    }

    double minX()
    {
        return min({points[0].p[0][0], points[1].p[0][0], points[2].p[0][0]});
    }

    void printTriangle()
    {
        cout<<"Printing Triangle-------\n";
        for(int i=0;i<3;i++)
        {
            points[i].printPoint();
        }
    }

};

int pushCount = 0;

vector<Point> points;
vector<Triangle> triangles;
vector<vector<double>> zBuffer;
//global variables
double eyeX, eyeY, eyeZ;
double lookX, lookY, lookZ;
double upX, upY, upZ;
double fovY, aspect_ratio, near, far;
int fileChoice;
int screenWidth,screenHeight;
double left_limit,right_limit,top_limit,bottom_limit,front_end,rear_end;
double dx,dy, TopY,LeftX,BottomY,RightX;
bitmap_image image;

//global stack
stack<vector<vector<double>>> STACK;
stack<int> countStack;

stack<vector<vector<double>>> initStack() {
    vector<vector<double>> identity = {
            {1,0,0,0},
            {0,1,0,0},
            {0,0,1,0},
            {0,0,0,1}
    };

    STACK.push(identity);
    return STACK;
}

//initialize global variables
void init_globals(string s, int line)
{
    stringstream ss(s);
    if(line == 1)
    {
        ss >> eyeX; ss >> eyeY; ss >> eyeZ;
        cout << eyeX << "," << eyeY <<","<<eyeZ <<endl ;
    }
    else if(line == 2)
    {
        ss >> lookX; ss >> lookY; ss >> lookZ;
        cout << lookX << "," << lookY <<","<<lookZ <<endl;
    }
    else if(line == 3)
    {
        ss >> upX; ss >> upY; ss >> upZ;
        cout << upX << "," << upY <<","<<upZ <<endl;
    }
    else if(line == 4)
    {
        ss >> fovY; ss >> aspect_ratio; ss >> near; ss >> far;
        cout << fovY << "," << aspect_ratio <<","<<near << "," << far <<endl;
    }

    else if(line == 5)
    {
        ss >> screenWidth; ss >> screenHeight;
        cout << screenWidth << "," << screenHeight  <<endl;
    }
    else if(line == 6)
    {
        ss >> left_limit;
        right_limit = -left_limit;
        cout << left_limit <<endl;
    }
    else if(line == 7)
    {
        ss >> bottom_limit;
        top_limit = -bottom_limit;
        cout << bottom_limit <<endl;
    }
    else if(line == 8)
    {
        ss >> front_end; ss>>rear_end;
        cout << front_end <<"  "<<rear_end<<endl;
    }

}


Point transformPoint(vector<vector<double>> m1, vector<vector<double>> m2) {
    int i, j, k;
    Point res;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 1; j++) {
            res.p[i][j] = 0;
            for (k = 0; k < 4; k++)
                res.p[i][j] += m1[i][k] * m2[k][j];
        }
    }

    double w = res.p[3][0];
    if(w!=1)
    {
        for(int i1=0;i1<4;i1++)
            res.p[i1][0] /= w;
    }
    return res;
}

vector<vector<double>> generateTranslationMatrix(double tx,double ty,double tz) {
    vector<vector<double>> TM = {
            {1,0,0,tx},
            {0,1,0,ty},
            {0,0,1,tz},
            {0,0,0,1}
    };
    return TM;
}

vector<vector<double>> generateScaleMatrix(double sx,double sy,double sz) {
    vector<vector<double>> SM = {
            {sx,0,0,0},
            {0,sy,0,0},
            {0,0,sz,0},
            {0,0,0,1}
    };
    return SM;
}


vector<vector<double>> generateRotationMatrix(double angle, double ax,double ay,double az) {
    //cout<<"In rotMat\n";
    double val = sqrt(ax*ax+ay*ay+az*az);
    ax /= val; ay /= val; az /= val;
    double sinVal = sin(angle*(M_PI/180));
    double cosVal = cos(angle*(M_PI/180));
    double cosVal2 = 1.0 - cosVal;
    //cout<<"val:"<<val<<"-sinval:"<<sinVal<<"-cosval:"<<cosVal<<"\n";

    vector<vector<double>> p1 = {
            {cosVal,0,0,0},
            {0,cosVal,0,0},
            {0,0,cosVal,0},
            {0,0,0,1}
    };

    vector<vector<double>> p2 = {
            {ax*ax*(cosVal2),ax*ay*(cosVal2),ax*az*(cosVal2),0},
            {ax*ay*(cosVal2),ay*ay*(cosVal2),ay*az*(cosVal2),0},
            {ax*az*(cosVal2),ay*az*(cosVal2),az*az*(cosVal2),0},
            {0,0,0,1}
    };

    vector<vector<double>> p3 = {
            {0,-az*sinVal,ay*sinVal,0},
            {az*sinVal,0,-ax*sinVal,0},
            {-ay*sinVal,ax*sinVal,0,0},
            {0,0,0,1}
    };

    vector<vector<double>> res = {
            {cosVal+ax*ax*(cosVal2),ax*ay*(cosVal2)-az*sinVal,ax*az*(cosVal2)+ay*sinVal,0},
            {ax*ay*(cosVal2)+az*sinVal,cosVal+ay*ay*(cosVal2),ay*az*(cosVal2)-ax*sinVal,0},
            {ax*az*(cosVal2)-ay*sinVal,ay*az*(cosVal2)+ax*sinVal,cosVal+az*az*(cosVal2),0},
            {0,0,0,1}
    };

    /*for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            res[i][j] = p1[i][j] + p2[i][j] + p3[i][j];
        }
    }*/
    return res;
}

void printMatrix(vector<vector<double>> p){
    for (int i = 0; i < p.size(); i++)
    {
        for (int j = 0; j < p[i].size(); j++)
        {
            cout << p[i][j] << " ";
        }
        cout << endl;
    }
}

void writeMatrix(vector<vector<double>> p, ofstream& t){
    for (int i = 0; i < p.size(); i++)
    {
        for (int j = 0; j < p[i].size(); j++)
        {
            if(p[i][j] < rear_end)
                t << p[i][j] << "    ";
        }
        t << endl;
    }
}

vector<vector<double>> product(vector<vector<double>> m1, vector<vector<double>> m2) {
    //cout<<"In Product\n";
    vector<vector<double>> res = {
            {0,0,0,0},
            {0,0,0,0},
            {0,0,0,0},
            {0,0,0,0}
    };

    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            for(int k = 0; k < 4; ++k)
            {
                res[i][j] += m1[i][k] * m2[k][j];
            }
    //printMatrix(res);
    return res;
}

//modelling transformation
void stage1()
{
    ofstream stage1;
    stage1 << setprecision(7) <<fixed;
    stage1.open("stage1.txt");
    fileChoice = 0 ;
    cout<<"Enter scene file no. : \n";
    while (fileChoice<=0 || fileChoice>4)
        cin>>fileChoice;

    stringstream ss;
    ss<<fileChoice;
    string line;
    ifstream sceneFile;
    sceneFile.open(ss.str()+"/scene.txt");
    int count = 1;

    //parse the input scene file
    while(getline(sceneFile, line)) {
        if(count <= 4)
        {
           init_globals(line, count);
           count++;
        }
        else
        {
            string coords;
            if(line == "triangle")
            {
                cout<<"Triangle\n";
                for(int i=0; i<3;i++)
                {
                    double x,y,z;
                    getline(sceneFile, coords);
                    stringstream ls(coords);
                    ls>>x; ls>>y ; ls>>z;
                    Point p1(x,y,z);
                    //cout<<"Single Point"<<endl;
                    //p1.printPoint();
                    Point out1 = transformPoint(STACK.top(),p1.getPoint());
                    //cout<<"Output Point"<<endl;
                    points.push_back(out1);
                    out1.printPoint();
                    out1.writePoint(stage1);
                    if(i==2)
                        stage1 << endl;
                }
            }
            else if(line == "translate")
            {
                cout<<"Translate\n";
                if(!countStack.empty())
                    countStack.top()++;
                double x,y,z;
                getline(sceneFile, coords);
                stringstream ls(coords);
                ls>>x; ls>>y ; ls>>z;

                vector<vector<double>> T = generateTranslationMatrix(x,y,z);
                if(countStack.empty())
                {
                    vector<vector<double>> st = STACK.top();
                    STACK.pop();
                    STACK.push(product(st,T));
                }
                else
                {
                    STACK.push(product(STACK.top(),T));
                }
            }

            else if(line == "scale")
            {
                cout<<"Scale\n";
                if(!countStack.empty())
                    countStack.top()++;
                double x,y,z;
                getline(sceneFile, coords);
                stringstream ls(coords);
                ls>>x; ls>>y ; ls>>z;
                //cout<<x<<","<<y<<","<<z<<"\n";

                vector<vector<double>> S = generateScaleMatrix(x,y,z);
                //printMatrix(S);
                if(countStack.empty())
                {
                    vector<vector<double>> st = STACK.top();
                    STACK.pop();
                    STACK.push(product(st,S));
                }
                else
                {
                    STACK.push(product(STACK.top(),S));
                }
            }

            else if(line == "rotate")
            {
                cout<<"Rotate\n";
                if(!countStack.empty())
                    countStack.top()++;
                double angle,ax,ay,az;
                getline(sceneFile, coords);
                stringstream ls(coords);
                ls>>angle; ls>>ax; ls>>ay ; ls>>az;

                vector<vector<double>> R = generateRotationMatrix(angle,ax,ay,az);
                if(countStack.empty())
                {
                    vector<vector<double>> st = STACK.top();
                    STACK.pop();
                    STACK.push(product(st,R));
                }
                else
                {
                    STACK.push(product(STACK.top(),R));
                }
            }

            else if(line == "push")
            {
                cout<<"Push\n";
                countStack.push(0);
                pushCount++;
            }

            else if(line == "pop")
            {
                cout<<"Pop\n";
                int d = countStack.top();
                //cout<<"countstack--"<<d<<" , StackSize"<<STACK.size()<<"\n";
                countStack.pop();
                for(int i=0;i<d;i++)
                    STACK.pop();
                pushCount--;
            }
            else
            {
                cout<<"Break\n";
                stage1.close();
                break;
            }
        }

    }
}

//view transformation
void stage2()
{
    ofstream stage2;
    stage2 << setprecision(7) <<fixed;
    stage2.open("stage2.txt");

    Vec look(lookX,lookY,lookZ);
    Vec eye(eyeX,eyeY,eyeZ);
    Vec up(upX,upY,upZ);

    Vec l,r,u;
    l = look.subtract(eye);
    l.normalize();

    r = l.cross(up);
    r.normalize();

    u = r.cross(l);

    vector<vector<double>> T = {
            {1,0,0,-eye.x},
            {0,1,0,-eye.y},
            {0,0,1,-eye.z},
            {0,0,0,1}
    };

    vector<vector<double>> R = {
            {r.x,r.y,r.z,0},
            {u.x,u.y,u.z,0},
            {-l.x,-l.y,-l.z,0},
            {0,0,0,1}
    };

    vector<vector<double>> V = product(R,T);

    int lc = 0;
    for (auto i = points.begin(); i != points.end(); ++i)
    {
        *i = transformPoint(V, (*i).getPoint());
        (*i).printPoint();
        (*i).writePoint(stage2);
        lc++;
        if(lc == 3)
        {
            stage2 << endl;
            lc = 0;
        }
    }

    stage2.close();
}


//projection transformation
void stage3()
{
    ofstream stage3;
    stage3 << setprecision(7) <<fixed;
    stage3.open("stage3.txt");

    double fovX,t,r;
    fovX = fovY*aspect_ratio;
    t = near*tan(fovY/2*(M_PI/180));
    r = near*tan(fovX/2*(M_PI/180));

    vector<vector<double>> P = {
            {near/r,0,0,0},
            {0,near/t,0,0},
            {0,0,-(far+near)/(far-near),-(2*far*near)/(far-near)},
            {0,0,-1,0}
    };

    int lc = 0;
    for (auto i = points.begin(); i != points.end(); ++i)
    {
        *i = transformPoint(P, (*i).getPoint());
        (*i).printPoint();
        (*i).writePoint(stage3);
        lc++;
        if(lc == 3)
        {
            stage3 << endl;
            lc = 0;
        }
    }
    stage3.close();
}

//final stage
void readData()
{
    stringstream ss;
    ss<<fileChoice;
    string line;
    ifstream sceneFile;
    sceneFile.open(ss.str()+"/config.txt");
    int lc = 5;

    while (getline(sceneFile,line))
    {
        init_globals(line,lc++);
    }

    int pointCount = 0;
    for (auto i = points.begin(); i != points.end(); i+=3)
    {
        Triangle t;
        t.setTriangle((*i),(*(i+1)),(*(i+2)));
        t.printTriangle();
        triangles.push_back(t);
    }
}

void initBuffer()
{
    zBuffer.resize(screenHeight);
    for (int i = 0; i < screenHeight; ++i)
        zBuffer[i].resize(screenWidth,rear_end);

    /*ofstream t;
    t.open("dummy.txt");
    writeMatrix(zBuffer,t);
    t.close();*/

    dx = (right_limit - left_limit)/screenWidth;
    dy = (top_limit - bottom_limit)/screenHeight;
    TopY = top_limit - (dy/2);
    LeftX = left_limit + (dx/2);
    BottomY = bottom_limit + (dy/2);
    RightX = right_limit - (dx/2);

    image.setwidth_height(screenWidth,screenHeight);
    for(int i=0;i<screenWidth;i++){
        for(int j=0;j<screenHeight;j++){
            image.set_pixel(i,j,0,0,0);
        }
    }
    //image.save_image("out.bmp");
}

int find_row(double x,bool v)
{
    if(v)
        return floor((TopY-x)/dy);
    else
        return ceil((TopY-x)/dy);
}

int find_col(double x,bool v)
{
    if(v)
        return floor(abs((LeftX-x)/dx));
    else
        return ceil(abs((LeftX-x)/dx));
}

void apply_procedure()
{
    for (auto i = triangles.begin(); i != triangles.end(); i++)
    {
        //cout<<"Triangle Object yaaaaay\n";
        int topScanline,bottomScanline, leftScanline,rightScanline;
        Triangle t = (*i);
        if(t.maxY()>TopY)
        {
            topScanline = 0;
        } else
        {
            topScanline = find_row(t.maxY(), true);
        }

        if(t.minY()<BottomY)
        {
            bottomScanline = screenHeight-1;
        } else
        {
            bottomScanline = find_row(t.minY(), false);
        }

        vector<double> xvals;
        vector<Point> edges;
        //cout<<"TopS:::"<<topScanline<<"::BottomS::"<<bottomScanline<<"\n";
        for(int r =topScanline; r<=bottomScanline; r++)
        {
            //cout<<"Outer Loopppppppppppppppp\n";
            double ys = TopY - r*dy;
            double x1,x2,x3, xmin,xmax;

            x1 = ((ys-t.points[0].p[1][0])*(t.points[0].p[0][0] - t.points[1].p[0][0])/
                    (t.points[0].p[1][0]-t.points[1].p[1][0])) + t.points[0].p[0][0];
            //cout<<"hey";
            if(x1 >= t.minX() && x1 <= t.maxX())
            {
                //cout<<"x1 hit\n";
                xvals.push_back(x1);
                edges.push_back(points[0]);
                edges.push_back(points[1]);
            }

            x2 = ((ys-t.points[0].p[1][0])*(t.points[0].p[0][0] - t.points[2].p[0][0])/
                  (t.points[0].p[1][0]-t.points[2].p[1][0])) + t.points[0].p[0][0];
            //cout<<"hey2";

            if(x2 >= t.minX() && x2 <= t.maxX())
            {
                //cout<<"x2 hit::"<<x2<<"::"<<ys<<endl;
                xvals.push_back(x2);
                edges.push_back(points[0]);
                edges.push_back(points[2]);
            }

            x3 = ((ys-t.points[1].p[1][0])*(t.points[1].p[0][0] - t.points[2].p[0][0])/
                  (t.points[1].p[1][0]-t.points[2].p[1][0])) + t.points[1].p[0][0];
            //cout<<"hey3";

            if(x3 >= t.minX() && x3 <= t.maxX())
            {
                //cout<<"x3 hit\n";
                xvals.push_back(x3);
                edges.push_back(points[1]);
                edges.push_back(points[2]);
            }

            //cout<<"hey3.5::"<<xvals.size()<<"\n";
            if(xvals.empty()) continue;
            xmin = *(min_element(xvals.begin(),xvals.end()));
            xmax = *(max_element(xvals.begin(),xvals.end()));
            //cout<<"hey4";

            if(xmin < LeftX)
            {
                leftScanline = 0;
            } else{
                leftScanline = find_col(xmin, false);
            }

            if(xmax > RightX)
            {
                rightScanline = screenWidth - 1;
            } else{
                rightScanline = find_col(xmax, true);
            }

            //cout<<"Z calc begin--::"<<edges.size()<<":::"<<xvals.size()<<"\n";
            if(edges.size()<4)
                continue;
            double za,zb,zp;
            za = edges[0].p[2][0] + ((ys - edges[0].p[1][0])* (edges[1].p[2][0]-edges[0].p[2][0])
                                     / (edges[1].p[1][0]-edges[0].p[1][0]));
            zb = edges[0].p[2][0] + ((ys - edges[0].p[1][0])* (edges[3].p[2][0]-edges[0].p[2][0])
                                     / (edges[3].p[1][0]-edges[0].p[1][0]));
            zp = za;
            //cout<<"Z calc end\n";

            for(int c = leftScanline; c<=rightScanline; c++)
            {
                //cout<<"Inner looooooooooppppppppp\n";
                zp += dx*((zb-za)/(xmax-xmin));
                if(zBuffer[r][c]>zp && zp>=front_end)
                {
                    zBuffer[r][c] = zp;
                    image.set_pixel(c,r,t.color[0],t.color[1],t.color[2]);
                }
            }
        }
    }

    ofstream fs;
    fs << setprecision(7) <<fixed;
    fs.open("z_buffer.txt");
    writeMatrix(zBuffer,fs);
    fs.close();
    image.save_image("out.bmp");
}

void finalstage()
{
    readData();
    initBuffer();
    apply_procedure();
}


int main() {
    cout << setprecision(7) <<fixed;
    initStack();
    stage1();
    stage2();
    stage3();
    finalstage();
    return 0;
}
