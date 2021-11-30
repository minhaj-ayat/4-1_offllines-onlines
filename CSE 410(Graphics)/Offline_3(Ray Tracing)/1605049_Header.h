#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<vector>
#include <fstream>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
#include <stack>
#include <cmath>
#include <iomanip>
#include<algorithm>
#include <memory>
#include "bitmap_image.hpp"

#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

#define konst 8
#define rdeg 4

extern void drawSphere(double radius,int slices,int stacks);
extern int rec_level;

using namespace std;
class Vector3D {
public:
    double x,y,z;

    Vector3D()
    {}

    Vector3D(double a,double b,double c)
    {
        x = a; y = b;  z = c;
    }

    void setVector(double a,double b,double c)
    {
        x = a; y = b;  z = c;
    }

    Vector3D addVector(Vector3D v)
    {
        return {x+v.x, y+v.y, z+v.z};
    }

    Vector3D subVector(Vector3D v)
    {
        return {x-v.x, y-v.y, z-v.z};
    }

    double dotVector(Vector3D v)
    {
        Vector3D v1(x,y,z);
        Vector3D v2(v.x,v.y,v.z);
        return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
    }

    Vector3D crossVector(Vector3D v)
    {
        Vector3D v1(x,y,z);
        Vector3D v2(v.x,v.y,v.z);
        return {v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x};
    }

    double val()
    {
        return sqrt(x*x + y*y + z*z);
    }

    void normalize()
    {
        x = x/val(); y = y/val(); z = z/val();
    }

    Vector3D scalermultVector(double s)
    {
        return {s*x, s*y, s*z};
    }
};

class Ray {
public:
    Vector3D start;
    Vector3D dir;

    Ray() = default;

    Ray(Vector3D st, Vector3D d){
        start.setVector(st.x,st.y,st.z);
        double val = sqrt(d.x*d.x+d.y*d.y+d.z*d.z);
        dir.setVector((d.x)/val,(d.y)/val,(d.z)/val);
    }

    Ray(double x,double y,double z,double x2,double y2,double z2){
        start.setVector(x,y,z);
        double val = sqrt(x2*x2 + y2*y2 + z2*z2);
        dir.setVector((x2)/val,(y2)/val,(z2)/val);
    }

    void setRay(Vector3D s, Vector3D d)
    {
        start = s;
        dir = d;
    }
};

class Light {
public:
    Vector3D light_pos;
    double color[3];

    Light(){
    }

    Light(Vector3D lp, double r, double g, double b)
    {
        light_pos = lp;
        color[0] = r; color[1] = g; color[2] = b;
    }

    void print()
    {
        cout<<"\n\nLight:-\n";
        cout<<"Light pos:"<<light_pos.x<<","<<light_pos.y<<","<<light_pos.z<<endl;
        cout<<"Color:"<<color[0]<<","<<color[1]<<","<<color[2]<<endl;
    }
};


class Object{
public:
    Vector3D reference_point;
    double height,width,length;

    double color[3];
    double newcolor[3];
    double coefficients[4];
    int shine;

    Object() {
    }

    virtual void draw() {
        //cout<<"Base draw\n";
    }

    virtual double intersect(Ray ray,double *col,int level){
        return -1.0;
    }

    virtual void print() {
        cout<<"Base\n";
    }

    void setColor(double r, double g, double b) {
        color[0] = r; color[1] = g; color[2] = b;
        newcolor[0] = r; newcolor[1] = g; newcolor[2] = b;
    }

    void setnewColor(double r, double g, double b) {
        newcolor[0] = r; newcolor[1] = g; newcolor[2] = b;
    }

    void setCoefficients (double amb, double diff, double spec, double rec) {
        coefficients[0] = amb; coefficients[1] = diff;
        coefficients[2] = spec; coefficients[3] = rec;
    }

    void setShine(int s) {
        shine = s;
    }
};

extern vector<Object *> objects;
extern vector<Light> lights;


