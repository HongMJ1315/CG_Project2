#ifndef MAIN_H
#define MAIN_H

#include "load_model.h"
#include "perlin.h"
#include "img_split.h"
#include "rotate_function.h"

#include <math.h>
#include <time.h>
#include <cmath>

#include <GL/freeglut.h>
#include <GL/glext.h>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>


#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#define WIDTH 600
#define HEIGHT 600

#define SUN_LIGHT GL_LIGHT0
#define HELICOPTER_LIGHT1 GL_LIGHT1
#define HELICOPTER_LIGHT2 GL_LIGHT2

#define PAR_LIGHT0 GL_LIGHT3
#define PAR_LIGHT1 GL_LIGHT4
#define PAR_LIGHT2 GL_LIGHT5

#define FIXABLE_LIGHT GL_LIGHT6

#define CANDLE_LIGHT GL_LIGHT7

#define BILLBOARD_NUM 20


#define TEXTURE_SIZE 256
#define BILLBOARD_SIZE 512
#define ESP 1e-6
#define MIRROR_TEXTURE_SIZE 256

GLUquadricObj *sphere = NULL, *cylind = NULL, *disk;


float points[][3] = { {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1} };
int face[][4] = { {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };
float colors[6][3] = { {0.5, 0.5, 0.5}, {0.7, 0.7, 0.7}, {0.7, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.7, 0.5}, {0.5, 0.5, 0.7} };
float mtx[16];
float a[3], b[3];
unsigned int textName[10];
unsigned int billboardName[BILLBOARD_NUM];

bool helicopterLightStatus = 1;
bool sunLightStatus = 1;

void Cube(float, float, float);

enum material{
    METAL, RUBBER, CEMENT, WOOD, TEST, FLOOR, WAX, CANDEL
}MATERIAL;

enum texture : int32_t{
    METAL_TEXTURE = 0, WOOD_TEXTURE = 1, WOOD2_TEXTURE, CEMENT_TEXTURE, RUBBER_TEXTURE, EARTH_TEXTURE, SKY_TEXTURE, MIRROR_TEXTURE
}TEXTURE;

enum billboard : int32_t{
    TREE_BILLBOARD = 0, GRASS1_BILLBOARD = 1, GRASS2_BILLBOARD, FLOWER1_BILLBOARD, FLOWER2_BILLBOARD,
    BIRD1_BILLBOARD, BIRD2_BILLBOARD, BIRD3_BILLBOARD, BIRD4_BILLBOARD, BIRD5_BILLBOARD, BIRD6_BILLBOARD,
    BIRD7_BILLBOARD, BIRD8_BILLBOARD, BIRD9_BILLBOARD
}BILLBOARD;

void ComputeABAxes(){
    float w0, w2;
    double len;

    /*----Get w0 and w2 from the modelview matrix mtx[] ---*/
    w0 = mtx[2];
    w2 = mtx[10];

    len = sqrt(w0 * w0 + w2 * w2);
    /*---- Define the a and b axes for billboards ----*/
    b[0] = 0.0;
    b[1] = 1.0;
    b[2] = 0.0;
    a[0] = w2 / len;
    a[1] = 0.0;
    a[2] = -w0 / len;
}

void ReadTexture(unsigned char texture[TEXTURE_SIZE][TEXTURE_SIZE][4], char *filename, int width = TEXTURE_SIZE, int height = TEXTURE_SIZE){
    cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);
    if(img.empty()){
        std::cout << "Could not read the image: " << filename << std::endl;
        return;
    }
    cv::resize(img, img, cv::Size(width, height));

    int index = 0;
    for(int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            cv::Vec3b color = img.at<cv::Vec3b>(cv::Point(j, i));
            texture[i][j][0] = color[2];
            texture[i][j][1] = color[1];
            texture[i][j][2] = color[0];
            texture[i][j][3] = 1.0;
        }
    }
    for(int i = 0; i < img.rows / 2; i++){
        for(int j = 0; j < img.cols; j++){
            std::swap(texture[i][j][0], texture[img.rows - i - 1][j][0]);
            std::swap(texture[i][j][1], texture[img.rows - i - 1][j][1]);
            std::swap(texture[i][j][2], texture[img.rows - i - 1][j][2]);
            std::swap(texture[i][j][3], texture[img.rows - i - 1][j][3]);
        }
    }
}

