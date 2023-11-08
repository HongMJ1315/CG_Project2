#include "main.h"


#define MAP_LENGTH 50
#define BUILDING_NUM 20
#define TREE_NUM 3
#define MAGANIFICATION 2.0f
#define ESP ((float)1e-7)
#define MOVE_SPEED 0.75f
#define ROTATE_SPEED 1.5f
#define BLADE_SPEED 5.0f
int width = 600;
int height = 600;
float helicopterRotateX = 0.0, helicopterRotateY = 0.0, helicopterRotateZ = 0.0;
float helicopterX = 0, helicopterY = 20.0, helicopterZ = 10.0;
float lookAtX = helicopterX - 10, lookAtY = helicopterY + 20, lookAtZ = helicopterZ + 20;
bool keyboardStates[256];
bool directionKey[4];
Model helicopterBody;
Model helicopterBackTire;
Model helicopterBackSupport;
Model helicopterLeftTire;
Model helicopterRightTire;
Model building;
float self_ang = 45.0;
std::pair<int, int> buildingPos[BUILDING_NUM];
float buildingRotate[BUILDING_NUM];
std::pair<int, int> treePos[TREE_NUM];
float treeRotate[TREE_NUM];
branch tree[TREE_NUM];

float bladeRotateSpeed = 0.5f;
float mapWidth = (MAP_LENGTH - 4.0f) * 10.0f, mapHeight = (MAP_LENGTH - 4.0f) * 10.0f;

const float sq2 = sqrt(2.0) / 2.0;
int viewPoint = 3;
void init(){
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    // glEnable(GL_LIGHT0);
    // glEnable(GL_LIGHTING);


    // helicopterBackSupport.load("../../model/Helicopter_Back_Support.obj");
    // helicopterBackTire.load("../../model/Helicopter_Back_Tire.obj");
    // helicopterBody.load("../../model/Helicopter_Body.obj");
    // helicopterLeftTire.load("../../model/Helicopter_Left_Tire.obj");
    // helicopterRightTire.load("../../model/Helicopter_Right_Tire.obj");
    building.load("../../model/Building02.obj");
    // building[1].load("../../model/Building02.obj");

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
    for(int i = 0; i < BUILDING_NUM; i++){

        int tx = rand() % (int) mapWidth;
        int tz = rand() % (int) mapHeight;
        std::cout << "x: " << tx << " y: " << 0.0 << " z: " << tz << std::endl;
        buildingPos[i] = std::make_pair(tx, tz);
        buildingRotate[i] = (float) (rand() % 360);
    }
    for(int i = 0; i < TREE_NUM; i++){
        int tx = rand() % (int) mapWidth;
        int tz = rand() % (int) mapHeight;
        std::cout << "x: " << tx << " y: " << 0.0 << " z: " << tz << std::endl;
        treePos[i] = std::make_pair(tx, tz);
        treeRotate[i] = (float) (rand() % 360);
        tree[i].bud(110.0f / 255.0, 44.0f / 255.0, 0);
        tree[i].grow(200);
        tree[i].bud(110.0f / 255.0, 44.0f / 255.0, 0);
        tree[i].grow(500);
        tree[i].bud(110.0f / 255.0, 44.0f / 255.0, 0);
        tree[i].grow(500);
        tree[i].bud(20.0f / 255.0, 90.0f / 50.0, 0);
        tree[i].grow(50);


    }
}


void draw_floor(){
    int i, j;

    for(i = 0; i < MAP_LENGTH; i++)
        for(j = 0; j < MAP_LENGTH; j++){
            if((i + j) % 2 == 0){
                glColor3f(1.0, 0.8, 0.8);
            }
            else{
                glColor3f(0.1, 0.1, 0.7);
            }
            glBegin(GL_POLYGON);
            glVertex3f((i - 5.0) * 10.0, -2.5, (j - 5.0) * 10.0);
            glVertex3f((i - 5.0) * 10.0, -2.5, (j - 4.0) * 10.0);
            glVertex3f((i - 4.0) * 10.0, -2.5, (j - 4.0) * 10.0);
            glVertex3f((i - 4.0) * 10.0, -2.5, (j - 5.0) * 10.0);
            glEnd();


        }
}