bool inShadow(Ray ray, double mt)
{
    bool Hit = false;
    double t,tMin = 10000;

    for (int p = 0; p < objects.size(); p++)
    {
        Object *o = objects.at(p);
        double *dummy;
        t = (*o).intersect(ray,dummy,0);
        if(t>0 && t < tMin)
        {
            tMin = t;
            Hit = true;
        }
    }

    if(tMin == mt)
    {
        //cout<<"Not in shadow"<<endl;
        return false;
    }
    else
    {
        //cout<<"In shadow"<<endl;
        return true;
    }
}

Object* nearest(Ray ray)
{
    bool Hit = false;
    double t,tMin = 10000;
    int nr;

    for (int p = 0; p < objects.size(); p++)
    {
        Object *o = objects.at(p);
        double *dummy;
        t = (*o).intersect(ray,dummy,0);
        if(t>0 && t < tMin)
        {
            tMin = t;
            Hit = true;
            nr = p;
        }
        else if(t!=-1 && t>0)
        {
            Hit = true;
        }
    }

    if(Hit)
    {
        //cout<<"Not in shadow"<<endl;
        return objects.at(nr);
    }
    else
    {
        return nullptr;
    }
}

class Sphere : public Object {
public:
    Sphere(Vector3D center, double radius)
    {
        reference_point = center;
        length = radius;
    }

    void draw() override
    {
        glPushMatrix();
        {
            glColor3f(color[0],color[1],color[2]);
            glTranslatef(reference_point.x, reference_point.y,reference_point.z);
            drawSphere(length,100,20);
        }
        glPopMatrix();
    }

    double intersect(Ray ray,double *col,int level) override
    {
        double tmin;
        double a = 1;
        Vector3D v(ray.start.x - reference_point.x,ray.start.y - reference_point.y,ray.start.z - reference_point.z);
        double b = 2*(ray.dir.x*v.x + ray.dir.y*v.y + ray.dir.z*v.z);
        double c = (v.x*v.x + v.y*v.y + v.z*v.z) - (length*length);
        double d = sqrt(b*b - 4*a*c);
        if(d>=0)
        {
            double t1 = (-b + d)/(2*a);
            double t2 = (-b - d)/(2*a);
            //cout<<"t1:"<<t1<<", t2:"<<t2<<endl;
            if(t1>=0 || t2>=0)
                tmin =  (t1<t2) ? t1 : t2;
            else
                tmin = -1;
        } else
            tmin = -1;

        if(level == 0)
        {
            return tmin;
        }
        else if(level>0 && tmin!=-1)
        {
            Vector3D ip(ray.start.x+tmin*ray.dir.x, ray.start.y+tmin*ray.dir.y, ray.start.z+tmin*ray.dir.z);

            col[0] = color[0]*coefficients[0];
            col[1] = color[1]*coefficients[0];
            col[2] = color[2]*coefficients[0];

            //cout<<"New Colors : "<<color[0]<<" "<<color[1]<<" "<<color[2]<<endl;
            //cout<<"Finding\n";
            //print();

            Vector3D normal = ip.subVector(reference_point);

            Ray reflected, refracted;
            double crefrac;
            for(int p=0; p<lights.size(); p++) {
                Light l = lights.at(p);

                Vector3D ld = ip.subVector(l.light_pos);
                Ray ray1(l.light_pos, ld);

                double lambert,phong;
                normal.normalize();
                ld.normalize();
                lambert = normal.dotVector(ld);
                Vector3D dref = normal.scalermultVector(2 * lambert).subVector(ld);
                Vector3D ref_start(ip.x + 0.001 * dref.x, ip.y + 0.001 * dref.y, ip.z + 0.001 * dref.z);
                dref.normalize();
                reflected.setRay(ref_start, dref);

                crefrac = 1.2;

                double rf1 = lambert*crefrac;
                double rf2 = sqrt(rf1-crefrac*crefrac*(1-lambert*lambert));
                Vector3D drefr = normal.scalermultVector(rf2).subVector(ld.scalermultVector(crefrac));
                Vector3D cref_start(ip.x + 0.001 * drefr.x, ip.y + 0.001 * drefr.y, ip.z + 0.001 * drefr.z);
                refracted.setRay(cref_start,drefr);
                //ray.dir.normalize();
                phong = dref.dotVector(ray.dir);

                double *dummy;
                double myt = intersect(ray1, dummy, 0);
                if (!inShadow(ray1, myt))
                {
                    if(lambert<0) lambert = 0;
                    if(phong<0) phong = 0;
                    col[0] += l.color[0] * coefficients[1] * lambert * color[0];
                    col[1] += l.color[1] * coefficients[1] * lambert * color[1];
                    col[2] += l.color[2] * coefficients[1] * lambert * color[2];
                    //cout<<"Lambert: "<<lambert<<endl;
                    //cout<<"New Colors : "<<newcolor[0]<<" "<<newcolor[1]<<" "<<newcolor[2]<<endl;

                    col[0] += l.color[0] * coefficients[2] * pow(phong, shine) * color[0];
                    col[1] += l.color[1] * coefficients[2] * pow(phong, shine) * color[1];
                    col[2] += l.color[2] * coefficients[2] * pow(phong, shine) * color[2];
                    //cout<<"Phong: "<<phong<<endl;
                }
            }

            Object *o1 = nearest(reflected);
            Object *o2 = nearest(refracted);
            if(level>=rec_level ||o1 == nullptr || o2== nullptr)
            {
                return tmin;
            }

            double *colref = new double[3];
            double *colrefr = new double[3];

            if(o1 != nullptr)
            {
                (*o1).intersect(reflected,colref,level+1);
            }
            if(o2 != nullptr)
            {
                (*o2).intersect(refracted,colrefr,level+1);
            }

            col[0] += colref[0]*coefficients[3];
            col[1] += colref[1]*coefficients[3];
            col[2] += colref[2]*coefficients[3];

            col[0] += colrefr[0]*(coefficients[3]/2);
            col[1] += colrefr[1]*(coefficients[3]/2);
            col[2] += colrefr[2]*(coefficients[3]/2);

            //delete[] colref;
            //te[] colrefr;
        }

        else
            return tmin;
    }

