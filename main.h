#ifndef MAIN_H
#define MAIN_H

#include "load_model.h"
#include "tree.h"

#include <math.h>
#include <time.h>
#include <cmath>

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

#define SUN_LIGHT GL_LIGHT0
#define HELICOPTER_LIGHT1 GL_LIGHT1
#define HELICOPTER_LIGHT2 GL_LIGHT2

#define PAR_LIGHT GL_LIGHT3

#define FIXABLE_LIGHT GL_LIGHT4

#define ESP 1e-6

GLUquadricObj *sphere = NULL, *cylind = NULL, *disk;

float points[][3] = { {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1} };
int face[][4] = { {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };
float colors[6][3] = { {0.5, 0.5, 0.5}, {0.7, 0.7, 0.7}, {0.7, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, {0.5, 0.5, 0.7} };

bool helicopterLightStatus = 1;
bool sunLightStatus = 1;

enum material{
    METAL, RUBBER, CEMENT, WOOD, TEST, FLOOR
}MATERIAL;

void SetMaterial(material materialType, float r = 1, float g = 1, float b = 1){
    GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_shininess[] = { 0.0 };

    switch(materialType){
        case METAL: // Metal
        mat_ambient[0] = r; mat_ambient[1] = g; mat_ambient[2] = b;
        mat_diffuse[0] = r; mat_diffuse[1] = g; mat_diffuse[2] = b;
        mat_specular[0] = 0.508273; mat_specular[1] = 0.508273; mat_specular[2] = 0.508273;
        mat_shininess[0] = 51.2;
        break;

        case RUBBER: // Rubber
        mat_ambient[0] = 0.05; mat_ambient[1] = 0.05; mat_ambient[2] = 0.05;
        mat_diffuse[0] = 0.05; mat_diffuse[1] = 0.05; mat_diffuse[2] = 0.05;
        mat_specular[0] = 0.7; mat_specular[1] = 0.7; mat_specular[2] = 0.7;
        mat_shininess[0] = 0.078125;
        break;

        case CEMENT: // Cement
        mat_ambient[0] = 0.8; mat_ambient[1] = 0.8; mat_ambient[2] = 0.8;
        mat_diffuse[0] = 0.8; mat_diffuse[1] = 0.8; mat_diffuse[2] = 0.8;
        mat_specular[0] = 1.0; mat_specular[1] = 1.0; mat_specular[2] = 1.0;
        mat_shininess[0] = 0.6;
        break;

        case WOOD: // Wood
        mat_ambient[0] = 0.4; mat_ambient[1] = 0.2; mat_ambient[2] = 0.0;
        mat_diffuse[0] = 0.8; mat_diffuse[1] = 0.4; mat_diffuse[2] = 0.0;
        mat_specular[0] = 0.4; mat_specular[1] = 0.2; mat_specular[2] = 0.0;
        mat_shininess[0] = 0.1;
        break;

        case TEST:
        // ambient 1.0f, 0.5f, 0.31f
        // diffuse 1.0f, 0.5f, 0.31f
        // specular 0.5f, 0.5f, 0.5f
        // shininess 32.0f
        mat_ambient[0] = 1.0f; mat_ambient[1] = 0.5f; mat_ambient[2] = 0.31f;
        mat_diffuse[0] = 1.0f; mat_diffuse[1] = 0.5f; mat_diffuse[2] = 0.31f;
        mat_specular[0] = 0.5f; mat_specular[1] = 0.5f; mat_specular[2] = 0.5f;
        mat_shininess[0] = 32.0f;
        break;

        case FLOOR:
        mat_ambient[0] = r; mat_ambient[1] = g; mat_ambient[2] = b;
        mat_diffuse[0] = r; mat_diffuse[1] = g; mat_diffuse[2] = b;
        mat_specular[0] = 0.0; mat_specular[1] = 0.0; mat_specular[2] = 0.0;
        mat_shininess[0] = 0.0;


        default:
        break;
    }

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void sun_light(){
    // glEnable(SUN_LIGHT);

    GLfloat lightPosition[] = { 0.0, 100.0, 0.0, 1.0 };  // Light position (x, y, z, w)
    GLfloat lightAmbient[] = { 0.2, 0.2, 0.2, 1.0 };     // Ambient light color (RGBA)
    GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };     // Diffuse light color (RGBA)
    GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };    // Specular light color (RGBA)

    glLightfv(SUN_LIGHT, GL_POSITION, lightPosition);
    glLightfv(SUN_LIGHT, GL_AMBIENT, lightAmbient);
    glLightfv(SUN_LIGHT, GL_DIFFUSE, lightDiffuse);
    glLightfv(SUN_LIGHT, GL_SPECULAR, lightSpecular);
}

void helicopter_light(Eigen::Vector3f color, Eigen::Vector3f dir, bool isOn = true){
    // glEnable(HELICOPTER_LIGHT);

    float r = color.x();
    float g = color.y();
    float b = color.z();
    GLfloat lightPosition[] = { -0.5, 0.0, 0.0, 1.0 };  // Light position (x, y, z, w)
    GLfloat lightPosition2[] = { 0.5, 0.0, 0.0, 1.0 };  // Light position (x, y, z, w)
    GLfloat lightAmbient[] = { 0.2, 0.2, 0.2, 1.0 };     // Ambient light color (RGBA)
    GLfloat lightDiffuse[] = { r, g, b, 1.0 };     // Diffuse light color (RGBA)
    GLfloat lightSpecular[] = { r, g, b, 1.0 };    // Specular light color (RGBA)

    glLightfv(HELICOPTER_LIGHT1, GL_POSITION, lightPosition);
    glLightfv(HELICOPTER_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(HELICOPTER_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(HELICOPTER_LIGHT1, GL_SPECULAR, lightSpecular);


    GLfloat spot_direction[] = { dir.x(), dir.y(), dir.z() };
    glLightfv(HELICOPTER_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(HELICOPTER_LIGHT1, GL_SPOT_CUTOFF, 45.0);
    glLightf(HELICOPTER_LIGHT1, GL_SPOT_EXPONENT, 10.0);

    glLightfv(HELICOPTER_LIGHT2, GL_POSITION, lightPosition2);
    glLightfv(HELICOPTER_LIGHT2, GL_AMBIENT, lightAmbient);
    glLightfv(HELICOPTER_LIGHT2, GL_DIFFUSE, lightDiffuse);
    glLightfv(HELICOPTER_LIGHT2, GL_SPECULAR, lightSpecular);

    glLightfv(HELICOPTER_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf(HELICOPTER_LIGHT2, GL_SPOT_CUTOFF, 45.0);
    glLightf(HELICOPTER_LIGHT2, GL_SPOT_EXPONENT, 10.0);
}


void Cube(float r, float g, float b){
    for(int i = 0; i < 6; i++){
        glBegin(GL_POLYGON);
        SetMaterial(METAL, r, g, b);
        for(int j = 0; j < 4; j++){
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
    SetMaterial(METAL, 1.0, 1.0, 1.0);
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
    glColor3f(0.0, 0.0, 1.0);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */

    /* Draw Y axis */
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
    glColor3f(0.0, 1.0, 0.0);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();

    /* Draw X axis */
    glColor3f(1.0, 0.0, 0.0);
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
    SetMaterial(RUBBER, r, g, b);
    glutSolidTorus(2.0,  /* inner radius */
        2.0,  /* outer radius */
        24,   /* divided into 18 segments */
        12);  /* 12 rings */
    glPopMatrix();
}

void draw_helicopter(Eigen::Vector3f helicopterLocation, Eigen::Vector3f helicopterRotate, Eigen::Vector3f lightDir, float self_ang){
    float helicopterX = helicopterLocation(0);
    float helicopterY = helicopterLocation(1);
    float helicopterZ = helicopterLocation(2);
    float helicopterRotateX = helicopterRotate(0);
    float helicopterRotateY = helicopterRotate(1);
    float helicopterRotateZ = helicopterRotate(2);
    glPushMatrix();
    glTranslatef(helicopterX, helicopterY, helicopterZ + 12);
    glRotatef(-helicopterRotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(-helicopterRotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(-helicopterRotateZ, 0.0f, 0.0f, 1.0f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    // glPushMatrix();

    // glScalef(10.0f, 10.0f, 10.0f);

    helicopter_light(Eigen::Vector3f(1, 1, 1), lightDir);


    //Main body
    glPushMatrix();
    glTranslatef(-6.0f, 0, 0.0f);
    glScalef(12, 10, 12);
    Cube(0.10f, 0.44f, 0.24f);
    glPopMatrix();

    //Tail
    glPushMatrix();
    glTranslatef(-1.5, 0, -12);
    glScalef(3, 3, 12);
    Cube(0.67f, 0.70f, 0.73f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.5, 3, -12);
    glScalef(3, 5, 3);
    Cube(115.0 / 255.0, 198.0 / 255.0, 182.0 / 255.0);
    glPopMatrix();

    //Tail rotor support
    glPushMatrix();
    glTranslatef(1, 5, -12);
    glScalef(1, 3, 3);
    Cube(133.0 / 255.0, 193.0 / 255.0, 233.0 / 255.0);
    glPopMatrix();

    //Tire support back
    glPushMatrix();
    glTranslatef(-0.5, -1, -13);
    glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    glScalef(1, 3, 1);
    Cube(133.0 / 255.0, 193.0 / 255.0, 233.0 / 255.0);
    glPopMatrix();

    //Tire back
    glPushMatrix();
    glTranslatef(0, -1, -13);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.5, 0.5, 0.5);
    Tire(0, 0, 0);
    glPopMatrix();

    // Main rotor support
    glPushMatrix();
    glTranslatef(-1.5, 10, 5);
    glScalef(3, 3, 3);
    Cube(174.0 / 255.0, 214.0 / 255.0, 241.0 / 255.0);
    glPopMatrix();

    //Tire support right
    glPushMatrix();
    glTranslatef(7, -1, 3.5);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glScalef(1, 3, 1);
    Cube(133.0 / 255.0, 193.0 / 255.0, 233.0 / 255.0);
    glPopMatrix();

    //Tire right
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-4, -1, -7.5);
    glScalef(0.5, 0.5, 0.5);
    Tire(0, 0, 0);
    glPopMatrix();

    //Tier support left
    glPushMatrix();
    glTranslatef(-8, -1, 3.5);
    glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);
    glScalef(1, 3, 1);
    Cube(133.0 / 255.0, 193.0 / 255.0, 233.0 / 255.0);
    glPopMatrix();

    //Tire left
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-4, -1, 7.5);
    glScalef(0.5, 0.5, 0.5);
    Tire(0, 0, 0);
    glPopMatrix();


    /*
    glColor3f(0.10f, 0.44f, 0.24f);
    // helicopterBody.draw();

    glColor3f(0.67f, 0.70f, 0.73f);
    // helicopterBackSupport.draw();

    glColor3f(0.0f, 0.0f, 0.0f);
    // helicopterBackTire.draw();
    // helicopterLeftTire.draw();
    // helicopterRightTire.draw();
    */

    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslated(0, 13.1, 7);
    glRotated(self_ang, 0.0, 1.0, 0.0);
    for(int i = 0; i < 4; i++){
        glPushMatrix();
        glTranslated(1, 0, -38);
        glScalef(1, 1, 5);
        glRotated(90.0, 1.0, 0.0, 0.0);
        Blade();

        glPopMatrix();
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    }
    glPopMatrix();
    glPushMatrix();
    glTranslated(2.1, 6.5, -10.5);
    glRotated(self_ang, 1.0, 0.0, 0.0);
    for(int i = 0; i < 4; i++){
        glPushMatrix();
        glTranslated(0, -8, 0.5);
        glScalef(1, 1, 0.5);
        glRotated(90.0, 0, 1.0, 0.0);
        Blade();

        glPopMatrix();
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    }
    glPopMatrix();

    glPopMatrix();
}

void draw_tree(std::pair<int, int> *treePos, float *treeRotate, branch *tree, int tree_num){
    for(int i = 0; i < tree_num; i++){
        // glColor3f(1, 1, 1);
        glPushMatrix();
        int tx = treePos[i].first;
        int tz = treePos[i].second;
        glTranslatef(tx, 0.0, tz);
        glRotatef(treeRotate[i], 0.0f, 1.0f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

        glScaled(10, 10, 10);
        tree[i].draw();
        glPopMatrix();
    }
}



void draw_building(std::pair<int, int> *buildingPos, float *buildingRotate, Model building, int building_num){
    for(int i = 0; i < building_num; i++){
        SetMaterial(CEMENT, 0.1, 0.1, 0.1);
        glPushMatrix();
        int tx = buildingPos[i].first;
        int tz = buildingPos[i].second;
        glTranslatef(tx, 0.0, tz);
        glRotatef(buildingRotate[i], 0.0f, 1.0f, 0.0f);
        glScaled(0.01, 0.01, 0.01);
        building.draw();
        glPopMatrix();
    }
}

void draw_par_light(Eigen::Vector3f loc, float rotate, Eigen::Vector3f color, Model par_light, bool test = 1){
    glEnable(PAR_LIGHT);
    glPushMatrix();
    glTranslatef(loc.x(), loc.y(), loc.z());
    // glColor3f(color.x(), color.y(), color.z());
    //rotate light
    glRotatef(rotate, 0.0f, 1.0f, 0.0f);
    SetMaterial(METAL, 0.1, 0.1, 0.1);
    if(test) par_light.draw();
    float r = color.x();
    float g = color.y();
    float b = color.z();
    GLfloat lightPosition[] = { loc.x() - 7.5, loc.y() + 7.5, loc.z(), 1.0 };  // Light position (x, y, z, w)
    GLfloat lightAmbient[] = { 0.2, 0.2, 0.2, 1.0 };     // Ambient light color (RGBA)
    GLfloat lightDiffuse[] = { r, g, b, 1.0 };     // Diffuse light color (RGBA)
    GLfloat lightSpecular[] = { r, g, b, 1.0 };    // Specular light color (RGBA)
    glPushMatrix();
    glTranslatef(-7.5, 7.5, 0);
    SetMaterial(METAL, 1, 1, 1);
    Cube(1, 1, 1);
    glPopMatrix();
    float cutoff = 30.0;
    float exponent = 10.0;
    glLightfv(PAR_LIGHT, GL_POSITION, lightPosition);
    glLightfv(PAR_LIGHT, GL_AMBIENT, lightAmbient);
    glLightfv(PAR_LIGHT, GL_DIFFUSE, lightDiffuse);
    glLightfv(PAR_LIGHT, GL_SPECULAR, lightSpecular);

    GLfloat spot_direction[] = { -0.5, 0.5, 0.0 };
    glLightfv(PAR_LIGHT, GL_SPOT_DIRECTION, spot_direction);
    glLightf(PAR_LIGHT, GL_SPOT_CUTOFF, cutoff);
    glLightf(PAR_LIGHT, GL_SPOT_EXPONENT, exponent);
    glPopMatrix();
}

void draw_fixable_light(Eigen::Vector3f loc, Eigen::Vector3f dir, Eigen::Vector3f color, float cutoff = 45.0){
    glPushMatrix();
    glTranslatef(loc.x(), loc.y(), loc.z());
    float r = color.x();
    float g = color.y();
    float b = color.z();
    GLfloat lightPosition[] = { 0, 0, 0, 1.0 };  // Light position (x, y, z, w)
    GLfloat lightAmbient[] = { 0.2, 0.2, 0.2, 1.0 };     // Ambient light color (RGBA)
    GLfloat lightDiffuse[] = { r, g, b, 1.0 };     // Diffuse light color (RGBA)
    GLfloat lightSpecular[] = { r, g, b, 1.0 };    // Specular light color (RGBA)
    glLightfv(FIXABLE_LIGHT, GL_POSITION, lightPosition);
    glLightfv(FIXABLE_LIGHT, GL_AMBIENT, lightAmbient);
    glLightfv(FIXABLE_LIGHT, GL_DIFFUSE, lightDiffuse);
    glLightfv(FIXABLE_LIGHT, GL_SPECULAR, lightSpecular);
    Cube(1, 1, 1);

    GLfloat spot_direction[] = { dir.x(), dir.y(), dir.z() };
    glLightfv(FIXABLE_LIGHT, GL_SPOT_DIRECTION, spot_direction);
    glLightf(FIXABLE_LIGHT, GL_SPOT_CUTOFF, cutoff);
    glLightf(FIXABLE_LIGHT, GL_SPOT_EXPONENT, 10.0);

    glPopMatrix();
}



// /*
void draw_view_volume(Eigen::Vector3f helicopterPos, Eigen::Vector3f lookAtPos, float fov, float nearClip, float farClip, float aspectRatio, float upDegree){
    // float fov = CLIP_DEGREE;  // 視野角度
    // float nearClip = NEAR_CLIP;  // 近裁剪面
    // float farClip = FAR_CLIP;  // 遠裁剪面
    // float aspectRatio = (float) width / (float) height;  // 視圖的擴張比例

    // 計算近裁剪面的一半高度和寬度
    float nearHeight = tan(fov / 2.0 * (PI / 180.0)) * nearClip;
    float nearWidth = nearHeight * aspectRatio;
    float farHeight = tan(fov / 2.0 * (PI / 180.0)) * farClip;
    float farWidth = farHeight * aspectRatio;

    // 利用相機位置和觀察點位置計算近裁剪面的中心點
    // Eigen::Vector3f helicopterPos(helicopterX, helicopterY, helicopterZ);
    // Eigen::Vector3f lookAtPos(lookAtX, lookAtY, lookAtZ);
    Eigen::Vector3f viewDir = (lookAtPos - helicopterPos).normalized();
    Eigen::Vector3f nearClipCenter = lookAtPos - nearClip * viewDir,
        farClipCenter = lookAtPos - farClip * viewDir;

    Eigen::Vector3f nearCilpTopRight, nearClipTopLeft, nearClipBottomRight, nearClipBottomLeft;
    Eigen::Vector3f farCilpTopRight, farClipTopLeft, farClipBottomRight, farClipBottomLeft;
    Eigen::Vector3f lr(viewDir.z(), 0, -viewDir.x());
    Eigen::Vector3f ud = lr.cross(viewDir);
    ud = ud / ud.norm();
    lr = lr / lr.norm();
    Eigen::Vector3f rud = rotate_up(viewDir, ud, upDegree);
    Eigen::Vector3f rlr = rotate_up(viewDir, lr, upDegree);
    nearCilpTopRight = nearClipCenter + nearWidth * rlr + nearHeight * rud;
    nearClipTopLeft = nearClipCenter - nearWidth * rlr + nearHeight * rud;
    nearClipBottomRight = nearClipCenter + nearWidth * rlr - nearHeight * rud;
    nearClipBottomLeft = nearClipCenter - nearWidth * rlr - nearHeight * rud;
    farCilpTopRight = farClipCenter + farWidth * rlr + farHeight * rud;
    farClipTopLeft = farClipCenter - farWidth * rlr + farHeight * rud;
    farClipBottomRight = farClipCenter + farWidth * rlr - farHeight * rud;
    farClipBottomLeft = farClipCenter - farWidth * rlr - farHeight * rud;
    //

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(nearClipTopLeft.x(), nearClipTopLeft.y(), nearClipTopLeft.z());
    glVertex3f(nearCilpTopRight.x(), nearCilpTopRight.y(), nearCilpTopRight.z());
    glVertex3f(nearClipBottomRight.x(), nearClipBottomRight.y(), nearClipBottomRight.z());
    glVertex3f(nearClipBottomLeft.x(), nearClipBottomLeft.y(), nearClipBottomLeft.z());
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(nearClipTopLeft.x(), nearClipTopLeft.y(), nearClipTopLeft.z());
    glVertex3f(farClipTopLeft.x(), farClipTopLeft.y(), farClipTopLeft.z());
    glVertex3f(nearCilpTopRight.x(), nearCilpTopRight.y(), nearCilpTopRight.z());
    glVertex3f(farCilpTopRight.x(), farCilpTopRight.y(), farCilpTopRight.z());
    glVertex3f(nearClipBottomRight.x(), nearClipBottomRight.y(), nearClipBottomRight.z());
    glVertex3f(farClipBottomRight.x(), farClipBottomRight.y(), farClipBottomRight.z());
    glVertex3f(nearClipBottomLeft.x(), nearClipBottomLeft.y(), nearClipBottomLeft.z());
    glVertex3f(farClipBottomLeft.x(), farClipBottomLeft.y(), farClipBottomLeft.z());
    glEnd();
}

void draw_view(Eigen::Vector3f helicopterLocation, float u[3][3]){
    int    i;
    float  x = helicopterLocation.x() + 10, y = helicopterLocation.y(), z = helicopterLocation.z();
    glMatrixMode(GL_MODELVIEW);
    /*----Draw Eye position-----*/
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(0.0, 1.0, 0.0);
    glutWireSphere(1.0, 10, 10);
    glPopMatrix();

    /*----Draw eye coord. axes -----*/
    glColor3f(1.0, 0.0, 0.0);           /* Draw Xe */
    glBegin(GL_LINES);
    glVertex3f(x, y, z);
    glVertex3f(x + 20.0 * u[0][0], y + 20.0 * u[0][1], z + 20.0 * u[0][2]);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);          /* Draw Ye */
    glBegin(GL_LINES);
    glVertex3f(x, y, z);
    glVertex3f(x + 20.0 * u[1][0], y + 20.0 * u[1][1], z + 20.0 * u[1][2]);
    glEnd();

    glColor3f(0.0, 0.0, 1.0);          /* Draw Ze */
    glBegin(GL_LINES);
    glVertex3f(x, y, z);
    glVertex3f(x + 20.0 * u[2][0], y + 20.0 * u[2][1], z + 20.0 * u[2][2]);
    glEnd();
}

void draw_floor(int len){
    int i, j;
    for(i = 0; i < len; i++)
        for(j = 0; j < len; j++){
            if((i + j) % 2 == 0){
                // glColor3f(0.7, 0.7, 0.7);
                SetMaterial(FLOOR, 0.7, 0.7, 0.7);
            }
            else{
                // glColor3f(0.1, 0.1, 0.7);
                SetMaterial(FLOOR, 0.1, 0.1, 0.7);
            }
            glBegin(GL_POLYGON);
            glVertex3f((i - 5.0) * 10.0, -2.5, (j - 5.0) * 10.0);
            glVertex3f((i - 5.0) * 10.0, -2.5, (j - 4.0) * 10.0);
            glVertex3f((i - 4.0) * 10.0, -2.5, (j - 4.0) * 10.0);
            glVertex3f((i - 4.0) * 10.0, -2.5, (j - 5.0) * 10.0);
            glEnd();
        }
}

Eigen::Vector3f move_camera_ud(Eigen::Vector3f O, Eigen::Vector3f P, float degree){
    // Eigen::Vector3f O(helicopterX, helicopterY, helicopterZ);
    // Eigen::Vector3f P(lookAtX, lookAtY, lookAtZ);
    Eigen::Vector3f result = rotate_matrix(O, P, degree);
    Eigen::Transpose<Eigen::Vector3f> loc = result.transpose();

    Eigen::Vector3f tresult = result - O;
    Eigen::Vector3f tv = Eigen::Vector3f(0, 1, 0);
    float cos_theta = tresult.dot(tv) / (tresult.norm() * tv.norm());
    float angle = acos(cos_theta) * 180.0 / PI;

    if(angle < ESP || 180.0f - angle < ESP) return P;
    else return result;
}

Eigen::Vector3f move_camera_lr(Eigen::Vector3f O, Eigen::Vector3f P, float degree){
    //旋轉中心
    float center_x = O.x(),
        center_z = O.z();
    //移回中心
    float tox = P.x() - center_x,
        toz = P.z() - center_z;

    degree = degree * PI / 180.0f;
    //旋轉
    float tx = tox * cos(degree) - toz * sin(degree),
        tz = tox * sin(degree) + toz * cos(degree);
    //移回
    return Eigen::Vector3f(tx + center_x, P.y(), tz + center_z);
}

Eigen::Vector3f up_vector(Eigen::Vector3f O, Eigen::Vector3f P, float degree){
    Eigen::Vector3f l = P - O;
    Eigen::Vector3f v(l.z(), 0, -l.x());
    l = l / l.norm();
    v = v / v.norm();
    Eigen::Vector3f result = rotate_up(l, v, degree);
    return result;
}

// */


#endif