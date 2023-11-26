#include "main.h"


#define MAP_LENGTH 50
#define BUILDING_NUM 20
#define TREE_NUM 3
#define MAGANIFICATION 2.0f
#define ESP ((float)1e-7)
#define MOVE_SPEED 0.75f
#define ROTATE_SPEED 1.5f
#define BLADE_SPEED 5.0f
#define ESP 1e-6
int width = 600;
int height = 600;
float helicopterRotateX = 0.0, helicopterRotateY = 0.0, helicopterRotateZ = 0.0;
float helicopterX = 0, helicopterY = 20.0, helicopterZ = 10.0;
float lookAtX = helicopterX - 0, lookAtY = helicopterY + 100, lookAtZ = helicopterZ + 100;
// float lookAtX = helicopterX - 50, lookAtY = helicopterY + 100, lookAtZ = helicopterZ + 100;
float upX = 0.0, upY = 1.0, upZ = 0.0;
float upDegree = 90.0;
bool keyboardStates[256];
bool directionKey[4];
bool shiftKey = false;
float axisDis = 100.0f;
Model helicopterBody;
Model helicopterBackTire;
Model helicopterBackSupport;
Model helicopterLeftTire;
Model helicopterRightTire;
Model building;
float self_ang = 0.0;
std::pair<int, int> buildingPos[BUILDING_NUM];
float buildingRotate[BUILDING_NUM];
std::pair<int, int> treePos[TREE_NUM];
float treeRotate[TREE_NUM];
branch tree[TREE_NUM];
float   u[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };

float bladeRotateSpeed = 0.5f;
float mapWidth = (MAP_LENGTH - 4.0f) * 10.0f, mapHeight = (MAP_LENGTH - 4.0f) * 10.0f;

const float sq2 = sqrt(2.0) / 2.0;
int viewPoint = 3;
int viewMode = 0;


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
        // std::cout << "x: " << tx << " y: " << 0.0 << " z: " << tz << std::endl;
        buildingPos[i] = std::make_pair(tx, tz);
        buildingRotate[i] = (float) (rand() % 360);
    }
    for(int i = 0; i < TREE_NUM; i++){
        int tx = rand() % (int) mapWidth;
        int tz = rand() % (int) mapHeight;
        // std::cout << "x: " << tx << " y: " << 0.0 << " z: " << tz << std::endl;
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

void reset_all(){
    for(int i = 0; i < 256; i++){
        keyboardStates[i] = false;
    }
    for(int i = 0; i < 4; i++){
        directionKey[i] = false;
    }
    helicopterRotateX = 0.0, helicopterRotateY = 0.0, helicopterRotateZ = 0.0;
    helicopterX = 0, helicopterY = 20.0, helicopterZ = 10.0;
    lookAtX = helicopterX - 0, lookAtY = helicopterY + 100, lookAtZ = helicopterZ + 100;
    upX = 0.0, upY = 1.0, upZ = 0.0;
    upDegree = 90.0;
    self_ang = 0.0;
    bladeRotateSpeed = 0.5f;
    viewPoint = 3;


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

    // glOrtho(-w / 10, w / 10, -h / 10, h / 10, -1000, 1000);
    gluPerspective(60.0, (float) w / (float) h, 1.0, 1000.0);
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

void draw_helicopter(){
    glPushMatrix();
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

    glPopMatrix();
}

void draw_view(){
    int    i;

    glMatrixMode(GL_MODELVIEW);
    float eye[3] = { helicopterX + 10, helicopterY, helicopterZ };
    /*----Draw Eye position-----*/
    glPushMatrix();
    glTranslatef(eye[0], eye[1], eye[2]);
    glColor3f(0.0, 1.0, 0.0);
    glutWireSphere(1.0, 10, 10);
    glPopMatrix();

    /*----Draw eye coord. axes -----*/
    glColor3f(1.0, 0.0, 0.0);           /* Draw Xe */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[0][0], eye[1] + 20.0 * u[0][1], eye[2] + 20.0 * u[0][2]);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);          /* Draw Ye */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[1][0], eye[1] + 20.0 * u[1][1], eye[2] + 20.0 * u[1][2]);
    glEnd();

    glColor3f(0.0, 0.0, 1.0);          /* Draw Ze */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[2][0], eye[1] + 20.0 * u[2][1], eye[2] + 20.0 * u[2][2]);
    glEnd();
}

