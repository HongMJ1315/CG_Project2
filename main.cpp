// intensity 

#include "main.h"

#define MAP_LENGTH 50
#define BUILDING_NUM 20
#define TREE_NUM 3
#define MAGANIFICATION 2.0f
#define ESP ((float)1e-7)
#define MOVE_SPEED 0.75f
#define ROTATE_SPEED 1.5f
#define BLADE_SPEED 5.0f
#define NEAR_CLIP 1.0f
#define FAR_CLIP 1000.0f
#define CLIP_DEGREE 60.0f
int width = 600;
int height = 600;
float helicopterRotateX = 0.0, helicopterRotateY = 0.0, helicopterRotateZ = 0.0;
float helicopterX = 0, helicopterY = 20.0, helicopterZ = 10.0;
float lookAtX = helicopterX - 0, lookAtY = helicopterY + 100, lookAtZ = helicopterZ + 100;
float helicopterLightDirectionX = 0.0, helicopterLightDirectionY = -1, helicopterLightDirectionZ = 0;
float helicopterLightInstance = 1.0;
float fixableLightLocationX = -20, fixableLightLocationY = 0, fixableLightLocationZ = 10;
float fixableLightDirectionX = 0, fixableLightDirectionY = 1, fixableLightDirectionZ = 0;
float fixableLightInstance = 1.0;
// float lookAtX = helicopterX - 50, lookAtY = helicopterY + 100, lookAtZ = helicopterZ + 100;
int sunLightColorIndex = 6, helicopterLightColorIndex = 0, fixableLightColorIndex = 0;
float upX = 0.0, upY = 1.0, upZ = 0.0;
float fixableLightCutoff = 30.0f;
float helicopterLightCutoff = 45.0f;
float upDegree = 90.0;
bool keyboardStates[256];
bool directionKey[4];
bool shiftKey = false;
bool parLightTest = 1;
int parLightArr[3] = { PAR_LIGHT0, PAR_LIGHT1, PAR_LIGHT2 };
int parLightIndex = 0;
int parLightColorIndex[3] = { 0, 2, 4 };
bool parLightStatus[3] = { 0, 0, 0 };
float orthographicScale = 10;
Model helicopterBody;
Model helicopterBackTire;
Model helicopterBackSupport;
Model helicopterLeftTire;
Model helicopterRightTire;
Model building;
Model parLight;
float self_ang = 0.0;
std::pair<int, int> buildingPos[BUILDING_NUM];
float buildingRotate[BUILDING_NUM];
std::pair<int, int> treePos[TREE_NUM];
float treeRotate[TREE_NUM];
branch tree[TREE_NUM];
float   u[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };

Eigen::Vector3f lightColor[] = {
    {1.0,0.0,0.0},
    {1.0,1.0,0.0},
    {0.0,1.0,0.0},
    {0.0,1.0,1.0},
    {0.0,0.0,1.0},
    {1.0,0.0,1.0},
    {1.0,1.0,1.0}
};

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
    glEnable(GL_LIGHTING);
    // glEnable(GL_COLOR_MATERIAL);
    // Initialize lighting
    glEnable(SUN_LIGHT);
    glEnable(HELICOPTER_LIGHT1);
    glEnable(HELICOPTER_LIGHT2);
    glEnable(FIXABLE_LIGHT);
    glEnable(CANDLE_LIGHT);
    // glEnable(PAR_LIGHT0);
    // glEnable(PAR_LIGHT1);
    // glEnable(PAR_LIGHT2);

    // helicopterBackSupport.load("../../model/Helicopter_Back_Support.obj");
    // helicopterBackTire.load("../../model/Helicopter_Back_Tire.obj");
    // helicopterBody.load("../../model/Helicopter_Body.obj");
    // helicopterLeftTire.load("../../model/Helicopter_Left_Tire.obj");
    // helicopterRightTire.load("../../model/Helicopter_Right_Tire.obj");
    building.load("../../model/Building02.obj");
    parLight.load("../../model/ParLight.obj");
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
    Eigen::Vector3f tld = rotate_up(Eigen::Vector3f(helicopterLightDirectionX, helicopterLightDirectionY, helicopterLightDirectionZ),
        Eigen::Vector3f(1, 0, 0), 90.0f);
    helicopterLightDirectionX = tld.x(); helicopterLightDirectionY = tld.y(); helicopterLightDirectionZ = tld.z();

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

