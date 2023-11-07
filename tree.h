#ifndef TREE_H
#define TREE_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <vector>
#include<math.h>
#define PI 3.1415926535897932384626433832795

float Sin(float);
float Asin(float);
float Cos(float);
float Acos(float);
float Tan(float);
float Atan(float);

// gsl_rng * r = gsl_rng_alloc (gsl_rng_taus);

float Sin(float x){ return sin(x * PI / 180); }
float Asin(float x){ return asin(x) / PI * 180; }
float Cos(float x){ return cos(x * PI / 180); }
float Acos(float x){ return acos(x) / PI * 180; }
float Tan(float x){ return tan(x * PI / 180); }
float Atan(float x){ return atan(x) / PI * 180; }

typedef struct{
    int width;
    int height;
    char *title;

    float field_of_view_angle;
    float z_near;
    float z_far;
} glutWindow;

struct color{
    float r, g, b;
    color(float x, float y, float z){
        r = x;
        g = y;
        b = z;
    }
    color(){}
};
class branch{
public:
    branch();
    branch(branch *);
    ~branch();
    void grow(void);
    void grow(int);
    void bud(float, float, float);
    void draw(float, float, float);
    void rotate(double, double);
    void drawSelf(float, float, float);
    void getBase(void);
    GLUquadricObj *myCylinder;
    branch *parent;
    std::vector<std::pair<branch, color> > children;
    double radius;
    double length;
    double X, Y, Z;
    double B_theta, B_phi, r_phi;
private:
};

// std::vector<branch*> branches;

branch::branch(){
    parent = 0;
    // id = 0;
    radius = 0.015;
    length = 0.1;
    X = 0;
    Y = 0;
    Z = 0;
    B_theta = 0;
    B_phi = 0;
    r_phi = 0;
    myCylinder = gluNewQuadric();
    children.resize(0);
}

branch::branch(branch *x){
    parent = x;
    // id = parent->id;
    radius = 0.015;
    length = 0.1;
    // These get set with getBase()
    X = 0;
    Y = 0;
    Z = 0;
    B_theta = 0;
    B_phi = 0;
    r_phi = 0;
    myCylinder = gluNewQuadric();
    children.resize(0);
    getBase();
}

branch::~branch(){}

void branch::draw(float r = 110.f / 255.f, float g = 44.f / 255.f, float b = 0.f / 255.f){
    drawSelf(r, g, b);
    // draw the children
    if(children.size() > 0){
        for(unsigned int i = 0; i < children.size(); i++){
            children[i].first.draw(children[i].second.r, children[i].second.g, children[i].second.b);
        }
    }
}

void branch::drawSelf(float r, float g, float b){
    glPushMatrix();
    // glColor3f(1,0,0);

    glTranslatef(X, Y, Z);
    glRotatef(-B_theta, Cos(B_phi), Sin(B_phi), 0);
    // glRotatef(-B_theta, X_AXIS, Y_AXIS, 0);

    glColor3f(r, g, b);

    gluCylinder(myCylinder, radius, radius * 0.8, length * 1.1, 20, 200);
    // gluQuadricNormals(myCylinder, GL_SMOOTH);
    // gluQuadricTexture(myCylinder, GL_TRUE);

    glPopMatrix();
}

void branch::grow(void){
    if(parent != 0){
        // Y = parent->Y + parent->length;
        getBase();
    }
    radius += 0.0001;
    length += 0.001;
    for(unsigned int i = 0; i < children.size(); i++){
        children[i].first.grow();
    }
}

void branch::grow(int n){
    for(int i = 0; i < n; i++){
        grow();
    }
}

void branch::bud(float r, float g, float b){
    if(children.size() == 0){
        double n_theta;
        double n_phi;
        n_theta = 25;
        n_phi = rand() % 180;
        // n_phi = 30;
        branch child1(this);
        child1.rotate(n_theta, n_phi);
        children.push_back(std::make_pair(child1, color(r, g, b)));
        branch child2(this);
        child2.rotate(-1 * n_theta, n_phi);
        children.push_back(std::make_pair(child2, color(r, g, b)));
        // std::cout << &child1 << " " << &child2 << std::endl; std::cout.flush();
        // branches.push_back(&child1);
        // branches.push_back(&child2);
    }
    else{
        for(unsigned int i = 0; i < children.size(); i++){
            children[i].first.bud(r, g, b);
        }
    }
}

