#ifndef MAIN_H
#define MAIN_H

#include "load_model.h"

#include <math.h>
#include <time.h>

#include <GL/freeglut.h>
#include <GL/glext.h>

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#define WIDTH 600
#define HEIGHT 600

float  points[][3] = { {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
                      {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
                      {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
                      {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5} };
/* face of box, each face composing of 4 vertices */
int    face[][4] = { {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5},
                    {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };
float  colors[6][3] = { {0.5,0.5,0.5}, {0.7,0.7,0.7}, {0.7,0.5,0.5},
                     {0.5,0.5,0.5}, {0.5,0.7,0.5}, {0.5,0.5,0.7} };

void Cube(){
    for(int i = 0; i < 6; i++){
        glBegin(GL_POLYGON);
        for(int j = 0; j < 4; j++){
            glColor3fv(colors[i]);
            glVertex3fv(points[face[i][j]]);
        }
        glEnd();
    }
}


#endif