void ReadBillboard(unsigned char texture[BILLBOARD_SIZE][BILLBOARD_SIZE][4], char *filename, int width = BILLBOARD_SIZE, int height = BILLBOARD_SIZE){
    cv::Mat img = cv::imread(filename, cv::IMREAD_UNCHANGED); // 讀取圖像，包括alpha通道
    if(img.empty()){
        std::cout << "Could not read the image: " << filename << std::endl;
        return;
    }
    cv::resize(img, img, cv::Size(width, height));

    for(int i = 0; i < img.rows; i++){
        for(int j = 0; j < img.cols; j++){
            cv::Vec4b color = img.at<cv::Vec4b>(cv::Point(j, i)); // 使用Vec4b來存儲RGBA顏色
            texture[i][j][0] = color[2]; // R
            texture[i][j][1] = color[1]; // G
            texture[i][j][2] = color[0]; // B
            texture[i][j][3] = color[3]; // A
        }
    }
    for(int i = 0; i < img.rows / 2; i++){
        for(int j = 0; j < img.cols; j++){
            std::swap(texture[i][j][0], texture[img.rows - i - 1][j][0]);
            std::swap(texture[i][j][1], texture[img.rows - i - 1][j][1]);
            std::swap(texture[i][j][2], texture[img.rows - i - 1][j][2]);
            std::swap(texture[i][j][3], texture[img.rows - i - 1][j][3]);
        }
    }
}