    void print() override
    {
        cout<<"\n\nSphere:-\n";
        cout<<"Center:"<<reference_point.x<<","<<reference_point.y<<","<<reference_point.z<<endl;
        cout<<"Radius:"<<length<<endl;
        cout<<"Color:"<<color[0]<<","<<color[1]<<","<<color[2]<<endl;
        cout<<"CoEfficients:"<<coefficients[0]<<","<<coefficients[1]<<","<<coefficients[2]<<","<<coefficients[3]<<endl;
        cout<<"Shine:"<<shine<<endl;

    }
};

double calcDet(double a,double b,double c,double p, double q, double r, double x, double y, double z)
{
    double res = a*(q*z-r*y) + b*(r*x-p*z) + c*(p*y-q*x);
    return res;
}

class Triangle : public Object {
public:
    Vector3D b,c;
    Triangle(Vector3D p1, Vector3D p2, Vector3D p3)
    {
        reference_point = p1;
        b = p2;
        c = p3;
    }

    void draw() override
    {
        glPushMatrix();
        {
            glColor3f(color[0],color[1],color[2]);
            glBegin(GL_TRIANGLES);
            {
                glTranslatef(0,0,0);
                glVertex3f(reference_point.x,reference_point.y,reference_point.z);
                glVertex3f(b.x,b.y,b.z);
                glVertex3f(c.x,c.y,c.z);
            }glEnd();
        }
        glPopMatrix();
    }

