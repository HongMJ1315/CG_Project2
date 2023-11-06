#include "main.h"


int width = 600;
int height = 600;
float lookAtX = 0, lookAtY = 10, lookAtZ = -10;
float helicopterX = 0, helicopterY = -5.0, helicopterZ = -50.0;
bool keyboardStates[256];
Model helicopterBody;
Model helicopterBackTire;
Model helicopterBackSupport;
Model helicopterLeftTire;
Model helicopterRightTire;

void init(){


    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // gluPerspective(20.0, 1.0, 1.0, 2000.0);
    glEnable(GL_DEPTH_TEST);

    helicopterBackSupport.load("Helicopter_Back_Support.obj");
    helicopterBackTire.load("Helicopter_Back_Tire.obj");
    helicopterBody.load("Helicopter_Body.obj");
    helicopterLeftTire.load("Helicopter_Left_Tire.obj");
    helicopterRightTire.load("Helicopter_Right_Tire.obj");

}


void reshap(int w, int h){

    width = w;
    height = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(w > h)
        glOrtho(-40.0, 40.0, -40.0 * (float) h / (float) w, 40.0 * (float) h / (float) w,
        -100.0, 100.0);
    else
        glOrtho(-40.0 * (float) w / (float) h, 40.0 * (float) w / (float) h, -40.0, 40.0,
        -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(lookAtX, lookAtY, lookAtZ, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);
    glTranslatef(helicopterX, helicopterY, helicopterZ);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(10.0f, 10.0f, 10.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    helicopterBody.draw();
    glColor3f(0.68, 0.68, 0.68);    /* Gray colored */
    helicopterBackSupport.draw();
    glColor3f(0.0f, 0.0f, 0.0f);
    helicopterBackTire.draw();
    helicopterLeftTire.draw();
    helicopterRightTire.draw();
    glutSwapBuffers();

}

void update(){

}

void keyboardDown(unsigned char key, int x, int y){

}

void keyboardUp(unsigned char key, int x, int y){

}

void main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    int POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    int POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Load Model");
    init();
    glutIdleFunc(update);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutReshapeFunc(reshap);
    glutDisplayFunc(display);
    glutMainLoop();
}