void branch::rotate(double A, double B){
    float vx, vy, vz, Ax, Ay, Az, Bx, By, Bz, rx, ry, rz, l;
    r_phi = B;
    vx = Sin(parent->B_theta) * Cos(parent->B_phi);
    vy = Sin(parent->B_theta) * Sin(parent->B_phi);
    vz = Cos(parent->B_theta);

    if(parent->B_theta == 0){
        Ax = 1;
        Ay = 0;
        Az = 0;
        Bx = 0;
        By = 1;
        Bz = 0;
    }
    else if(parent->B_phi == 0){
        Ax = 1;
        Ay = 0;
        Az = 0;
        Bx = 0;
        By = 1;
        Bz = 0;
    }
    else if(parent->B_phi == 90){
        Ax = 0;
        Ay = 1;
        Az = 0;
        Bx = -1;
        By = 0;
        Bz = 0;
    }
    else if(parent->B_phi == 180){
        Ax = -1;
        Ay = 0;
        Az = 0;
        Bx = 0;
        By = -1;
        Bz = 0;
    }
    else if(parent->B_phi == 270){
        Ax = 0;
        Ay = -1;
        Az = 0;
        Bx = 1;
        By = 0;
        Bz = 0;
    }
    else{
        Ax = vz / sqrt((vx * vx) + (vz * vz));
        Ay = 0;
        Az = -vx / sqrt((vx * vx) + (vz * vz));
        Bx = 0;
        By = vz / sqrt((vy * vy) + (vz * vz));
        Bz = -vy / sqrt((vx * vx) + (vz * vz));
    }

    B += parent->r_phi;

    rx = (vx * Cos(A)) + (Sin(A) * Cos(B) * Ax) + (Sin(A) * Sin(B) * Bx);
    ry = (vy * Cos(A)) + (Sin(A) * Cos(B) * Ay) + (Sin(A) * Sin(B) * By);
    rz = (vz * Cos(A)) + (Sin(A) * Cos(B) * Az) + (Sin(A) * Sin(B) * Bz);

    l = sqrt((rx * rx) + (ry * ry) + (rz * rz));
    rx /= l;
    ry /= l;
    rz /= l;

    if(rz != 0){
        B_theta = Acos(rz);
    }
    else{
        B_theta = 0;
    }

    if((ry == 0) && (rx >= 0)){
        B_phi = 0;
    }
    else if((rx == 0) && (ry >= 0)){
        B_phi = 90;
    }
    else if((ry == 0) && (rx < 0)){
        B_phi = 180;
    }
    else if((rx == 0) && (ry < 0)){
        B_phi = 270;
    }
    else if(rx >= 0){
        B_phi = Atan(ry / rx);
    }
    else if(rx < 0){
        B_phi = Atan(ry / rx);
        B_theta *= -1;
    }

    // if(Verbose){
    //     std::cout << "parent theta: " << parent->B_theta << " phi: " << parent->B_phi << std::endl; std::cout.flush();
    //     std::cout << "v: " << vx << " " << vy << " " << vz << std::endl; std::cout.flush();
    //     std::cout << "A: " << Ax << " " << Ay << " " << Az << std::endl; std::cout.flush();
    //     std::cout << "B: " << Bx << " " << By << " " << Bz << std::endl; std::cout.flush();
    //     std::cout << "r: " << rx << " " << ry << " " << rz << std::endl; std::cout.flush();
    //     std::cout << "new theta: " << B_theta << " phi: " << B_phi << std::endl << std::endl; std::cout.flush();
    // }
}

void branch::getBase(void){
    // X = parent->X;
    X = parent->X - (parent->length * Sin(parent->B_theta) * Sin(parent->B_phi));
    Y = parent->Y + (parent->length * Sin(parent->B_theta) * Cos(parent->B_phi));
    Z = parent->Z + (parent->length * Cos(parent->B_theta));
}
#endif