    double intersect(Ray ray,double *col,int level) override
    {
        double tmin;

        double det = calcDet((reference_point.x-b.x),(reference_point.x-c.x),ray.dir.x,(reference_point.y-b.y),(reference_point.y-c.y),ray.dir.y,
                             (reference_point.z-b.z),(reference_point.z-c.z),ray.dir.z);

        double beta = calcDet((reference_point.x-ray.start.x),(reference_point.x-c.x),ray.dir.x,(reference_point.y-ray.start.y),(reference_point.y-c.y),ray.dir.y,
                              (reference_point.z-ray.start.z),(reference_point.z-c.z),ray.dir.z)/det;

        double gamma = calcDet((reference_point.x-b.x),(reference_point.x-ray.start.x),ray.dir.x,(reference_point.y-b.y),(reference_point.y-ray.start.y),ray.dir.y,
                               (reference_point.z-b.z),(reference_point.z-ray.start.z),ray.dir.z)/det;

        double t = calcDet((reference_point.x-b.x),(reference_point.x-c.x),(reference_point.x-ray.start.x),(reference_point.y-b.y),(reference_point.y-c.y),(reference_point.y-ray.start.y),
                           (reference_point.z-b.z),(reference_point.z-c.z),(reference_point.z-ray.start.z))/det;

        if(beta>0 && gamma>0 && (beta+gamma)<1 && t>0)
        {
            tmin = t;
        }
        else
            tmin = -1;

        if(level == 0)
        {
            return tmin;
        }

        else if(level>0 && tmin!=-1)
        {
            Vector3D ip(ray.start.x+tmin*ray.dir.x, ray.start.y+tmin*ray.dir.y, ray.start.z+tmin*ray.dir.z);
            col[0] = color[0]*coefficients[0];
            col[1] = color[1]*coefficients[0];
            col[2] = color[2]*coefficients[0];

            //cout<<"New Colors : "<<color[0]<<" "<<color[1]<<" "<<color[2]<<endl;
            //cout<<"Finding\n";
            //print();
            Vector3D normal = b.subVector(reference_point).crossVector(c.subVector(reference_point));

            Ray reflected,refracted;
            double crefrac;
            for(int p=0; p<lights.size(); p++) {
                Light l = lights.at(p);

                Vector3D ld = ip.subVector(l.light_pos);
                Ray ray1(l.light_pos, ld);

                double lambert, phong;
                normal.normalize();
                ld.normalize();
                lambert = normal.dotVector(ld);
                Vector3D dref = normal.scalermultVector(2 * lambert).subVector(ld);
                Vector3D ref_start(ip.x + 0.001 * dref.x, ip.y + 0.001 * dref.y, ip.z + 0.001 * dref.z);
                dref.normalize();
                reflected.setRay(ref_start, dref);

                crefrac = 1.2;

                double rf1 = lambert*crefrac;
                double rf2 = sqrt(rf1-crefrac*crefrac*(1-lambert*lambert));
                Vector3D drefr = normal.scalermultVector(rf2).subVector(ld.scalermultVector(crefrac));
                Vector3D cref_start(ip.x + 0.001 * drefr.x, ip.y + 0.001 * drefr.y, ip.z + 0.001 * drefr.z);
                refracted.setRay(cref_start,drefr);
                //ray.dir.normalize();
                phong = dref.dotVector(ray.dir);

                double *dummy;
                double myt = intersect(ray1, dummy , 0);
                if (!inShadow(ray1, myt))
                {
                    if(lambert<0) lambert = 0;
                    if(phong<0) phong = 0;
                    col[0] += l.color[0] * coefficients[1] * lambert * color[0];
                    col[1] += l.color[1] * coefficients[1] * lambert * color[1];
                    col[2] += l.color[2] * coefficients[1] * lambert * color[2];
                    //cout<<"Lambert: "<<lambert<<endl;
                    //cout<<"New Colors : "<<newcolor[0]<<" "<<newcolor[1]<<" "<<newcolor[2]<<endl;

                    col[0] += l.color[0] * coefficients[2] * pow(phong, shine) * color[0];
                    col[1] += l.color[1] * coefficients[2] * pow(phong, shine) * color[1];
                    col[2] += l.color[2] * coefficients[2] * pow(phong, shine) * color[2];
                    //cout<<"Phong: "<<phong<<endl;
                }
            }

            Object *o1 = nearest(reflected);
            Object *o2 = nearest(refracted);
            if(level>=rec_level ||o1 == nullptr || o2== nullptr)
            {
                return tmin;
            }

            double *colref = new double[3];
            double *colrefr = new double[3];


            if(o1 != nullptr)
            {
                (*o1).intersect(reflected,colref,level+1);
            }
            if(o2 != nullptr)
            {
                (*o2).intersect(refracted,colrefr,level+1);
            }
            col[0] += colref[0]*coefficients[3];
            col[1] += colref[1]*coefficients[3];
            col[2] += colref[2]*coefficients[3];

            col[0] += colrefr[0]*(coefficients[3]/2);
            col[1] += colrefr[1]*(coefficients[3]/2);
            col[2] += colrefr[2]*(coefficients[3]/2);

            //delete[] colref;
            //te[] colrefr;
        }

        else
            return tmin;

    }