void draw_scene(bool viewVolume, bool view = true){
    glPushMatrix();
    if(view)draw_floor(MAP_LENGTH);
    if(viewVolume) draw_view_volume(Eigen::Vector3f(helicopterX, helicopterY, helicopterZ), Eigen::Vector3f(lookAtX, lookAtY, lookAtZ), CLIP_DEGREE, NEAR_CLIP, FAR_CLIP, float(width) / float(height), upDegree);
    draw_axes();
    draw_sun_light(lightColor[sunLightColorIndex]);
    SetMaterial(WOOD);
    draw_tree(treePos, treeRotate, tree, TREE_NUM);
    draw_building(buildingPos, buildingRotate, building, BUILDING_NUM);
    draw_helicopter(Eigen::Vector3f(helicopterX, helicopterY, helicopterZ),
        Eigen::Vector3f(helicopterRotateX, helicopterRotateY, helicopterRotateZ),
        Eigen::Vector3f(helicopterLightDirectionX, helicopterLightDirectionY, helicopterLightDirectionZ),
        lightColor[helicopterLightColorIndex], self_ang, helicopterLightCutoff, helicopterLightInstance);
    draw_par_light(PAR_LIGHT0, Eigen::Vector3f(150 - 14.14 * 2, 0, 150), 120, lightColor[parLightColorIndex[0]], parLight, parLightTest);
    draw_par_light(PAR_LIGHT1, Eigen::Vector3f(150 + 14.14 * 2, 0, 150), 0, lightColor[parLightColorIndex[1]], parLight, parLightTest);
    draw_par_light(PAR_LIGHT2, Eigen::Vector3f(150, 0, 150 + 20 * 2), 240, lightColor[parLightColorIndex[2]], parLight, parLightTest);
    draw_fixable_light(Eigen::Vector3f(fixableLightLocationX, fixableLightLocationY, fixableLightLocationZ),
        Eigen::Vector3f(fixableLightDirectionX, fixableLightDirectionY, fixableLightDirectionZ),
        lightColor[fixableLightColorIndex], fixableLightInstance, fixableLightCutoff);
    draw_candle(Eigen::Vector3f(15, 15, 0));
    glPopMatrix();
}