void draw_scene(bool view = true){
    glPushMatrix();
    if(view)draw_floor();
    draw_axes();
    draw_tree();
    draw_building();
    draw_helicopter();
    glPopMatrix();
}
/*
void draw_view_pyramid(){
    float fov = 60.0;  // 視野角度
    float nearClip = 1.0;  // 近裁剪面
    float farClip = 1000.0;  // 遠裁剪面
    float aspectRatio = (float) width / (float) height;  // 視圖的擴張比例

    // 計算近裁剪面的一半高度和寬度
    float nearHeight = tan(fov / 2.0 * (PI / 180.0)) * nearClip;
    float nearWidth = nearHeight * aspectRatio;

    // 計算近裁剪面的四個角的坐標
    float nearTopLeft[3] = { -nearWidth, nearHeight, -nearClip };
    float nearTopRight[3] = { nearWidth, nearHeight, -nearClip };
    float nearBottomLeft[3] = { -nearWidth, -nearHeight, -nearClip };
    float nearBottomRight[3] = { nearWidth, -nearHeight, -nearClip };


    float cameraPos[3] = { helicopterX, helicopterY, helicopterZ };
    float lookAtPos[3] = { lookAtX, lookAtY, lookAtZ };
}
// */
void view_direction(int x){
    switch(x){
        case 0:
        gluLookAt(lookAtX, lookAtY, lookAtZ, helicopterX, helicopterY, helicopterZ, upX, upY, upZ);

        break;
        case 1:
        gluLookAt(helicopterX, helicopterY, helicopterZ + axisDis, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);

        break;
        case 2:
        gluLookAt(helicopterX + 0.01f, helicopterY + axisDis, helicopterZ, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);

        break;
        case 3:
        gluLookAt(helicopterX + axisDis, helicopterY, helicopterZ, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);

        break;
    }
}

void multiview_projection(){
    int viewportWidth = width / 2;
    int viewportHeight = height / 2;

    glPushMatrix();
    // Top Left Viewport
    glViewport(0, height / 2, viewportWidth, viewportHeight);
    view_direction(0);
    draw_scene((lookAtY) > ESP);
    glPopMatrix();
    glPushMatrix();
    view_direction(0);
    draw_view();
    glPopMatrix();

    glPushMatrix();
    // Top Right Viewport
    glViewport(viewportWidth, height / 2, viewportWidth, viewportHeight);
    view_direction(2);
    draw_scene((helicopterY + axisDis) > ESP);
    glPopMatrix();
    glPushMatrix();
    view_direction(2);
    draw_view();
    // draw_view_pyramid();
    glPopMatrix();


    glPushMatrix();
    // Bottom Left Viewport
    glViewport(0, 0, viewportWidth, viewportHeight);
    view_direction(1);
    draw_scene();
    glPopMatrix();
    glPushMatrix();
    view_direction(1);
    draw_view();
    // draw_view_pyramid();
    glPopMatrix();

    glPushMatrix();
    // Bottom Right Viewport
    glViewport(viewportWidth, 0, viewportWidth, viewportHeight);
    view_direction(3);
    draw_scene();
    glPopMatrix();
    glPushMatrix();
    view_direction(3);
    draw_view();
    // draw_view_pyramid();
    glPopMatrix();
}

void singleview_projection(){
    glLoadIdentity();
    if(viewPoint == 3)
        gluLookAt(lookAtX, lookAtY, lookAtZ, helicopterX, helicopterY, helicopterZ, upX, upY, upZ);
    else if(viewPoint == 2)
        gluLookAt(helicopterX, helicopterY, helicopterZ + axisDis, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);
    else if(viewPoint == 1)
        gluLookAt(helicopterX + 0.01f, helicopterY + axisDis, helicopterZ, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);
    else
        gluLookAt(helicopterX + axisDis, helicopterY, helicopterZ, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);
    if(viewPoint == 3) draw_scene((lookAtY > ESP));
    else if(viewPoint == 1) draw_scene((helicopterY + axisDis) > ESP);
    else draw_scene();
    draw_view();

}
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    switch(viewMode){
        case 0:
        singleview_projection();
        break;
        case 1:
        multiview_projection();
        break;
    }


    glutSwapBuffers();
}

void move_camera_ud(float degree){
    Eigen::Vector3f O(helicopterX, helicopterY, helicopterZ);
    Eigen::Vector3f P(lookAtX, lookAtY, lookAtZ);
    Eigen::Vector3f result = rotate_matrix(O, P, degree);
    Eigen::Transpose<Eigen::Vector3f> loc = result.transpose();

    Eigen::Vector3f tresult = result - O;
    Eigen::Vector3f tv = Eigen::Vector3f(0, 1, 0);
    float cos_theta = tresult.dot(tv) / (tresult.norm() * tv.norm());
    float angle = acos(cos_theta) * 180.0 / PI;

    if(angle < ESP || 180.0f - angle < ESP) return;
    lookAtX = loc.x(); lookAtY = loc.y(); lookAtZ = loc.z();
}

