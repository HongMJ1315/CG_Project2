#ifndef MAIN_H
#define MAIN_H

#include "load_model.h"
#include "tree.h"

#include <math.h>
#include <time.h>

#include <GL/freeglut.h>
#include <GL/glext.h>
#include <Eigen/Dense>

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#define WIDTH 600
#define HEIGHT 600


GLUquadricObj *sphere = NULL, *cylind = NULL, *disk;

float  points[][3] = { {0, 0, 0}, {1, 0, 0},
                      {1, 1, 0}, {0, 1, 0},
                      {0, 0, 1}, {1, 0, 1},
                      {1, 1, 1}, {0, 1, 1} };
/* face of box, each face composing of 4 vertices */
int    face[][4] = { {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5},
                    {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };
float  colors[6][3] = { {0.5,0.5,0.5}, {0.7,0.7,0.7}, {0.7,0.5,0.5},
                     {0.5,0.5,0.5}, {0.5,0.7,0.5}, {0.5,0.5,0.7} };

void Cube(float r, float g, float b){
    for(int i = 0; i < 6; i++){
        glBegin(GL_POLYGON);
        for(int j = 0; j < 4; j++){
            glColor3f(r, g, b);
            glVertex3fv(points[face[i][j]]);
        }
        glEnd();
    }
}

void Blade(){
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 4.0, 0.0);
    glVertex3f(1.0, 8.0, 0.0);
    glVertex3f(-1.0, 8.0, 0.0);
    glVertex3f(-1.0, 4.0, 0.0);
    glEnd();
}


/*-------------------------------------------------------
 * Procedure to draw three axes and the orign
 */
void draw_axes(){

    /*----Draw a white sphere to represent the original-----*/
    glColor3f(0.9, 0.9, 0.9);

    gluSphere(sphere, 2.0,   /* radius=2.0 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 8 stacks */

    /*----Draw three axes in colors, yellow, meginta, and cyan--*/
    /* Draw Z axis  */
    glColor3f(0.0, 0.95, 0.95);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */

    /* Draw Y axis */
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
    glColor3f(0.95, 0.0, 0.95);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();

    /* Draw X axis */
    glColor3f(0.95, 0.95, 0.0);
    glPushMatrix();
    glRotatef(90.0, 0.0, 1.0, 0.0);  /*Rotate about y  by 90', x becomes z */
    gluCylinder(cylind, 0.5, 0.5,   /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();
    /*-- Restore the original modelview matrix --*/
    glPopMatrix();
}

float csc(float x){
    return 1.0 / sin(x);
}

void Tire(float r, float g, float b){
    glPushMatrix();
    glColor3f(r, g, b);
    glutSolidTorus(2.0,  /* inner radius */
        2.0,  /* outer radius */
        24,   /* divided into 18 segments */
        12);  /* 12 rings */
    glPopMatrix();
}




Eigen::Matrix3f rotation_matrix(Eigen::Vector3f axis, double theta){
    axis = axis / axis.norm();
    double a = cos(theta / 2.0);
    double b, c, d;
    b = -axis(0) * sin(theta / 2.0);
    c = -axis(1) * sin(theta / 2.0);
    d = -axis(2) * sin(theta / 2.0);
    double aa, bb, cc, dd;
    aa = a * a; bb = b * b; cc = c * c; dd = d * d;
    double bc, ad, ac, ab, bd, cd;
    bc = b * c; ad = a * d; ac = a * c; ab = a * b; bd = b * d; cd = c * d;
    Eigen::Matrix3f rot_mat;
    rot_mat << aa + bb - cc - dd, 2 * (bc + ad), 2 * (bd - ac),
        2 * (bc - ad), aa + cc - bb - dd, 2 * (cd + ab),
        2 * (bd + ac), 2 * (cd - ab), aa + dd - bb - cc;
    return rot_mat;
}

Eigen::Vector3f rotate_matrix(Eigen::Vector3f O, Eigen::Vector3f P, float degree){
    double t = degree * PI / 180;  

    // 計算從O到P的向量
    Eigen::Vector3f OP = P - O;
    Eigen::Vector3f Z(0, 0, 1);

    // 定義M平面的向量
    // Eigen::Vector3f OM(O(0) - P(0), O(1) - P(1), 0);
    Eigen::Vector3f OM(0, 1, 0);

    // 計算旋轉軸
    Eigen::Vector3f axis = OP.cross(OM);

    // 正規化旋轉軸
    axis = axis / axis.norm();

    // 計算旋轉矩陣
    Eigen::Matrix3f R = rotation_matrix(axis, t);

    // 計算旋轉後的點
    Eigen::Vector3f P_prime = R * OP + O;
    return P_prime;
}

Eigen::Vector3f rotate_up(Eigen::Vector3f l, Eigen::Vector3f v, float degree){
    Eigen::Vector3f result;
    degree = degree * PI / 180;
    result = v * cos(degree) + (l.cross(v) * sin(degree)) + l * (l.dot(v)) * (1 - cos(degree));
    return result;
}
#endif