    void print() override
    {
        cout<<"\n\nTriangle:-\n";
        cout<<"Point 1:"<<reference_point.x<<","<<reference_point.y<<","<<reference_point.z<<endl;
        cout<<"Point 2:"<<b.x<<","<<b.y<<","<<b.z<<endl;
        cout<<"Point 3:"<<c.x<<","<<c.y<<","<<c.z<<endl;

        cout<<"Color:"<<color[0]<<","<<color[1]<<","<<color[2]<<endl;
        cout<<"CoEfficients:"<<coefficients[0]<<","<<coefficients[1]<<","<<coefficients[2]<<","<<coefficients[3]<<endl;
        cout<<"Shine:"<<shine<<endl;

    }
};

class General : public Object {
public:
    double A,B,C,D,E,F,G,H,I,J;
    General(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j,
            Vector3D refpoint, double len, double wid, double ht)
    {
        A=a; B=b; C=c; D=d; E=e; F=f; G=g; H=h; I=i; J=j;
        reference_point = refpoint;
        length = len;
        width = wid;
        height = ht;
    }

    double intersect(Ray ray,double *col,int level) override
    {
        double tmin;
        double aq = A*(ray.dir.x*ray.dir.x) + B*(ray.dir.y*ray.dir.y) + C*(ray.dir.z*ray.dir.z)
                    + D*(ray.dir.x*ray.dir.y) + E*(ray.dir.x*ray.dir.z) + F*(ray.dir.y*ray.dir.z);

        double bq = 2*A*(ray.start.x*ray.dir.x) + 2*B*(ray.start.y*ray.dir.y) + 2*C*(ray.start.z*ray.dir.z)
                    + D*(ray.start.x*ray.dir.y + ray.start.y*ray.dir.x) + E*(ray.start.x*ray.dir.z + ray.start.z*ray.dir.x)
                    + F*(ray.start.y*ray.dir.z + ray.start.z*ray.dir.y)
                    + G*ray.dir.x + H*ray.dir.y + I*ray.dir.z;

        double cq = A*(ray.start.x*ray.start.x) + B*(ray.start.y*ray.start.y) + C*(ray.start.z*ray.start.z)
                    + D*(ray.start.x*ray.start.y) + E*(ray.start.x*ray.start.z) + F*(ray.start.y*ray.start.z)
                    + G*ray.start.x + H*ray.start.y + I*ray.start.z + J;

        double dd = sqrt(bq*bq - 4*aq*cq);

        if(dd>=0)
        {
            double t1 = (-bq - dd)/(2*aq);
            double t2 = (-bq + dd)/(2*aq);
            //cout<<"t1:"<<t1<<", t2:"<<t2<<endl;
            if(t1>=0 || t2>=0)
            {
                Vector3D v1(ray.start.x+t1*ray.dir.x, ray.start.y+t1*ray.dir.y, ray.start.z+t1*ray.dir.z);
                Vector3D v2(ray.start.x+t2*ray.dir.x, ray.start.y+t2*ray.dir.y, ray.start.z+t2*ray.dir.z);
                bool t1x= true,t1y= true,t1z= true,t2x= true,t2y= true,t2z= true;
                bool t1in= false,t2in= false;
                if(length>0)
                {
                    if(v1.x>length)
                        t1x = false;
                    if(v2.x>length)
                        t2x = false;

                }
                if(width>0)
                {
                    if(v1.y>width)
                        t1y = false;
                    if(v2.y>width)
                        t2y = false;

                }
                if(height>0)
                {
                    if(v1.z>height)
                        t1z = false;
                    if(v2.z>height)
                        t2z = false;

                }
                if(t1x && t1y && t1z) t1in = true;
                if(t2x && t2y && t2z) t2in = true;

                if(t1in && t2in)
                    tmin =  (t1<t2) ? t1 : t2;
                else if(t1in)
                    tmin =  t1;
                else if(t2in)
                    tmin =  t2;
                else
                    tmin = -1;
            }
            else
                tmin = -1;
        } else
            tmin = -1;

        if(level == 0)
        {
            return tmin;
        }

        else if(level>0 && tmin!=-1)
        {
            Vector3D ip(ray.start.x+tmin*ray.dir.x, ray.start.y+tmin*ray.dir.y, ray.start.z+tmin*ray.dir.z);
            col[0] = color[0]*coefficients[0];
            col[1] = color[1]*coefficients[0];
            col[2] = color[2]*coefficients[0];

            //cout<<"New Colors : "<<color[0]<<" "<<color[1]<<" "<<color[2]<<endl;
            //cout<<"Finding\n";
            //print();
            double x1 = 2*A*ip.x + D*ip.y + E*ip.z + G;
            double y1 = 2*B*ip.y + D*ip.x + F*ip.z + H;
            double z1 = 2*C*ip.z + E*ip.x + F*ip.y + I;
            Vector3D normal(x1,y1,z1);
            normal.setVector(x1/normal.val(), y1/normal.val(), z1/normal.val());

            Ray reflected, refracted;
            double crefrac;
            for(int p=0; p<lights.size(); p++) {
                Light l = lights.at(p);

                Vector3D ld = ip.subVector(l.light_pos);
                Ray ray1(l.light_pos, ld);

                double lambert, phong;
                normal.normalize();
                ld.normalize();
                lambert = normal.dotVector(ld);
                Vector3D dref = normal.scalermultVector(2 * lambert).subVector(ld);
                Vector3D ref_start(ip.x + 0.001 * dref.x, ip.y + 0.001 * dref.y, ip.z + 0.001 * dref.z);
                dref.normalize();
                reflected.setRay(ref_start, dref);

                crefrac = 1.2;
                double rf1 = lambert*crefrac;
                double rf2 = sqrt(rf1-crefrac*crefrac*(1-lambert*lambert));
                Vector3D drefr = normal.scalermultVector(rf2).subVector(ld.scalermultVector(crefrac));
                Vector3D cref_start(ip.x + 0.001 * drefr.x, ip.y + 0.001 * drefr.y, ip.z + 0.001 * drefr.z);
                refracted.setRay(cref_start,drefr);
                //ray.dir.normalize();
                phong = dref.dotVector(ray.dir);

                double *dummy;
                double myt = intersect(ray1, dummy, 0);
                if (!inShadow(ray1, myt))
                {
                    if(lambert<0) lambert = 0;
                    if(phong<0) phong = 0;
                    col[0] += l.color[0] * coefficients[1] * lambert * color[0];
                    col[1] += l.color[1] * coefficients[1] * lambert * color[1];
                    col[2] += l.color[2] * coefficients[1] * lambert * color[2];
                    //cout<<"Lambert: "<<lambert<<endl;
                    //cout<<"New Colors : "<<newcolor[0]<<" "<<newcolor[1]<<" "<<newcolor[2]<<endl;

                    col[0] += l.color[0] * coefficients[2] * pow(phong, shine) * color[0];
                    col[1] += l.color[1] * coefficients[2] * pow(phong, shine) * color[1];
                    col[2] += l.color[2] * coefficients[2] * pow(phong, shine) * color[2];
                    //cout<<"Phong: "<<phong<<endl;
                }
            }

            Object *o1 = nearest(reflected);
            Object *o2 = nearest(refracted);
            if(level>=rec_level ||o1 == nullptr || o2== nullptr)
            {
                return tmin;
            }

            double *colref = new double[3];
            double *colrefr = new double[3];

            if(o1 != nullptr)
            {
                (*o1).intersect(reflected,colref,level+1);
            }
            if(o2 != nullptr)
            {
                (*o2).intersect(refracted,colrefr,level+1);
            }
            col[0] += colref[0]*coefficients[3];
            col[1] += colref[1]*coefficients[3];
            col[2] += colref[2]*coefficients[3];

            col[0] += colrefr[0]*(coefficients[3]/2);
            col[1] += colrefr[1]*(coefficients[3]/2);
            col[2] += colrefr[2]*(coefficients[3]/2);

            //delete[] colref;
            //delete[] colrefr;
        }
        else
            return tmin;
    }