void TextureInit(texture textType, unsigned int *textName, unsigned char texture[TEXTURE_SIZE][TEXTURE_SIZE][4], int width, int height){
    glBindTexture(GL_TEXTURE_2D, textName[textType]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT = repeat texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
}

void BillboardInit(billboard billboardType, unsigned int *billboardName, unsigned char texture[BILLBOARD_SIZE][BILLBOARD_SIZE][4], int width, int height){
    glBindTexture(GL_TEXTURE_2D, billboardName[billboardType]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT = repeat texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, BILLBOARD_SIZE, BILLBOARD_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
}

void SetTexture(texture textType, unsigned int *textName){
    float diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    float specular[] = { 1.0, 1.0, 1.0, 1.0 };
    float ambient[] = { 0.8, 0.8, 0.8, 1.0 };
    float emission[] = { 0.0, 0.0, 0.0, 1.0 };
    float shininess = 0.0;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textName[textType]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    switch(textType){
        case METAL_TEXTURE:
        shininess = 0.6;
        specular[0] = 0.508273; specular[1] = 0.508273; specular[2] = 0.508273;
        break;

        case WOOD_TEXTURE: case WOOD2_TEXTURE: case EARTH_TEXTURE:
        shininess = 0.1;
        specular[0] = 0.4; specular[1] = 0.2; specular[2] = 0.0;
        break;

        case RUBBER_TEXTURE:
        shininess = 0.078125;
        break;

        case CEMENT_TEXTURE:
        shininess = 0.08;
        specular[0] = 0.0; specular[1] = 0.0; specular[2] = 0.0;
        break;

        default:
        break;
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
}

void SetBillboard(billboard billboardType, unsigned int *billboardName){
    float diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    float specular[] = { 1.0, 1.0, 1.0, 1.0 };
    float ambient[] = { 0.8, 0.8, 0.8, 1.0 };
    float emission[] = { 0.0, 0.0, 0.0, 1.0 };
    float shininess = 0.0;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, billboardName[billboardType]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    switch(billboardType){
        case TREE_BILLBOARD:
        shininess = 0.6;
        specular[0] = 0.508273; specular[1] = 0.508273; specular[2] = 0.508273;
        break;

        default:
        break;
    }
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
}

void SetMaterial(material materialType, float r = 1, float g = 1, float b = 1){
    GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_shininess[] = { 0.0 };
    GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 0.0 };

    switch(materialType){
        case METAL: // Metal
        mat_ambient[0] = r; mat_ambient[1] = g; mat_ambient[2] = b;
        mat_diffuse[0] = r; mat_diffuse[1] = g; mat_diffuse[2] = b;
        mat_specular[0] = 0.508273 * r; mat_specular[1] = 0.508273; mat_specular[2] = 0.508273 * b;
        mat_shininess[0] = 64;
        break;

        case RUBBER: // Rubber
        mat_ambient[0] = 0.05; mat_ambient[1] = 0.05; mat_ambient[2] = 0.05;
        mat_diffuse[0] = 0.05; mat_diffuse[1] = 0.05; mat_diffuse[2] = 0.05;
        mat_specular[0] = 0.1; mat_specular[1] = 0.1; mat_specular[2] = 0.1;
        mat_shininess[0] = 0.078125;
        break;

        case CEMENT: // Cement
        mat_ambient[0] = 0.8; mat_ambient[1] = 0.8; mat_ambient[2] = 0.8;
        mat_diffuse[0] = 0.8; mat_diffuse[1] = 0.8; mat_diffuse[2] = 0.8;
        mat_specular[0] = 0.0; mat_specular[1] = 0.0; mat_specular[2] = 0.0;
        mat_shininess[0] = 0.08;
        break;

        case WOOD: // Wood
        mat_ambient[0] = 0.4; mat_ambient[1] = 0.2; mat_ambient[2] = 0.0;
        mat_diffuse[0] = 0.8; mat_diffuse[1] = 0.4; mat_diffuse[2] = 0.0;
        mat_specular[0] = 0.4; mat_specular[1] = 0.2; mat_specular[2] = 0.0;
        mat_shininess[0] = 0.1;
        break;

        case FLOOR:
        mat_ambient[0] = r; mat_ambient[1] = g; mat_ambient[2] = b;
        mat_diffuse[0] = r; mat_diffuse[1] = g; mat_diffuse[2] = b;
        mat_specular[0] = 0.0; mat_specular[1] = 0.0; mat_specular[2] = 0.0;
        mat_shininess[0] = 0.0;

        case WAX:
        mat_ambient[0] = r; mat_ambient[1] = g; mat_ambient[2] = b;
        mat_diffuse[0] = r; mat_diffuse[1] = g; mat_diffuse[2] = b;
        mat_specular[0] = 0.3; mat_specular[1] = 0.3; mat_specular[2] = 0.3;
        mat_shininess[0] = 10;

        break;

        case CANDEL:
        mat_ambient[0] = r; mat_ambient[1] = g; mat_ambient[2] = b;
        mat_diffuse[0] = r; mat_diffuse[1] = g; mat_diffuse[2] = b;
        mat_specular[0] = 0.5; mat_specular[1] = 0.5; mat_specular[2] = 0.5;
        mat_shininess[0] = 32;
        mat_emission[0] = r; mat_emission[1] = g; mat_emission[2] = b;
        mat_emission[3] = 1.0;
        break;


        case TEST:
        // ambient 1.0f, 0.5f, 0.31f
        // diffuse 1.0f, 0.5f, 0.31f
        // specular 0.5f, 0.5f, 0.5f
        // shiness 32.0f
        mat_ambient[0] = 1.0f; mat_ambient[1] = 0.5f; mat_ambient[2] = 0.31f;
        mat_diffuse[0] = 1.0f; mat_diffuse[1] = 0.5f; mat_diffuse[2] = 0.31f;
        mat_specular[0] = 0.5f; mat_specular[1] = 0.5f; mat_specular[2] = 0.5f;
        mat_shininess[0] = 32.0f;
        break;

        default:
        break;
    }

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
}

void DrawBillboard(float x, float y, float z, float w, float h, billboard billboardType, unsigned int *billboardName){
    float v0[3], v1[3], v2[3], v3[3];


    // glPushMatrix();
    // glTranslatef(x, y, z);
    // glScalef(2, 2, 2);
    // Cube(1, 1, 1);
    // glPopMatrix();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    /*----Compute the 4 vertices of the billboard ----*/
    v0[0] = x - (w / 2) * a[0];
    v0[1] = 0.0;
    v0[2] = z - (w / 2) * a[2];
    v1[0] = x + (w / 2) * a[0];
    v1[1] = 0.0;
    v1[2] = z + (w / 2) * a[2];
    v2[0] = x + (w / 2) * a[0];
    v2[1] = h;
    v2[2] = z + (w / 2) * a[2];
    v3[0] = x - (w / 2) * a[0];
    v3[1] = h;
    v3[2] = z - (w / 2) * a[2];
    // std::cout << v0[0] << " " << v0[1] << " " << v0[2] << std::endl;
    // std::cout << v1[0] << " " << v1[1] << " " << v1[2] << std::endl;
    // std::cout << v2[0] << " " << v2[1] << " " << v2[2] << std::endl;
    // std::cout << v3[0] << " " << v3[1] << " " << v3[2] << std::endl;
    SetBillboard(billboardType, billboardName);
    // SetTexture(METAL_TEXTURE, textName);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(v0);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(v1);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(v2);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(v3);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

void DrawSunLight(glm::vec3 color, float instance){
    // glEnable(SUN_LIGHT);
    glPushMatrix();
    glTranslatef(225, 10, 225);
    float r = color.x, g = color.y, b = color.z;
    GLfloat lightPosition[] = { 0, 0, 0, 0.0 };  // Light position (x, y, z, w), w=0 for directional light
    GLfloat lightAmbient[] = { r * 0.7 * instance, g * 0.7 * instance, b * 0.7 * instance, 1.0 };     // Ambient light color (RGBA)
    GLfloat lightDiffuse[] = { r, g, b, 1.0 };     // Diffuse light color (RGBA)
    GLfloat lightSpecular[] = { r, g, b, 1.0 };    // Specular light color (RGBA)
    GLfloat lightDirection[] = { 0, -1, 0 };  // Light direction for directional light
    glLightfv(SUN_LIGHT, GL_POSITION, lightPosition);
    glLightfv(SUN_LIGHT, GL_AMBIENT, lightAmbient);
    glLightfv(SUN_LIGHT, GL_DIFFUSE, lightDiffuse);
    glLightfv(SUN_LIGHT, GL_SPECULAR, lightSpecular);
    glLightfv(SUN_LIGHT, GL_SPOT_DIRECTION, lightDirection);

    // glScalef(10, 10, 10);
    // Cube(1, 1, 1);
    glPopMatrix();
}

void DrawMirror(glm::vec3 mirrorLoc, int width, int height, texture textureType){
    glPushMatrix();
    glTranslatef(mirrorLoc.x, mirrorLoc.y, mirrorLoc.z);
    glScalef(width, height, 1);
    SetTexture(textureType, textName);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5, 0, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.5, 0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.5, 1, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-0.5, 1, 0.0);
    glEnd();
    glPopMatrix();
}
void HelicopterLight(glm::vec3 color, glm::vec3 dir, float cutoff, float intensity, bool isOn = true){
    // glEnable(HELICOPTER_LIGHT);

    float r = color.x * intensity;
    float g = color.y * intensity;
    float b = color.z * intensity;
    r = std::min(r, 1.0f);
    g = std::min(g, 1.0f);
    b = std::min(b, 1.0f);
    r = std::max(r, 0.0f);
    g = std::max(g, 0.0f);
    b = std::max(b, 0.0f);
    GLfloat lightPosition[] = { -0.5, 0.0, 0.0, 1.0 };  // Light position (x, y, z, w)
    GLfloat lightPosition2[] = { 0.5, 0.0, 0.0, 1.0 };  // Light position (x, y, z, w)
    GLfloat lightAmbient[] = { 0.2 * intensity * r, 0.2 * intensity * g, 0.2 * intensity * b, 1.0 };     // Ambient light color (RGBA)
    GLfloat lightDiffuse[] = { r, g, b, 1.0 };     // Diffuse light color (RGBA)
    GLfloat lightSpecular[] = { r, g, b, 1.0 };    // Specular light color (RGBA)

    glLightfv(HELICOPTER_LIGHT1, GL_POSITION, lightPosition);
    glLightfv(HELICOPTER_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(HELICOPTER_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(HELICOPTER_LIGHT1, GL_SPECULAR, lightSpecular);


    GLfloat spot_direction[] = { dir.x, dir.y, dir.z };
    glLightfv(HELICOPTER_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(HELICOPTER_LIGHT1, GL_SPOT_CUTOFF, cutoff);
    glLightf(HELICOPTER_LIGHT1, GL_SPOT_EXPONENT, 5.0);
    glLightf(HELICOPTER_LIGHT1, GL_CONSTANT_ATTENUATION, 2.0);

    glLightfv(HELICOPTER_LIGHT2, GL_POSITION, lightPosition2);
    glLightfv(HELICOPTER_LIGHT2, GL_AMBIENT, lightAmbient);
    glLightfv(HELICOPTER_LIGHT2, GL_DIFFUSE, lightDiffuse);
    glLightfv(HELICOPTER_LIGHT2, GL_SPECULAR, lightSpecular);

    glLightfv(HELICOPTER_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf(HELICOPTER_LIGHT2, GL_SPOT_CUTOFF, cutoff);
    glLightf(HELICOPTER_LIGHT2, GL_SPOT_EXPONENT, 5.0);
    glLightf(HELICOPTER_LIGHT2, GL_CONSTANT_ATTENUATION, 2.0);
}

void SkyBox(){
    // glScalef(1.0, 3.0, 1.0);
    // SetTexture(METAL_TEXTURE, textName);
    glPushMatrix();
    glScalef(750, 750, 750);
    SetTexture(SKY_TEXTURE, textName);
    for(int i = 0; i < 6; i++){
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(points[face[i][0]]);
        glTexCoord2f(1.0, 0.0);
        glVertex3fv(points[face[i][1]]);
        glTexCoord2f(1.0, 1.0);
        glVertex3fv(points[face[i][2]]);
        glTexCoord2f(0.0, 1.0);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }
    // GLUquadric *quad = gluNewQuadric();
    // gluSphere(quad, 1.0, 100, 100);
    // gluDeleteQuadric(quad);

    glPopMatrix();
}

void Cube(float r, float g, float b){
    // glScalef(1.0, 3.0, 1.0);
    SetTexture(METAL_TEXTURE, textName);
    for(int i = 0; i < 6; i++){
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(points[face[i][0]]);
        glTexCoord2f(8.0, 0.0);
        glVertex3fv(points[face[i][1]]);
        glTexCoord2f(8.0, 8.0);
        glVertex3fv(points[face[i][2]]);
        glTexCoord2f(0.0, 8.0);
        glVertex3fv(points[face[i][3]]);
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




/*-------------------------------------------------------
 * Procedure to draw three axes and the orign
 */
void DrawAxes(){

    /*----Draw a white sphere to represent the original-----*/
    // glColor3f(0.9, 0.9, 0.9);
    SetMaterial(METAL, 0.9, 0.9, 0.9);
    gluSphere(sphere, 2.0,   /* radius=2.0 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 8 stacks */

    /*----Draw three axes in colors, yellow, meginta, and cyan--*/
    /* Draw Z axis  */
    // glColor3f(0.0, 0.0, 1.0);
    SetMaterial(METAL, 0, 0, 1.0);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */

    /* Draw Y axis */
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
    // glColor3f(0.0, 1.0, 0.0);
    SetMaterial(METAL, 0, 1, 0);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();

    /* Draw X axis */
    // glColor3f(1.0, 0.0, 0.0);
    SetMaterial(METAL, 1, 0, 0);
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

void DrawBird(glm::vec3 loc, int animeFram, billboard billboardType){
    DrawBillboard(loc.x, loc.y, loc.z, 30, 30, billboard(billboardType + animeFram), billboardName);
}

void DrawHelicopter(glm::vec3 helicopterLocation, glm::vec3 helicopterRotate, glm::vec3 lightDir, glm::vec3 color, float self_ang, float cutoff, float intensity){
    float helicopterX = helicopterLocation.x;
    float helicopterY = helicopterLocation.y;
    float helicopterZ = helicopterLocation.z;
    float helicopterRotateX = helicopterRotate.x;
    float helicopterRotateY = helicopterRotate.y;
    float helicopterRotateZ = helicopterRotate.z;
    // std::cout << helicopterX << " " << helicopterY << " " << helicopterZ << std::endl;
    glPushMatrix();
    glTranslatef(helicopterX, helicopterY, helicopterZ + 12);
    glRotatef(-helicopterRotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(-helicopterRotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(-helicopterRotateZ, 0.0f, 0.0f, 1.0f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    // glPushMatrix();

    // glScalef(10.0f, 10.0f, 10.0f);

    HelicopterLight(color, lightDir, cutoff, intensity);


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
/*
void DrawModelTree(std::pair<int, int> *treePos, float *treeRotate, branch *tree, int tree_num){
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
// */
void DrawBillboardTree(std::pair<int, int> *treePos, int tree_num){
    for(int i = 0; i < tree_num; i++){
        DrawBillboard(treePos[i].first, 0, treePos[i].second, 100, 100, TREE_BILLBOARD, billboardName);
        // draw_billboard(5, 0, 5, 50, 50, TREE_BILLBOARD, billboardName);
    }
}

void DrawBillboardGrass(std::pair<int, int> *grassPos, int grass_num, billboard billboardType){
    for(int i = 0; i < grass_num; i++){
        DrawBillboard(grassPos[i].first, 0, grassPos[i].second, 10, 10, billboardType, billboardName);
    }
}

void DrawBillboardFlower(std::pair<int, int> *flowerPos, int flower_num, billboard billboardType){
    for(int i = 0; i < flower_num; i++){
        DrawBillboard(flowerPos[i].first, 0, flowerPos[i].second, 10, 10, billboardType, billboardName);
    }
}


void DrawBuilding(std::pair<int, int> *buildingPos, float *buildingRotate, Model building, int building_num){
    for(int i = 0; i < building_num; i++){
        SetTexture(CEMENT_TEXTURE, textName);
        // SetMaterial(CEMENT, 0.1, 0.1, 0.1);
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

void DrawParLight(int parLightID, glm::vec3 loc, float rotate, glm::vec3 color, Model par_light, bool test = 0){
    glPushMatrix();
    glTranslatef(loc.x, loc.y, loc.z);
    // glColor3f(color.x, color.y, color.z);
    //rotate light
    glRotatef(rotate, 0.0f, 1.0f, 0.0f);
    SetMaterial(METAL, 0.1, 0.1, 0.1);
    if(test) par_light.draw();
    float r = color.x;
    float g = color.y;
    float b = color.z;
    GLfloat lightPosition[] = { 0, 0, 0, 1.0 };  // Light position (x, y, z, w)
    // GLfloat lightPosition[] = { loc.x - 7.5, loc.y + 7.5, loc.z, 1.0 };  // Light position (x, y, z, w)
    GLfloat lightAmbient[] = { 0.2 * r, 0.2 * g, 0.2 * b, 1.0 };     // Ambient light color (RGBA)
    GLfloat lightDiffuse[] = { r, g, b, 1.0 };     // Diffuse light color (RGBA)
    GLfloat lightSpecular[] = { r, g, b, 1.0 };    // Specular light color (RGBA)
    glPushMatrix();
    glTranslatef(-7.5, 7.5, 0);
    SetMaterial(METAL, 1, 1, 1);
    Cube(1, 1, 1);
    glPopMatrix();
    float cutoff = 30.0;
    float exponent = 5.0;
    glLightfv(parLightID, GL_POSITION, lightPosition);
    glLightfv(parLightID, GL_AMBIENT, lightAmbient);
    glLightfv(parLightID, GL_DIFFUSE, lightDiffuse);
    glLightfv(parLightID, GL_SPECULAR, lightSpecular);

    GLfloat spot_direction[] = { -0.5, 0.5, 0.0 };
    glLightfv(parLightID, GL_SPOT_DIRECTION, spot_direction);
    glLightf(parLightID, GL_SPOT_CUTOFF, cutoff);
    glLightf(parLightID, GL_SPOT_EXPONENT, exponent);
    glLightf(parLightID, GL_CONSTANT_ATTENUATION, 1.5);
    glPopMatrix();
}

void DrawFixableLight(glm::vec3 loc, glm::vec3 dir, glm::vec3 color, float intensity, float cutoff = 45.0){
    glPushMatrix();
    glTranslatef(loc.x, loc.y, loc.z);
    float r = color.x * intensity;
    float g = color.y * intensity;
    float b = color.z * intensity;
    r = std::min(r, 1.0f);
    g = std::min(g, 1.0f);
    b = std::min(b, 1.0f);
    r = std::max(r, 0.0f);
    g = std::max(g, 0.0f);
    b = std::max(b, 0.0f);
    GLfloat lightPosition[] = { 0, 0, 0, 1.0 };  // Light position (x, y, z, w)
    GLfloat lightAmbient[] = { 0.2 * intensity * r, 0.2 * intensity * g, 0.2 * intensity * b, 1.0 };     // Ambient light color (RGBA)
    GLfloat lightDiffuse[] = { r, g, b, 1.0 };     // Diffuse light color (RGBA)
    GLfloat lightSpecular[] = { r, g, b, 1.0 };    // Specular light color (RGBA)
    glLightfv(FIXABLE_LIGHT, GL_POSITION, lightPosition);
    glLightfv(FIXABLE_LIGHT, GL_AMBIENT, lightAmbient);
    glLightfv(FIXABLE_LIGHT, GL_DIFFUSE, lightDiffuse);
    glLightfv(FIXABLE_LIGHT, GL_SPECULAR, lightSpecular);
    Cube(1, 1, 1);

    GLfloat spot_direction[] = { dir.x, dir.y, dir.z };
    glLightfv(FIXABLE_LIGHT, GL_SPOT_DIRECTION, spot_direction);
    glLightf(FIXABLE_LIGHT, GL_SPOT_CUTOFF, cutoff);
    glLightf(FIXABLE_LIGHT, GL_SPOT_EXPONENT, 5.0);

    glPopMatrix();
}

void DrawCandle(glm::vec3 loc, float instance){
    // std::cout << instance << std::endl;
    float r = (247.0 / 255) * instance;
    float g = (91.0 / 255) * instance;
    float b = (17.0 / 255) * instance;

    glPushMatrix();
    glScalef(5, 5, 5);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(loc.x, loc.y, loc.z - 1.5);
    glPushMatrix();
    glScalef(1, 1, 2);
    glTranslatef(0, 0, 0);
    SetMaterial(WAX, 1, 1, 1);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        1.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);
    gluDisk(cylind, 0.0, 0.5, 12, 3);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.25, 0.25, 0.5);
    glTranslatef(0, 0, -1);
    SetMaterial(CANDEL, r, g, b);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        1.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0, -0.25);
    glScalef(0.25, 0.25, 0.5);
    SetMaterial(CANDEL, r, g, b);
    gluDisk(cylind, 0.0, 0.5, 12, 3);
    glPopMatrix();
    glPopMatrix();
    // cnadle light
    glPushMatrix();
    glTranslatef(loc.x * 5, loc.z * 5 + 10, loc.y * 5);
    // glPushMatrix();
    // glScalef(1, 10, 1);
    // Cube(1, 1, 1);
    // glPopMatrix();

    GLfloat lightPosition[] = { 0, 0, 0, 1.0 };  // Light position (x, y, z, w)
    GLfloat lightAmbient[] = { 0.1 * r, 0.1 * g, 0.1 * b, 1.0 };     // Ambient light color (RGBA)
    GLfloat lightDiffuse[] = { r / 4, g / 4, b / 4, 1.0 };     // Diffuse light color (RGBA)
    GLfloat lightSpecular[] = { r / 4, g / 4, b / 4, 1.0 };    // Specular light color (RGBA)
    GLfloat lightDirection[] = { 0, 1, 0 };
    glLightfv(CANDLE_LIGHT, GL_POSITION, lightPosition);
    glLightfv(CANDLE_LIGHT, GL_AMBIENT, lightAmbient);
    glLightfv(CANDLE_LIGHT, GL_DIFFUSE, lightDiffuse);
    glLightfv(CANDLE_LIGHT, GL_SPECULAR, lightSpecular);
    glLightfv(CANDLE_LIGHT, GL_SPOT_DIRECTION, lightDirection);
    glLightf(CANDLE_LIGHT, GL_SPOT_CUTOFF, 180.0);
    glLightf(CANDLE_LIGHT, GL_SPOT_EXPONENT, 10.0);
    glLightf(CANDLE_LIGHT, GL_CONSTANT_ATTENUATION, 2.5);
    glPopMatrix();

}



// /*
void DrawViewVolume(glm::vec3 helicopterPos, glm::vec3 lookAtPos, float fov, float nearClip, float farClip, float aspectRatio, float upDegree){
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
    // glm::vec3 helicopterPos(helicopterX, helicopterY, helicopterZ);
    // glm::vec3 lookAtPos(lookAtX, lookAtY, lookAtZ);
    glm::vec3 viewDir = glm::normalize(lookAtPos - helicopterPos);
    glm::vec3 nearClipCenter = lookAtPos - nearClip * viewDir,
        farClipCenter = lookAtPos - farClip * viewDir;

    glm::vec3 nearCilpTopRight, nearClipTopLeft, nearClipBottomRight, nearClipBottomLeft;
    glm::vec3 farCilpTopRight, farClipTopLeft, farClipBottomRight, farClipBottomLeft;
    glm::vec3 lr(viewDir.z, 0, -viewDir.x);
    glm::vec3 ud = glm::cross(lr, viewDir);
    ud = ud / glm::normalize(ud);
    lr = lr / glm::normalize(lr);
    glm::vec3 rud = RotateUp(viewDir, ud, upDegree);
    glm::vec3 rlr = RotateUp(viewDir, lr, upDegree);
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
    glVertex3f(nearClipTopLeft.x, nearClipTopLeft.y, nearClipTopLeft.z);
    glVertex3f(nearCilpTopRight.x, nearCilpTopRight.y, nearCilpTopRight.z);
    glVertex3f(nearClipBottomRight.x, nearClipBottomRight.y, nearClipBottomRight.z);
    glVertex3f(nearClipBottomLeft.x, nearClipBottomLeft.y, nearClipBottomLeft.z);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(nearClipTopLeft.x, nearClipTopLeft.y, nearClipTopLeft.z);
    glVertex3f(farClipTopLeft.x, farClipTopLeft.y, farClipTopLeft.z);
    glVertex3f(nearCilpTopRight.x, nearCilpTopRight.y, nearCilpTopRight.z);
    glVertex3f(farCilpTopRight.x, farCilpTopRight.y, farCilpTopRight.z);
    glVertex3f(nearClipBottomRight.x, nearClipBottomRight.y, nearClipBottomRight.z);
    glVertex3f(farClipBottomRight.x, farClipBottomRight.y, farClipBottomRight.z);
    glVertex3f(nearClipBottomLeft.x, nearClipBottomLeft.y, nearClipBottomLeft.z);
    glVertex3f(farClipBottomLeft.x, farClipBottomLeft.y, farClipBottomLeft.z);
    glEnd();
}

void DrawView(glm::vec3 helicopterLocation, float u[3][3]){
    int    i;
    float  x = helicopterLocation.x + 10, y = helicopterLocation.y, z = helicopterLocation.z;
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

void DrawFog(glm::vec3 color){
    // float fog_color[] = { 0.15, 0.20, 0.20, 0.50 };
    color /= 4;
    float fog_color[4] = { color.x, color.y, color.z, 1.0 };
    glFogi(GL_FOG_MODE, GL_LINEAR);  /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
    glFogf(GL_FOG_DENSITY, 0.15);    /*fog opacity(density)= 0.25*/
    glFogf(GL_FOG_START, 1.0);       /*Setup two ends for GL_LINEAR*/
    glFogf(GL_FOG_END, 500.0);
    glFogfv(GL_FOG_COLOR, fog_color);/*set the fog color */
}

void DrawFloor(int len){
    int i, j;
    for(i = 0; i < len; i++)
        for(j = 0; j < len; j++){
            // if((i + j) % 2 == 0){
            //     // glColor3f(0.7, 0.7, 0.7);
            //     // SetMaterial(FLOOR, 0.7, 0.7, 0.7);
            //     SetTexture(WOOD2_TEXTURE, textName);
            // }
            // else{
            //     // glColor3f(0.1, 0.1, 0.7);
            //     // SetMaterial(FLOOR, 0.1, 0.1, 0.7);
            //     SetTexture(WOOD_TEXTURE, textName);
            // }
            SetTexture(EARTH_TEXTURE, textName);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0, 0.0);
            glVertex3f((i - 5.0) * 10.0, 0, (j - 5.0) * 10.0);
            glTexCoord2f(1.0, 0.0);
            glVertex3f((i - 5.0) * 10.0, 0, (j - 4.0) * 10.0);
            glTexCoord2f(1.0, 1.0);
            glVertex3f((i - 4.0) * 10.0, 0, (j - 4.0) * 10.0);
            glTexCoord2f(0.0, 1.0);
            glVertex3f((i - 4.0) * 10.0, 0, (j - 5.0) * 10.0);
            glEnd();

        }
}

void DrawSkyBox(glm::vec3 loc){
    float x = loc.x;
    float y = loc.y;
    float z = loc.z;

    glPushMatrix();
    glTranslatef(x - 375, y - 200, z - 375);
    SkyBox();
    glPopMatrix();
}

// */


#endif