void reshap(int w, int h){
    width = w;
    height = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-w / 10, w / 10, -h / 10, h / 10, -1000, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void draw_building(){
    for(int i = 0; i < BUILDING_NUM; i++){
        glColor3f(0.5, 0.5, 0.5);
        glPushMatrix();
        int tx = buildingPos[i].first;
        int tz = buildingPos[i].second;
        glTranslatef(tx, 0.0, tz);
        glRotatef(buildingRotate[i], 0.0f, 1.0f, 0.0f);
        glScaled(0.01, 0.01, 0.01);
        // gluCylinder(cylind, 0.5, 0.5,
        //     10.0,
        //     12,
        //     3);
        building.draw();
        glPopMatrix();

    }
}

void draw_tree(){
    for(int i = 0; i < TREE_NUM; i++){
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

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(viewPoint == 3)
        gluLookAt(lookAtX, lookAtY, lookAtZ, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);
    else if(viewPoint == 2)
        gluLookAt(helicopterX, helicopterY, helicopterZ - 10.0f, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);
    else if(viewPoint == 1)
        gluLookAt(helicopterX + 0.01f, helicopterY + 10.0f, helicopterZ, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);
    else
        gluLookAt(helicopterX + 10.0f, helicopterY, helicopterZ, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);
    draw_floor();
    draw_axes();
    draw_tree();
    draw_building();

    glTranslatef(helicopterX, helicopterY, helicopterZ + 12);
    glRotatef(-helicopterRotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(-helicopterRotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(-helicopterRotateZ, 0.0f, 0.0f, 1.0f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    // glPushMatrix();

    // glScalef(10.0f, 10.0f, 10.0f);

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

    glutSwapBuffers();
}

void update(){
    if(helicopterY > ESP)
        self_ang += bladeRotateSpeed;
    std::cout << helicopterY << std::endl;
    if(self_ang >= 360.0)
        self_ang -= 360.0;
    if(keyboardStates['w']){
        if(helicopterRotateX < 45.0 && helicopterY > ESP){
            helicopterRotateX += ROTATE_SPEED * 0.2;
        }

        helicopterZ -= MOVE_SPEED * cos(helicopterRotateY * PI / 180.0);
        lookAtZ -= MOVE_SPEED * cos(helicopterRotateY * PI / 180.0);
        helicopterX += MOVE_SPEED * sin(helicopterRotateY * PI / 180.0);
        lookAtX += MOVE_SPEED * sin(helicopterRotateY * PI / 180.0);
    }
    if(keyboardStates['s']){
        if(helicopterRotateX > -45.0 && helicopterY > ESP){
            helicopterRotateX -= ROTATE_SPEED * 0.2;
        }
        helicopterZ += MOVE_SPEED * cos(helicopterRotateY * PI / 180.0);
        lookAtZ += MOVE_SPEED * cos(helicopterRotateY * PI / 180.0);
        helicopterX -= MOVE_SPEED * sin(helicopterRotateY * PI / 180.0);
        lookAtX -= MOVE_SPEED * sin(helicopterRotateY * PI / 180.0);
    }
    if(keyboardStates['a']){
        if(helicopterY > ESP){
            if(helicopterRotateZ > -45.0){
                helicopterRotateZ -= ROTATE_SPEED * 0.2;
            }
            helicopterX -= MOVE_SPEED * sin((helicopterRotateY + 90) * PI / 180.0);
            lookAtX -= MOVE_SPEED * sin((helicopterRotateY + 90) * PI / 180.0);
            helicopterZ += MOVE_SPEED * cos((helicopterRotateY + 90) * PI / 180.0);
            lookAtZ += MOVE_SPEED * cos((helicopterRotateY + 90) * PI / 180.0);
        }
    }
    if(keyboardStates['d']){
        if(helicopterY > ESP){
            if(helicopterRotateZ < 45.0){
                helicopterRotateZ += ROTATE_SPEED * 0.2;
            }
            helicopterX += MOVE_SPEED * sin((helicopterRotateY + 90) * PI / 180.0);
            lookAtX += MOVE_SPEED * sin((helicopterRotateY + 90) * PI / 180.0);
            helicopterZ -= MOVE_SPEED * cos((helicopterRotateY + 90) * PI / 180.0);
            lookAtZ -= MOVE_SPEED * cos((helicopterRotateY + 90) * PI / 180.0);
        }
    }
    if(directionKey[0]){
        bladeRotateSpeed = BLADE_SPEED * 2;
        helicopterY += MOVE_SPEED;
        lookAtY += MOVE_SPEED;
    }
    if(directionKey[1]){
        if(helicopterY > ESP){
            bladeRotateSpeed = BLADE_SPEED * 0.5;
            helicopterY -= MOVE_SPEED;
            lookAtY -= MOVE_SPEED;
        }
        else{
            bladeRotateSpeed = BLADE_SPEED;
        }
    }
    if(directionKey[2]){
        helicopterRotateY += ROTATE_SPEED * 0.5;
    }
    if(directionKey[3]){
        helicopterRotateY -= ROTATE_SPEED * 0.5;
    }
    if(!directionKey[0] && !directionKey[1] && !keyboardStates['w'] && !keyboardStates['s'] && !keyboardStates['a'] && !keyboardStates['d']){
        if(helicopterRotateX - ROTATE_SPEED > ESP){
            helicopterRotateX -= ROTATE_SPEED;
        }
        else if(helicopterRotateX + ROTATE_SPEED < ESP){
            helicopterRotateX += ROTATE_SPEED;
        }

        if(helicopterRotateZ - ROTATE_SPEED > ESP){
            helicopterRotateZ -= ROTATE_SPEED;
        }
        else if(helicopterRotateZ + ROTATE_SPEED < ESP){
            helicopterRotateZ += ROTATE_SPEED;
        }
        // helicopterRotateX = 0.0;
        // helicopterRotateZ = 0.0;
        bladeRotateSpeed = BLADE_SPEED;
    }
    if(keyboardStates['r']){
        if(keyboardStates['l']){
            keyboardStates['d'] = directionKey[3] = 1;
        }
        else{
            keyboardStates['d'] = directionKey[3] = 0;
        }
        if(keyboardStates['k']){
            keyboardStates['a'] = directionKey[2] = 1;
        }
        else{
            keyboardStates['a'] = directionKey[2] = 0;
        }
    }

    display();
}

void keyboardDown(unsigned char key, int x, int y){
    keyboardStates[key] = true;
    switch(key){
        case 'v':
        viewPoint = (viewPoint + 1) % 4;
        break;

        default:
        break;
    }
    // std::cout << "viewPoint: " << viewPoint << std::endl;

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
    glutCreateWindow("Helicopter");
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