    void print() override
    {
        cout<<"\n\nGeneral:-\n";
        cout<<"A="<<A<<", B="<<B<<", C="<<C<<", D="<<D<<", E="<<E<<", F="<<F<<", G="<<G<<", H="<<H<<", I="<<I<<", J="<<J<<endl;

        cout<<"Cube Reference Point:"<<reference_point.x<<","<<reference_point.y<<","<<reference_point.z<<endl;
        cout<<"Length:"<<length<<"  Width:"<<width<<"  Height:"<<height<<endl;
        cout<<"Color:"<<color[0]<<","<<color[1]<<","<<color[2]<<endl;
        cout<<"CoEfficients:"<<coefficients[0]<<","<<coefficients[1]<<","<<coefficients[2]<<","<<coefficients[3]<<endl;
        cout<<"Shine:"<<shine<<endl;
    }
};

class Floor : public Object {
public:
    Vector3D normal;
    Floor(double floorwidth, double tilewidth)
    {
        Vector3D v(-floorwidth/2, -floorwidth/2,0);
        reference_point = v;
        length = tilewidth;
        width = floorwidth;
        normal.setVector(0,0,1);
    }

    double intersect(Ray ray,double *col,int level) override
    {
        double tmin;
        Vector3D c(reference_point.x-ray.start.x, reference_point.y-ray.start.y, reference_point.z-ray.start.z);
        double nu = -(ray.start.z);
        double den = ray.dir.z;
        if(abs(den)>0)
        {
            double t = nu/den;
            //cout<<"Plane:t = "<<t<<endl;
            Vector3D v(ray.start.x+t*ray.dir.x, ray.start.y+t*ray.dir.y, ray.start.z+t*ray.dir.z);
            if(t>0)
            {
                //cout<<"Plane:t = "<<t<<endl;
                if(v.x >= -width/2 && v.x <= width/2 && v.y >= -width/2 && v.y <= width/2)
                {
                    int tx = (v.x + width/2 - length/2)/20;
                    int ty = (v.y + width/2 - length/2)/20;
                    int tcl = (tx+ty)%2;
                    setColor(tcl,tcl,tcl);
                    tmin =  t;
                }
                else
                    tmin = -1;
            }
            else
                tmin = -1;
        }
        else tmin = -1;

        if(level == 0)
        {
            return tmin;
        }

        else if(level>0 && tmin!=-1)
        {
            Vector3D ip(ray.start.x+tmin*ray.dir.x, ray.start.y+tmin*ray.dir.y, ray.start.z+tmin*ray.dir.z);
            col[0] = color[0]*coefficients[0];
            col[1] = color[1]*coefficients[0];
            col[2] = color[2]*coefficients[0];

            //cout<<"New Colors : "<<color[0]<<" "<<color[1]<<" "<<color[2]<<endl;
            //cout<<"Finding\n";
            //print();

            Ray reflected,refracted;
            double crefrac;
            for(int p=0; p<lights.size(); p++) {
                Light l = lights.at(p);

                Vector3D ld = ip.subVector(l.light_pos);
                Ray ray1(l.light_pos, ld);

                double lambert, phong;
                normal.normalize();
                ld.normalize();
                lambert = normal.dotVector(ld);
                Vector3D dref = normal.scalermultVector(2 * lambert).subVector(ld);
                Vector3D ref_start(ip.x + 0.001 * dref.x, ip.y + 0.001 * dref.y, ip.z + 0.001 * dref.z);
                dref.normalize();
                reflected.setRay(ref_start, dref);

                crefrac = 1.2;
                double rf1 = lambert*crefrac;
                double rf2 = sqrt(rf1-crefrac*crefrac*(1-lambert*lambert));
                Vector3D drefr = normal.scalermultVector(rf2).subVector(ld.scalermultVector(crefrac));
                Vector3D cref_start(ip.x + 0.001 * drefr.x, ip.y + 0.001 * drefr.y, ip.z + 0.001 * drefr.z);
                refracted.setRay(cref_start,drefr);
                //ray.dir.normalize();
                phong = dref.dotVector(ray.dir);

                double *dummy;
                double myt = intersect(ray1, dummy, 0);
                if (!inShadow(ray1, myt))
                {
                    if(lambert<0) lambert = 0;
                    if(phong<0) phong = 0;

                    col[0] += l.color[0] * coefficients[1] * lambert * color[0];
                    col[1] += l.color[1] * coefficients[1] * lambert * color[1];
                    col[2] += l.color[2] * coefficients[1] * lambert * color[2];
                    //cout<<"Lambert: "<<lambert<<endl;
                    //cout<<"New Colors : "<<newcolor[0]<<" "<<newcolor[1]<<" "<<newcolor[2]<<endl;

                    col[0] += l.color[0] * coefficients[2] * pow(phong, shine) * color[0];
                    col[1] += l.color[1] * coefficients[2] * pow(phong, shine) * color[1];
                    col[2] += l.color[2] * coefficients[2] * pow(phong, shine) * color[2];

                    //cout<<"Phong: "<<phong<<endl;
                }
            }

            Object *o1 = nearest(reflected);
            Object *o2 = nearest(refracted);
            if(level>=rec_level ||o1 == nullptr || o2== nullptr)
            {
                return tmin;
            }

            double *colref = new double[3];
            double *colrefr = new double[3];

            if(o1 != nullptr)
            {
                (*o1).intersect(reflected,colref,level+1);
            }
            if(o2 != nullptr)
            {
                (*o2).intersect(refracted,colrefr,level+1);
            }
            col[0] += colref[0]*coefficients[3];
            col[1] += colref[1]*coefficients[3];
            col[2] += colref[2]*coefficients[3];

            col[0] += colrefr[0]*(coefficients[3]/2);
            col[1] += colrefr[1]*(coefficients[3]/2);
            col[2] += colrefr[2]*(coefficients[3]/2);

            //delete[] colref;
            //delete[] colrefr;
        }
        else
            return tmin;

    }

    void draw() override
    {
        int wl = (width/2-length/2);
        //cout<<wl;
        int colChange = 0;
        glPushMatrix();
        glTranslatef(-wl,-wl,0);
        for(int i = -wl; i<= wl; i+= length)
            for(int j=-wl; j<=wl; j+=length)
            {
                glColor3f(1-colChange,1-colChange,1-colChange);
                glBegin(GL_QUADS);{
                    glVertex3f(length/2,length/2,0);
                    glVertex3f(length/2,-length/2,0);
                    glVertex3f(-length/2,-length/2,0);
                    glVertex3f(-length/2,length/2,0);
                }glEnd();
                colChange = 1-colChange;
                if(j<wl)
                    glTranslatef(length,0,0);
                else
                {
                    glTranslatef(-(width-length),length,0);
                    colChange = 1-colChange;
                }
            }
        glPopMatrix();
    }
};