void view_direction(int x){
    switch(x){
        case 0:
        // glOrtho(-w / 10, w / 10, -h / 10, h / 10, -1000, 1000);
        gluPerspective(CLIP_DEGREE, (float) width / (float) height, NEAR_CLIP, FAR_CLIP);

        gluLookAt(lookAtX, lookAtY, lookAtZ, helicopterX, helicopterY, helicopterZ, upX, upY, upZ);

        break;
        case 1:
        glOrtho(-width / orthographicScale, width / orthographicScale, -height / orthographicScale, height / orthographicScale, -1000, 1000);
        // gluPerspective(CLIP_DEGREE, (float) w / (float) h, NEAR_CLIP, FAR_CLIP);

        gluLookAt(helicopterX, helicopterY, helicopterZ + 1.0f, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);

        break;
        case 2:
        glOrtho(-width / orthographicScale, width / orthographicScale, -height / orthographicScale, height / orthographicScale, -1000, 1000);
        // gluPerspective(CLIP_DEGREE, (float) w / (float) h, NEAR_CLIP, FAR_CLIP);

        gluLookAt(helicopterX + 0.01f, helicopterY + 1.0f, helicopterZ, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);

        break;
        case 3:

        glOrtho(-width / orthographicScale, width / orthographicScale, -height / orthographicScale, height / orthographicScale, -1000, 1000);
        // gluPerspective(CLIP_DEGREE, (float) w / (float) h, NEAR_CLIP, FAR_CLIP);
        gluLookAt(helicopterX + 1.0f, helicopterY, helicopterZ, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);

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
    draw_scene(0, (lookAtY) > ESP);
    glPopMatrix();
    glPushMatrix();
    view_direction(0);
    draw_view(Eigen::Vector3f(helicopterX, helicopterY, helicopterZ), u);
    glPopMatrix();

    glPushMatrix();
    // Top Right Viewport
    glViewport(viewportWidth, height / 2, viewportWidth, viewportHeight);
    view_direction(2);
    draw_scene(1, (helicopterY) > ESP);
    glPopMatrix();
    glPushMatrix();
    view_direction(2);
    draw_view(Eigen::Vector3f(helicopterX, helicopterY, helicopterZ), u);
    glPopMatrix();


    glPushMatrix();
    // Bottom Left Viewport
    glViewport(0, 0, viewportWidth, viewportHeight);
    view_direction(1);
    draw_scene(1);
    glPopMatrix();
    glPushMatrix();
    view_direction(1);
    draw_view(Eigen::Vector3f(helicopterX, helicopterY, helicopterZ), u);
    glPopMatrix();

    glPushMatrix();
    // Bottom Right Viewport
    glViewport(viewportWidth, 0, viewportWidth, viewportHeight);
    view_direction(3);
    draw_scene(1);
    glPopMatrix();
    glPushMatrix();
    view_direction(3);
    draw_view(Eigen::Vector3f(helicopterX, helicopterY, helicopterZ), u);
    glPopMatrix();
}

void singleview_projection(){
    glLoadIdentity();
    glViewport(0, 0, width, height);
    const float test = 15;
    if(viewPoint == 3){
        gluPerspective(CLIP_DEGREE, (float) width / (float) height, NEAR_CLIP, FAR_CLIP);
        gluLookAt(lookAtX, lookAtY, lookAtZ, helicopterX, helicopterY, helicopterZ, upX, upY, upZ);
    }
    else if(viewPoint == 2){
        glOrtho(-width / orthographicScale, width / orthographicScale, -height / orthographicScale, height / orthographicScale, -1000, 1000);
        gluLookAt(helicopterX, helicopterY, helicopterZ + 1.0f, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);
    }
    else if(viewPoint == 1){
        glOrtho(-width / orthographicScale, width / orthographicScale, -height / orthographicScale, height / orthographicScale, -1000, 1000);
        gluLookAt(helicopterX + 0.01f, helicopterY + 1.0f, helicopterZ, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);
    }
    else{
        glOrtho(-width / orthographicScale, width / orthographicScale, -height / orthographicScale, height / orthographicScale, -1000, 100);
        gluLookAt(helicopterX + 1.0f, helicopterY, helicopterZ, helicopterX, helicopterY, helicopterZ, 0.0f, 1.0f, 0.0f);
    }
    if(viewPoint == 3) draw_scene(0, (lookAtY > ESP));
    else draw_scene(1);
    draw_view(Eigen::Vector3f(helicopterX, helicopterY, helicopterZ), u);

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

void reshap(int w, int h){
    width = w;
    height = h;


    display();
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





void update(){
    if(helicopterY > ESP)
        self_ang += bladeRotateSpeed;
    // std::cout << helicopterY << std::endl;
    if(self_ang >= 360.0)
        self_ang -= 360.0;
    if(!keyboardStates['z'] && !keyboardStates['x']){
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

        //yghj
    // /*
        if(keyboardStates['y']){
            Eigen::Vector3f res = move_camera_ud(Eigen::Vector3f(helicopterX, helicopterY, helicopterZ), Eigen::Vector3f(lookAtX, lookAtY, lookAtZ), 1.0f);
            lookAtX = res.x(); lookAtY = res.y(); lookAtZ = res.z();
        }
        if(keyboardStates['h']){
            Eigen::Vector3f res = move_camera_ud(Eigen::Vector3f(helicopterX, helicopterY, helicopterZ), Eigen::Vector3f(lookAtX, lookAtY, lookAtZ), -1.0f);
            lookAtX = res.x(); lookAtY = res.y(); lookAtZ = res.z();
        }
        if(keyboardStates['g']){
            Eigen::Vector3f res = move_camera_lr(Eigen::Vector3f(helicopterX, helicopterY, helicopterZ), Eigen::Vector3f(lookAtX, lookAtY, lookAtZ), 1.0f);
            lookAtX = res.x(); lookAtY = res.y(); lookAtZ = res.z();
        }
        if(keyboardStates['j']){
            Eigen::Vector3f res = move_camera_lr(Eigen::Vector3f(helicopterX, helicopterY, helicopterZ), Eigen::Vector3f(lookAtX, lookAtY, lookAtZ), -1.0f);
            lookAtX = res.x(); lookAtY = res.y(); lookAtZ = res.z();
        }
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
        //*/
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
            helicopterRotateY -= ROTATE_SPEED * 0.5;
        }
        if(directionKey[3]){
            helicopterRotateY += ROTATE_SPEED * 0.5;
        }
        if(!directionKey[0] && !directionKey[1] && !keyboardStates['w'] && !keyboardStates['s'] && !keyboardStates['a'] && !keyboardStates['d']){
            bladeRotateSpeed = BLADE_SPEED;
        }
        if(keyboardStates['=']){
            orthographicScale += 1.0f;
        }
        if(keyboardStates['-']){
            orthographicScale -= 1.0f;
        }

    }
    if(keyboardStates['z']){
        if(keyboardStates['y']){
            Eigen::Vector3f tld = rotate_up(Eigen::Vector3f(1, 0, 0),
                Eigen::Vector3f(helicopterLightDirectionX, helicopterLightDirectionY, helicopterLightDirectionZ), -1.0f);
            helicopterLightDirectionX = tld.x(); helicopterLightDirectionY = tld.y(); helicopterLightDirectionZ = tld.z();
        }
        if(keyboardStates['h']){
            Eigen::Vector3f tld = rotate_up(Eigen::Vector3f(1, 0, 0),
                Eigen::Vector3f(helicopterLightDirectionX, helicopterLightDirectionY, helicopterLightDirectionZ), 1.0f);
            helicopterLightDirectionX = tld.x(); helicopterLightDirectionY = tld.y(); helicopterLightDirectionZ = tld.z();
        }
        if(keyboardStates['g']){
            Eigen::Vector3f tld = rotate_up(Eigen::Vector3f(0, 1, 0),
                Eigen::Vector3f(helicopterLightDirectionX, helicopterLightDirectionY, helicopterLightDirectionZ), 1.0f);
            helicopterLightDirectionX = tld.x(); helicopterLightDirectionY = tld.y(); helicopterLightDirectionZ = tld.z();
        }
        if(keyboardStates['j']){
            Eigen::Vector3f tld = rotate_up(Eigen::Vector3f(0, 1, 0),
                Eigen::Vector3f(helicopterLightDirectionX, helicopterLightDirectionY, helicopterLightDirectionZ), -1.0f);
            helicopterLightDirectionX = tld.x(); helicopterLightDirectionY = tld.y(); helicopterLightDirectionZ = tld.z();
        }
        if(keyboardStates['b']){
            helicopterLightCutoff += 1.0f;
        }
        if(keyboardStates['n']){
            helicopterLightCutoff -= 1.0f;
        }
        if(keyboardStates['=']){
            helicopterLightInstance += 0.1f;
        }
        if(keyboardStates['-']){
            helicopterLightInstance -= 0.1f;
        }
        helicopterLightCutoff = std::min(90.0f, helicopterLightCutoff);
        helicopterLightCutoff = std::max(0.0f, helicopterLightCutoff);
    }
    else if(keyboardStates['x']){
        if(keyboardStates['w']){
            fixableLightLocationZ -= MOVE_SPEED;
        }
        if(keyboardStates['s']){
            fixableLightLocationZ += MOVE_SPEED;

        }
        if(keyboardStates['a']){
            fixableLightLocationX -= MOVE_SPEED;

        }
        if(keyboardStates['d']){
            fixableLightLocationX += MOVE_SPEED;
        }
        if(directionKey[0]){
            fixableLightLocationY += MOVE_SPEED;
        }
        if(directionKey[1]){
            fixableLightLocationY -= MOVE_SPEED;
        }
        //yghj
        if(keyboardStates['y']){
            Eigen::Vector3f res = rotate_up(Eigen::Vector3f(1, 0, 0),
                Eigen::Vector3f(fixableLightDirectionX, fixableLightDirectionY, fixableLightDirectionZ),
                1.0f);
            fixableLightDirectionX = res.x(); fixableLightDirectionY = res.y(); fixableLightDirectionZ = res.z();
        }
        if(keyboardStates['h']){
            Eigen::Vector3f res = rotate_up(Eigen::Vector3f(1, 0, 0),
                Eigen::Vector3f(fixableLightDirectionX, fixableLightDirectionY, fixableLightDirectionZ),
                -1.0f);
            fixableLightDirectionX = res.x(); fixableLightDirectionY = res.y(); fixableLightDirectionZ = res.z();
        }
        if(keyboardStates['g']){
            Eigen::Vector3f res = rotate_up(Eigen::Vector3f(0, 1, 0)
                , Eigen::Vector3f(fixableLightDirectionX, fixableLightDirectionY, fixableLightDirectionZ),
                1.0f);
            fixableLightDirectionX = res.x(); fixableLightDirectionY = res.y(); fixableLightDirectionZ = res.z();
        }
        if(keyboardStates['j']){
            Eigen::Vector3f res = rotate_up(Eigen::Vector3f(0, 1, 0)
                , Eigen::Vector3f(fixableLightDirectionX, fixableLightDirectionY, fixableLightDirectionZ),
                -1.0f);
            fixableLightDirectionX = res.x(); fixableLightDirectionY = res.y(); fixableLightDirectionZ = res.z();
        }
        if(keyboardStates['b']){
            fixableLightCutoff += 1.0f;
        }
        if(keyboardStates['n']){
            fixableLightCutoff -= 1.0f;
        }
        if(keyboardStates['=']){
            fixableLightInstance += 0.1f;
        }
        if(keyboardStates['-']){
            fixableLightInstance -= 0.1f;
        }
        fixableLightCutoff = std::min(fixableLightCutoff, 90.0f);
        fixableLightCutoff = std::max(fixableLightCutoff, 0.0f);
    }
    else if(keyboardStates['e']){
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
    else{

    }

    //nb

    //fixed up vector  
    Eigen::Vector3f tup = up_vector(Eigen::Vector3f(helicopterX, helicopterY, helicopterZ), Eigen::Vector3f(lookAtX, lookAtY, lookAtZ), upDegree);
    upX = tup.x(); upY = tup.y(); upZ = tup.z();


    display();
}

void keyboardDown(unsigned char key, int x, int y){
    std::cout << char(key) << std::endl;
    keyboardStates[key] = true;
    switch(key){
        case 'v':
        if(viewMode == 0)
            viewPoint = (viewPoint + 1) % 4;
        break;

        case 'm':
        viewMode = (viewMode + 1) % 2;
        break;
        case 'r':{
            upDegree = 90.0f;
            lookAtX = helicopterX - 0, lookAtY = helicopterY + 100, lookAtZ = helicopterZ + 100;
            // helicopter lihgt reset
            helicopterLightDirectionX = 0.0, helicopterLightDirectionY = -1, helicopterLightDirectionZ = 0;
            Eigen::Vector3f tld = rotate_up(Eigen::Vector3f(helicopterLightDirectionX, helicopterLightDirectionY, helicopterLightDirectionZ),
                Eigen::Vector3f(1, 0, 0), 90.0f);
            helicopterLightDirectionX = tld.x(); helicopterLightDirectionY = tld.y(); helicopterLightDirectionZ = tld.z();
            break;
        }

        case 'i':{
            if(sunLightStatus){
                sunLightStatus = false;
                glDisable(GL_LIGHT0);
            }
            else{
                sunLightStatus = true;
                glEnable(GL_LIGHT0);
            }
            break;
        }

        case 'o':{
            if(helicopterLightStatus){
                helicopterLightStatus = false;
                glDisable(HELICOPTER_LIGHT1);
                glDisable(HELICOPTER_LIGHT2);
            }
            else{
                helicopterLightStatus = true;
                glEnable(HELICOPTER_LIGHT1);
                glEnable(HELICOPTER_LIGHT2);
            }
            break;
        }

        case 'p':
        parLightTest = !parLightTest;
        break;

        case '1':{
            sunLightColorIndex = (sunLightColorIndex + 1) % 7;
            break;
        }
        case '2':{
            helicopterLightColorIndex = (helicopterLightColorIndex + 1) % 7;
            break;
        }
        case '3':{
            fixableLightColorIndex = (fixableLightColorIndex + 1) % 7;
            break;
        }
                // case '4':{
                //     parLightColorIndex = (parLightColorIndex + 1) % 7;
                //     break;
                // }

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

        case GLUT_KEY_SHIFT_R:
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

void timeFunc(int value){
    // for(int i = 0; i < 3; i++){
    //     std::cout << parLightStatus[i] << " ";
    // }
    // std::cout << std::endl;
    // for(int i = 0; i < 3; i++){
    //     std::cout << parLightColorIndex[i] << " ";
    // }
    // std::cout << std::endl;
    if(parLightStatus[parLightIndex]){
        glDisable(parLightArr[parLightIndex]);
        parLightColorIndex[parLightIndex] = (parLightColorIndex[parLightIndex] + 1) % 6;
        parLightStatus[parLightIndex] = 0;
    }
    else{
        glEnable(parLightArr[parLightIndex]);
        parLightStatus[parLightIndex] = 1;
    }

    parLightIndex = (parLightIndex + 1) % 3;
    glutTimerFunc(2000, timeFunc, 0);
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
    glutTimerFunc(2000, timeFunc, 0);
    glutMainLoop();
    return 0;
}
