#include "main.h"

int width = 600;
int height = 600;
float helicopterX = 0, helicopterY = 20.0, helicopterZ = 10.0;
float lookAtX = helicopterX - 10, lookAtY = helicopterY + 20, lookAtZ = helicopterZ + 20;
bool keyboardStates[256];
bool directionKey[4];
Model helicopterBody;
Model helicopterBackTire;
Model helicopterBackSupport;
Model helicopterLeftTire;
Model helicopterRightTire;

float self_ang = 45.0;
void init(){
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    // glEnable(GL_LIGHT0);
    // glEnable(GL_LIGHTING);


    // 加载模型
    helicopterBackSupport.load("../../model/Helicopter_Back_Support.obj");
    helicopterBackTire.load("../../model/Helicopter_Back_Tire.obj");
    helicopterBody.load("../../model/Helicopter_Body.obj");
    helicopterLeftTire.load("../../model/Helicopter_Left_Tire.obj");
    helicopterRightTire.load("../../model/Helicopter_Right_Tire.obj");


    if(sphere == NULL){
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_FILL);
        gluQuadricNormals(sphere, GLU_SMOOTH);
    }
    if(cylind == NULL){
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }
    if(disk == NULL){
        disk = gluNewQuadric();
        gluQuadricDrawStyle(disk, GLU_FILL);
        gluQuadricNormals(disk, GLU_SMOOTH);
    }
}

void reshap(int w, int h){
    width = w;
    height = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(w > h)
        glOrtho(-40.0, 40.0, -40.0 * (float) h / (float) w, 40.0 * (float) h / (float) w, -100.0, 100.0);
    else
        glOrtho(-40.0 * (float) w / (float) h, 40.0 * (float) w / (float) h, -40.0, 40.0, -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    // 设置视图和相机位置
    gluLookAt(lookAtX, lookAtY, lookAtZ, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);

    draw_floor();
    draw_axes();

    // glPushMatrix();
    glTranslatef(helicopterX, helicopterY, helicopterZ);
    // gluCylinder(cylind, 10.0, 10.0, 4.0, 12, 3);

    // glPopMatrix();
    glPushMatrix();
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(10.0f, 10.0f, 10.0f);


    glColor3f(0.10f, 0.44f, 0.24f);
    helicopterBody.draw();

    glColor3f(0.67f, 0.70f, 0.73f);
    helicopterBackSupport.draw();

    glColor3f(0.0f, 0.0f, 0.0f);
    helicopterBackTire.draw();
    helicopterLeftTire.draw();
    helicopterRightTire.draw();

    glPopMatrix();


    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslated(2.5, 10, -11.7);
    glRotated(self_ang, 0.0, 1.0, 0.0);
    for(int i = 0; i < 4; i++){
        glPushMatrix();
        glTranslated(1, 0, -38);
        glScalef(1, 1, 5);
        glRotated(90.0, 1.0, 0.0, 0.0);
        Blade();

        glPopMatrix();
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);  // 旋转到下一个 Blade 位置
    }
    glPopMatrix();
    glPushMatrix();
    glTranslated(0.5, 4.8, 50.8);
    glRotated(self_ang, 1.0, 0.0, 0.0);
    for(int i = 0; i < 4; i++){
        glPushMatrix();
        glTranslated(0, -8, 0.5);
        glScalef(1, 1, 0.5);
        glRotated(90.0, 0, 1.0, 0.0);
        Blade();

        glPopMatrix();
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);  // 旋转到下一个 Blade 位置
    }
    glPopMatrix();

    glutSwapBuffers();
}

void update(){
    self_ang += 0.5;
    if(self_ang >= 360.0)
        self_ang -= 360.0;
    if(keyboardStates['w']){
        std::cout << "w" << std::endl;
        helicopterZ -= 0.1;
        lookAtZ -= 0.1;
    }
    if(keyboardStates['s']){
        std::cout << "s" << std::endl;
        helicopterZ += 0.1;
        lookAtZ += 0.1;
    }
    if(keyboardStates['a']){
        std::cout << "a" << std::endl;
        helicopterX -= 0.1;
        lookAtX -= 0.1;
    }
    if(keyboardStates['d']){
        std::cout << "d" << std::endl;
        helicopterX += 0.1;
        lookAtX += 0.1;
    }
    if(directionKey[0]){
        std::cout << "up" << std::endl;
        helicopterY += 0.1;
        lookAtY += 0.1;
    }
    if(directionKey[1]){
        std::cout << "down" << std::endl;
        helicopterY -= 0.1;
        lookAtY -= 0.1;
    }
    display();
}

void keyboardDown(unsigned char key, int x, int y){
    keyboardStates[key] = true;
}

void keyboardUp(unsigned char key, int x, int y){
    keyboardStates[key] = false;
}
void specialDown(int key, int x, int y){
    switch(key){
        case GLUT_KEY_UP:
        directionKey[0] = true;
        break;
        case GLUT_KEY_DOWN:
        directionKey[1] = true;
        break;
        case GLUT_KEY_LEFT:
        directionKey[2] = true;
        break;
        case GLUT_KEY_RIGHT:
        directionKey[3] = true;
        break;
        default:
        break;
    }
}

void specialUp(int key, int x, int y){
    switch(key){
        case GLUT_KEY_UP:
        directionKey[0] = false;
        break;
        case GLUT_KEY_DOWN:
        directionKey[1] = false;
        break;
        case GLUT_KEY_LEFT:
        directionKey[2] = false;
        break;
        case GLUT_KEY_RIGHT:
        directionKey[3] = false;
        break;
        default:
        break;
    }
}
int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    int POS_X = (glutGet(GLUT_SCREEN_WIDTH) - width) >> 1;
    int POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - height) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(width, height);
    glutCreateWindow("Load Model");
    init();
    glutIdleFunc(update);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);
    glutReshapeFunc(reshap);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