void zoom(float len){
    float dis = sqrt((lookAtX - helicopterX) * (lookAtX - helicopterX) + (lookAtY - helicopterY) * (lookAtY - helicopterY) + (lookAtZ - helicopterZ) * (lookAtZ - helicopterZ));
    float dx = (lookAtX - helicopterX) / dis;
    float dy = (lookAtY - helicopterY) / dis;
    float dz = (lookAtZ - helicopterZ) / dis;
    if(dis + len < 0.0001f)
        return;
    lookAtX = lookAtX + dx * len;
    lookAtY = lookAtY + dy * len;
    lookAtZ = lookAtZ + dz * len;
}

void move_camera_lr(float degree){
    //旋轉中心
    float center_x = helicopterX,
        center_z = helicopterZ;
    //移回中心
    float tox = lookAtX - center_x,
        toz = lookAtZ - center_z;

    degree = degree * PI / 180.0f;
    //旋轉
    float tx = tox * cos(degree) - toz * sin(degree),
        tz = tox * sin(degree) + toz * cos(degree);
    //移回
    lookAtX = tx + center_x;
    lookAtZ = tz + center_z;
}

void up_vector(){
    Eigen::Vector3f O(helicopterX, helicopterY, helicopterZ);
    Eigen::Vector3f P(lookAtX, lookAtY, lookAtZ);
    Eigen::Vector3f l = P - O;
    Eigen::Vector3f v(l.z(), 0, -l.x());
    l = l / l.norm();
    v = v / v.norm();
    Eigen::Vector3f result = rotate_up(l, v, upDegree);
    Eigen::Transpose<Eigen::Vector3f> loc = result.transpose();
    upX = loc.x(); upY = loc.y(); upZ = loc.z();
}

void axis_distance(float len){
    axisDis += len;

    std::cout << axisDis << " " << len << std::endl;
}
void update(){
    if(helicopterY > ESP)
        self_ang += bladeRotateSpeed;
    // std::cout << helicopterY << std::endl;
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
    else{
        if(helicopterRotateX - ROTATE_SPEED > ESP){
            helicopterRotateX -= ROTATE_SPEED;
        }
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
    else{
        if(helicopterRotateX + ROTATE_SPEED < ESP){
            helicopterRotateX += ROTATE_SPEED;
        }
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
    else{
        if(helicopterRotateZ + ROTATE_SPEED < ESP){
            helicopterRotateZ += ROTATE_SPEED;
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
    else{

        if(helicopterRotateZ - ROTATE_SPEED > ESP){
            helicopterRotateZ -= ROTATE_SPEED;
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
        bladeRotateSpeed = BLADE_SPEED;
    }
    if(keyboardStates['e']){
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
    //yghj
// /*
    if(keyboardStates['y']){
        move_camera_ud(1.0f);
        std::cout << "lookAt:(" << lookAtX << ", " << lookAtY << ", " << lookAtZ << ")" << std::endl;
    }
    if(keyboardStates['h']){
        move_camera_ud(-1.0f);
    }
    if(keyboardStates['g']){
        move_camera_lr(1.0f);
    }
    if(keyboardStates['j']){
        move_camera_lr(-1.0f);
    }
    //*/
    //nb
    if(keyboardStates['n']){
        zoom(1.0f);
    }
    if(keyboardStates['b']){
        zoom(-1.0f);
    }
    //tu
    if(keyboardStates['t']){
        upDegree += 1.0f;

    }
    if(keyboardStates['u']){
        upDegree -= 1.0f;

    }
    //fixed up vector  
    up_vector();

    if(keyboardStates['=']){
        axis_distance(5.0f);
    }
    if(keyboardStates['-']){
        axis_distance(-5.0f);
    }
    display();
}

void keyboardDown(unsigned char key, int x, int y){
    keyboardStates[key] = true;
    switch(key){
        case 'v':
        if(viewMode == 0)
            viewPoint = (viewPoint + 1) % 4;
        break;

        case 'm':
        viewMode = (viewMode + 1) % 2;
        break;
        case 'r':
        upDegree = 90.0f;
        lookAtX = helicopterX - 0, lookAtY = helicopterY + 100, lookAtZ = helicopterZ + 100;
        break;
        default:
        break;
    }

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

        case GLUT_KEY_SHIFT_L:
        reset_all();
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
