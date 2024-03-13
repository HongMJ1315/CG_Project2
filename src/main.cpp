// intensity 

#include "main.h"

#define MAP_LENGTH 100
#define BUILDING_NUM 20
#define TREE_NUM 3
#define BILLBOARD_TREE_NUM 5
#define BILLBOARD_GRASS_NUM 5
#define BILLBOARD_FLOWER_NUM 5
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

// helicopter
float helicopterX = 0, helicopterY = 20.0, helicopterZ = 100.0;
float lookAtX = helicopterX - 0, lookAtY = helicopterY + 100, lookAtZ = helicopterZ + 100;
float upX = 0.0, upY = 1.0, upZ = 0.0;
// float lookAtX = helicopterX - 50, lookAtY = helicopterY + 100, lookAtZ = helicopterZ + 100;
float helicopterRotateX = 0.0, helicopterRotateY = 180, helicopterRotateZ = 0.0;
float helicopterLightDirectionX = 0.0, helicopterLightDirectionY = -1, helicopterLightDirectionZ = 0;
float helicopterLightInstance = 1.0;
float helicopterLightCutoff = 45.0f;
int helicopterLightColorIndex = 0;
float upDegree = 90.0;
float self_ang = 0.0;
float bladeRotateSpeed = 0.5f;

// light
float fixableLightLocationX = -20, fixableLightLocationY = 0, fixableLightLocationZ = 10;
float fixableLightDirectionX = 0, fixableLightDirectionY = 1, fixableLightDirectionZ = 0;
float fixableLightInstance = 1.0;
float fixableLightCutoff = 30.0f;
int fixableLightColorIndex = 0;
bool fixableLightStatus = 1;

// candle
int cnadleLightIndex = 0;
bool candleStatus = 1;
float candleLightInstance[1000];

// sun
int sunLightColorIndex = 6;
float sunLightInstance = 1.0f;

// par light
int parLightArr[3] = { PAR_LIGHT0, PAR_LIGHT1, PAR_LIGHT2 };
int parLightColorIndex[3] = { 0, 2, 4 };
bool parLightStatus[3] = { 0, 0, 0 };
bool parLightEnable = 1;
bool parLightTest = 1;
int parLightIndex = 0;

// keyboard
bool keyboardStates[256];
bool directionKey[4];
bool shiftKey = false;

// display
float orthographicScale = 10;
float mapWidth = (MAP_LENGTH - 4.0f) * 10.0f, mapHeight = (MAP_LENGTH - 4.0f) * 10.0f;
float   u[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };
int viewPoint = 3;
int viewMode = 0;

// mirror
unsigned char mirrorTexture[MIRROR_TEXTURE_SIZE][MIRROR_TEXTURE_SIZE][4];
float mirrorHeight = MIRROR_TEXTURE_SIZE, mirrorWidth = MIRROR_TEXTURE_SIZE;
float mirrorX = 0, mirrorZ = 0;

// Model
Model helicopterBody;
Model helicopterBackTire;
Model helicopterBackSupport;
Model helicopterLeftTire;
Model helicopterRightTire;
Model building;
Model parLight;
std::pair<int, int> buildingPos[BUILDING_NUM];
float buildingRotate[BUILDING_NUM];
std::pair<int, int> modelTreePos[TREE_NUM];
float modelTreeRotate[TREE_NUM];
// branch modelTree[TREE_NUM];

// Texture
unsigned char wood[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned char metal[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned char wood2[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned char cement[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned char earth[TEXTURE_SIZE][TEXTURE_SIZE][4];
unsigned char sky[TEXTURE_SIZE][TEXTURE_SIZE][4];

//Billboard Pos
std::pair<int, int> billboardTreePos[BILLBOARD_TREE_NUM];
std::pair<int, int> billboardGrass1Pos[BILLBOARD_GRASS_NUM];
std::pair<int, int> billboardGrass2Pos[BILLBOARD_GRASS_NUM];
std::pair<int, int> billboardFlower1Pos[BILLBOARD_FLOWER_NUM];
std::pair<int, int> billboardFlower2Pos[BILLBOARD_FLOWER_NUM];

//Billboard
unsigned char tree[BILLBOARD_SIZE][BILLBOARD_SIZE][4];
unsigned char grass1[BILLBOARD_SIZE][BILLBOARD_SIZE][4];
unsigned char grass2[BILLBOARD_SIZE][BILLBOARD_SIZE][4];
unsigned char flower1[BILLBOARD_SIZE][BILLBOARD_SIZE][4];
unsigned char flower2[BILLBOARD_SIZE][BILLBOARD_SIZE][4];
unsigned char bird1[9][BILLBOARD_SIZE][BILLBOARD_SIZE][4];
//Bird Fly
int birdFlyIndex = 0;
glm::vec3 birdFlyPos = { -20, 50, 100 };

//Fog
int fogColorIndex = 6;
bool showFog = 1;


bool demoMirror = 0;

glm::vec3 lightColor[] = {
    {1.0,0.0,0.0},
    {1.0,1.0,0.0},
    {0.0,1.0,0.0},
    {0.0,1.0,1.0},
    {0.0,0.0,1.0},
    {1.0,0.0,1.0},
    {1.0,1.0,1.0}
};
const float sq2 = sqrt(2.0) / 2.0;

unsigned char tmp[BILLBOARD_SIZE][BILLBOARD_SIZE][4];

void init(){
    // setGLSLshaders("Phong.vert", "Phong.frag");
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    glEnable(GL_LIGHTING);
    // glEnable(GL_BLEND);
    // glEnable(GL_COLOR_MATERIAL);
    // Initialize lighting
    glEnable(SUN_LIGHT);
    glEnable(HELICOPTER_LIGHT1);
    glEnable(HELICOPTER_LIGHT2);
    glEnable(FIXABLE_LIGHT);
    glEnable(GL_FOG);
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
    /*
    for(int i = 0; i < TREE_NUM; i++){
        int tx = rand() % (int) mapWidth;
        int tz = rand() % (int) mapHeight;
        // std::cout << "x: " << tx << " y: " << 0.0 << " z: " << tz << std::endl;
        modelTreePos[i] = std::make_pair(tx, tz);
        modelTreeRotate[i] = (float) (rand() % 360);
        modelTree[i].bud(110.0f / 255.0, 44.0f / 255.0, 0);
        modelTree[i].grow(200);
        modelTree[i].bud(110.0f / 255.0, 44.0f / 255.0, 0);
        modelTree[i].grow(500);
        modelTree[i].bud(110.0f / 255.0, 44.0f / 255.0, 0);
        modelTree[i].grow(500);
        modelTree[i].bud(20.0f / 255.0, 90.0f / 50.0, 0);
        modelTree[i].grow(50);
    }
    // */
    for(int i = 0; i < BILLBOARD_TREE_NUM; i++){
        int tx = rand() % (int) mapWidth;
        int tz = rand() % (int) mapHeight;
        // std::cout << "x: " << tx << " y: " << 0.0 << " z: " << tz << std::endl;
        billboardTreePos[i] = std::make_pair(tx, tz);
    }
    for(int i = 0; i < BILLBOARD_GRASS_NUM; i++){
        int tx = rand() % (int) mapWidth;
        int tz = rand() % (int) mapHeight;
        // std::cout << "x: " << tx << " y: " << 0.0 << " z: " << tz << std::endl;
        billboardGrass1Pos[i] = std::make_pair(tx, tz);
    }
    for(int i = 0; i < BILLBOARD_GRASS_NUM; i++){
        int tx = rand() % (int) mapWidth;
        int tz = rand() % (int) mapHeight;
        // std::cout << "x: " << tx << " y: " << 0.0 << " z: " << tz << std::endl;
        billboardGrass2Pos[i] = std::make_pair(tx, tz);
    }
    for(int i = 0; i < BILLBOARD_FLOWER_NUM; i++){
        int tx = rand() % (int) mapWidth;
        int tz = rand() % (int) mapHeight;
        // std::cout << "x: " << tx << " y: " << 0.0 << " z: " << tz << std::endl;
        billboardFlower1Pos[i] = std::make_pair(tx, tz);
    }
    for(int i = 0; i < BILLBOARD_FLOWER_NUM; i++){
        int tx = rand() % (int) mapWidth;
        int tz = rand() % (int) mapHeight;
        // std::cout << "x: " << tx << " y: " << 0.0 << " z: " << tz << std::endl;
        billboardFlower2Pos[i] = std::make_pair(tx, tz);
    }
    glm::vec3 tld = RotateUp(glm::vec3(helicopterLightDirectionX, helicopterLightDirectionY, helicopterLightDirectionZ),
        glm::vec3(1, 0, 0), 90.0f);
    helicopterLightDirectionX = tld.x; helicopterLightDirectionY = tld.y; helicopterLightDirectionZ = tld.z;
    perlin p;
    p.perlin_noise(candleLightInstance, 1000, time(NULL));
    glPixelStorei(GL_UNPACK_ALIGNMENT, 10);
    glGenTextures(10, textName);
    glGenTextures(BILLBOARD_NUM, billboardName);

    ReadTexture(wood2, "../../texture/wood2.jpg", 256, 256);
    glBindTexture(GL_TEXTURE_2D, textName[texture::WOOD2_TEXTURE]);
    TextureInit(WOOD2_TEXTURE, textName, wood2, 256, 256);

    ReadTexture(cement, "../../texture/cement.jpg", 256, 256);
    glBindTexture(GL_TEXTURE_2D, textName[texture::CEMENT_TEXTURE]);
    TextureInit(CEMENT_TEXTURE, textName, cement, 256, 256);

    ReadTexture(wood, "../../texture/wood.jpg", 256, 256);
    glBindTexture(GL_TEXTURE_2D, textName[texture::WOOD_TEXTURE]);
    TextureInit(WOOD_TEXTURE, textName, wood, 256, 256);

    ReadTexture(metal, "../../texture/metal.jpg", 256, 256);
    glBindTexture(GL_TEXTURE_2D, textName[texture::METAL_TEXTURE]);
    TextureInit(METAL_TEXTURE, textName, metal, 256, 256);

    ReadTexture(earth, "../../texture/earth.jpg", 256, 256);
    glBindTexture(GL_TEXTURE_2D, textName[texture::EARTH_TEXTURE]);
    TextureInit(EARTH_TEXTURE, textName, earth, 256, 256);

    ReadBillboard(tree, "../../billboard/tree.png", 512, 512);
    glBindTexture(GL_TEXTURE_2D, billboardName[billboard::TREE_BILLBOARD]);
    BillboardInit(TREE_BILLBOARD, billboardName, tree, 512, 512);

    ReadBillboard(grass1, "../../billboard/grass1.png", 512, 512);
    glBindTexture(GL_TEXTURE_2D, billboardName[billboard::GRASS1_BILLBOARD]);
    BillboardInit(GRASS1_BILLBOARD, billboardName, grass1, 512, 512);

    ReadBillboard(grass2, "../../billboard/grass2.png", 512, 512);
    glBindTexture(GL_TEXTURE_2D, billboardName[billboard::GRASS2_BILLBOARD]);
    BillboardInit(GRASS2_BILLBOARD, billboardName, grass2, 512, 512);

    ReadBillboard(flower1, "../../billboard/flower1.png", 512, 512);
    glBindTexture(GL_TEXTURE_2D, billboardName[billboard::FLOWER1_BILLBOARD]);
    BillboardInit(FLOWER1_BILLBOARD, billboardName, flower1, 512, 512);

    ReadBillboard(flower2, "../../billboard/flower2.png", 512, 512);
    glBindTexture(GL_TEXTURE_2D, billboardName[billboard::FLOWER2_BILLBOARD]);
    BillboardInit(FLOWER2_BILLBOARD, billboardName, flower2, 512, 512);

    ReadTexture(sky, "../../texture/sky.jpg", 256, 256);
    glBindTexture(GL_TEXTURE_2D, textName[texture::SKY_TEXTURE]);
    TextureInit(SKY_TEXTURE, textName, sky, 256, 256);

    ImageDivider imgDivider("../../billboard/bird1.png");
    std::vector<cv::Mat> dividedImages = imgDivider.divide(3, 3);
    for(int i = 0; i < 9; i++){
        cv::Mat tmpMat = dividedImages[i];
        for(int j = 0; j < tmpMat.rows; j++){
            for(int k = 0; k < tmpMat.cols; k++){
                cv::Vec4b color = tmpMat.at<cv::Vec4b>(cv::Point(k, j));
                bird1[i][j][k][0] = color[2];
                bird1[i][j][k][1] = color[1];
                bird1[i][j][k][2] = color[0];
                bird1[i][j][k][3] = color[3];
            }
        }
        for(int j = 0; j < tmpMat.rows / 2; j++){
            for(int k = 0; k < tmpMat.cols; k++){
                std::swap(bird1[i][j][k][0], bird1[i][tmpMat.rows - j - 1][k][0]);
                std::swap(bird1[i][j][k][1], bird1[i][tmpMat.rows - j - 1][k][1]);
                std::swap(bird1[i][j][k][2], bird1[i][tmpMat.rows - j - 1][k][2]);
                std::swap(bird1[i][j][k][3], bird1[i][tmpMat.rows - j - 1][k][3]);
            }
        }
        std::string filename = "img" + std::to_string(i) + ".png";
        cv::imwrite(filename, tmpMat);
        BillboardInit(billboard(BIRD1_BILLBOARD + i), billboardName, bird1[i], 512, 512);
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

void draw_scene(bool viewVolume, bool view = true, bool mirror = true){
    glPushMatrix();
    DrawCandle(glm::vec3(15, 15, 0), candleLightInstance[cnadleLightIndex]);
    DrawAxes();
    DrawSunLight(lightColor[sunLightColorIndex], sunLightInstance);
    DrawFixableLight(glm::vec3(fixableLightLocationX, fixableLightLocationY, fixableLightLocationZ),
        glm::vec3(fixableLightDirectionX, fixableLightDirectionY, fixableLightDirectionZ),
        lightColor[fixableLightColorIndex], fixableLightInstance, fixableLightCutoff);
    DrawParLight(PAR_LIGHT0, glm::vec3(150 - 14.14 * 2, 0, 150), 120, lightColor[parLightColorIndex[0]], parLight, parLightTest);
    DrawParLight(PAR_LIGHT1, glm::vec3(150 + 14.14 * 2, 0, 150), 0, lightColor[parLightColorIndex[1]], parLight, parLightTest);
    DrawParLight(PAR_LIGHT2, glm::vec3(150, 0, 150 + 20 * 2), 240, lightColor[parLightColorIndex[2]], parLight, parLightTest);
    DrawHelicopter(glm::vec3(helicopterX, helicopterY, helicopterZ),
        glm::vec3(helicopterRotateX, helicopterRotateY, helicopterRotateZ),
        glm::vec3(helicopterLightDirectionX, helicopterLightDirectionY, helicopterLightDirectionZ),
        lightColor[helicopterLightColorIndex], self_ang, helicopterLightCutoff, helicopterLightInstance);
    SetMaterial(WOOD);
    // DrawModelTree(modelTreePos, modelTreeRotate, modelTree, TREE_NUM);
    DrawBillboardTree(billboardTreePos, BILLBOARD_TREE_NUM);
    DrawBillboardGrass(billboardGrass1Pos, BILLBOARD_GRASS_NUM, GRASS1_BILLBOARD);
    DrawBillboardGrass(billboardGrass2Pos, BILLBOARD_GRASS_NUM, GRASS2_BILLBOARD);
    DrawBillboardFlower(billboardGrass1Pos, BILLBOARD_FLOWER_NUM, FLOWER1_BILLBOARD);
    DrawBillboardFlower(billboardFlower1Pos, BILLBOARD_FLOWER_NUM, FLOWER1_BILLBOARD);
    DrawBillboardFlower(billboardFlower2Pos, BILLBOARD_FLOWER_NUM, FLOWER2_BILLBOARD);
    DrawBuilding(buildingPos, buildingRotate, building, BUILDING_NUM);
    if(view)DrawFloor(MAP_LENGTH);
    if(viewVolume) DrawViewVolume(glm::vec3(helicopterX, helicopterY, helicopterZ), glm::vec3(lookAtX, lookAtY, lookAtZ), CLIP_DEGREE, NEAR_CLIP, FAR_CLIP, float(width) / float(height), upDegree);
    if(!viewVolume) DrawSkyBox(glm::vec3(helicopterX, helicopterY, helicopterZ));
    if(mirror) DrawFog(lightColor[fogColorIndex]);
    DrawBird(birdFlyPos, birdFlyIndex, BIRD1_BILLBOARD);
    // if(mirror) DrawMirror(glm::vec3(mirrorX, 0, mirrorZ), mirrorWidth, mirrorHeight, texture::MIRROR_TEXTURE);
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
    DrawView(glm::vec3(helicopterX, helicopterY, helicopterZ), u);
    glPopMatrix();

    glPushMatrix();
    // Top Right Viewport
    glViewport(viewportWidth, height / 2, viewportWidth, viewportHeight);
    view_direction(2);
    draw_scene(1, (helicopterY) > ESP);
    glPopMatrix();
    glPushMatrix();
    view_direction(2);
    DrawView(glm::vec3(helicopterX, helicopterY, helicopterZ), u);
    glPopMatrix();


    glPushMatrix();
    // Bottom Left Viewport
    glViewport(0, 0, viewportWidth, viewportHeight);
    view_direction(1);
    draw_scene(1);
    glPopMatrix();
    glPushMatrix();
    view_direction(1);
    DrawView(glm::vec3(helicopterX, helicopterY, helicopterZ), u);
    glPopMatrix();

    glPushMatrix();
    // Bottom Right Viewport
    glViewport(viewportWidth, 0, viewportWidth, viewportHeight);
    view_direction(3);
    draw_scene(1);
    glPopMatrix();
    glPushMatrix();
    view_direction(3);
    DrawView(glm::vec3(helicopterX, helicopterY, helicopterZ), u);
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
    DrawView(glm::vec3(helicopterX, helicopterY, helicopterZ), u);

}
/*
void GetFrameBuffer(unsigned char buffer[MIRROR_TEXTURE_SIZE][MIRROR_TEXTURE_SIZE][4], glm::vec3 cameraPos, glm::vec3 cameraUp, float mirrorWidth, float mirrorHeight){
    // std::cout << comeraPos.x << " " << comeraPos.y << " " << comeraPos.z << std::endl;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // 以cameraPos为视点，cameraUp为上方向，mirrorWidth和mirrorHeight为视口大小，相機位置为cameraPos，看向鏡子中心
    gluPerspective(CLIP_DEGREE, MIRROR_TEXTURE_SIZE / MIRROR_TEXTURE_SIZE, MIRROR_TEXTURE_SIZE, MIRROR_TEXTURE_SIZE * (FAR_CLIP / NEAR_CLIP));
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, mirrorX, mirrorHeight / 2, mirrorZ, cameraUp.x, cameraUp.y, cameraUp.z);
    draw_scene(0, (lookAtY > ESP), 0);
    glReadPixels(0, 0, MIRROR_TEXTURE_SIZE, MIRROR_TEXTURE_SIZE, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    // for(int i = 0; i < MIRROR_TEXTURE_SIZE / 2; i++){
    //     for(int j = 0; j < MIRROR_TEXTURE_SIZE; j++){
    //         std::swap(buffer[i][j][0], buffer[MIRROR_TEXTURE_SIZE - i - 1][j][0]);
    //         std::swap(buffer[i][j][1], buffer[MIRROR_TEXTURE_SIZE - i - 1][j][1]);
    //         std::swap(buffer[i][j][2], buffer[MIRROR_TEXTURE_SIZE - i - 1][j][2]);
    //         std::swap(buffer[i][j][3], buffer[MIRROR_TEXTURE_SIZE - i - 1][j][3]);
    //     }
    // }
    for(int i = 0; i < MIRROR_TEXTURE_SIZE; i++){
        for(int j = 0; j < MIRROR_TEXTURE_SIZE / 2; j++){
            std::swap(buffer[i][j][0], buffer[i][MIRROR_TEXTURE_SIZE - j - 1][0]);
            std::swap(buffer[i][j][1], buffer[i][MIRROR_TEXTURE_SIZE - j - 1][1]);
            std::swap(buffer[i][j][2], buffer[i][MIRROR_TEXTURE_SIZE - j - 1][2]);
            std::swap(buffer[i][j][3], buffer[i][MIRROR_TEXTURE_SIZE - j - 1][3]);
        }
    }
    glBindTexture(GL_TEXTURE_2D, textName[texture::MIRROR_TEXTURE]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT = repeat texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MIRROR_TEXTURE_SIZE, MIRROR_TEXTURE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}\



void MirrorFunc(){
    // glm::vec3 newCamera = glm::vec3(lookAtX, lookAtY, lookAtZ - (lookAtZ - mirrorZ) * 2.0f);
    glm::vec3 newCamera = glm::vec3(-lookAtX, lookAtY, -lookAtZ);

    glm::vec3 newCameraUp = glm::vec3(0, 1, 0);
    GetFrameBuffer(mirrorTexture, newCamera, newCameraUp, mirrorWidth, mirrorHeight);
}
*/
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
    ComputeABAxes();
    switch(viewMode){
        case 0:
        singleview_projection();
        break;
        case 1:
        multiview_projection();
        break;
    }
    glutSwapBuffers();
    // MirrorFunc();
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
    if(!keyboardStates['z'] && !keyboardStates['x'] && !keyboardStates['c']){
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
            glm::vec3 res = MoveCameraUD(glm::vec3(helicopterX, helicopterY, helicopterZ), glm::vec3(lookAtX, lookAtY, lookAtZ), 1.0f);
            lookAtX = res.x; lookAtY = res.y; lookAtZ = res.z;
        }
        if(keyboardStates['h']){
            glm::vec3 res = MoveCameraUD(glm::vec3(helicopterX, helicopterY, helicopterZ), glm::vec3(lookAtX, lookAtY, lookAtZ), -1.0f);
            lookAtX = res.x; lookAtY = res.y; lookAtZ = res.z;
        }
        if(keyboardStates['g']){
            glm::vec3 res = MoveCameraLR(glm::vec3(helicopterX, helicopterY, helicopterZ), glm::vec3(lookAtX, lookAtY, lookAtZ), 1.0f);
            lookAtX = res.x; lookAtY = res.y; lookAtZ = res.z;
        }
        if(keyboardStates['j']){
            glm::vec3 res = MoveCameraLR(glm::vec3(helicopterX, helicopterY, helicopterZ), glm::vec3(lookAtX, lookAtY, lookAtZ), -1.0f);
            lookAtX = res.x; lookAtY = res.y; lookAtZ = res.z;
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
            if(helicopterY - 3 > ESP){
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
            glm::vec3 tld = RotateUp(glm::vec3(1, 0, 0),
                glm::vec3(helicopterLightDirectionX, helicopterLightDirectionY, helicopterLightDirectionZ), -1.0f);
            helicopterLightDirectionX = tld.x; helicopterLightDirectionY = tld.y; helicopterLightDirectionZ = tld.z;
        }
        if(keyboardStates['h']){
            glm::vec3 tld = RotateUp(glm::vec3(1, 0, 0),
                glm::vec3(helicopterLightDirectionX, helicopterLightDirectionY, helicopterLightDirectionZ), 1.0f);
            helicopterLightDirectionX = tld.x; helicopterLightDirectionY = tld.y; helicopterLightDirectionZ = tld.z;
        }
        if(keyboardStates['g']){
            glm::vec3 tld = RotateUp(glm::vec3(0, 1, 0),
                glm::vec3(helicopterLightDirectionX, helicopterLightDirectionY, helicopterLightDirectionZ), 1.0f);
            helicopterLightDirectionX = tld.x; helicopterLightDirectionY = tld.y; helicopterLightDirectionZ = tld.z;
        }
        if(keyboardStates['j']){
            glm::vec3 tld = RotateUp(glm::vec3(0, 1, 0),
                glm::vec3(helicopterLightDirectionX, helicopterLightDirectionY, helicopterLightDirectionZ), -1.0f);
            helicopterLightDirectionX = tld.x; helicopterLightDirectionY = tld.y; helicopterLightDirectionZ = tld.z;
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
        helicopterLightInstance = std::max(0.0f, helicopterLightInstance);
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
            glm::vec3 res = RotateUp(glm::vec3(1, 0, 0),
                glm::vec3(fixableLightDirectionX, fixableLightDirectionY, fixableLightDirectionZ),
                1.0f);
            fixableLightDirectionX = res.x; fixableLightDirectionY = res.y; fixableLightDirectionZ = res.z;
        }
        if(keyboardStates['h']){
            glm::vec3 res = RotateUp(glm::vec3(1, 0, 0),
                glm::vec3(fixableLightDirectionX, fixableLightDirectionY, fixableLightDirectionZ),
                -1.0f);
            fixableLightDirectionX = res.x; fixableLightDirectionY = res.y; fixableLightDirectionZ = res.z;
        }
        if(keyboardStates['g']){
            glm::vec3 res = RotateUp(glm::vec3(0, 1, 0)
                , glm::vec3(fixableLightDirectionX, fixableLightDirectionY, fixableLightDirectionZ),
                1.0f);
            fixableLightDirectionX = res.x; fixableLightDirectionY = res.y; fixableLightDirectionZ = res.z;
        }
        if(keyboardStates['j']){
            glm::vec3 res = RotateUp(glm::vec3(0, 1, 0)
                , glm::vec3(fixableLightDirectionX, fixableLightDirectionY, fixableLightDirectionZ),
                -1.0f);
            fixableLightDirectionX = res.x; fixableLightDirectionY = res.y; fixableLightDirectionZ = res.z;
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
        fixableLightInstance = std::max(fixableLightInstance, 0.0f);
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
    else if(keyboardStates['c']){
        if(keyboardStates['-']) sunLightInstance -= 0.1f;
        else if(keyboardStates['=']) sunLightInstance += 0.1f;
    }

    //fixed up vector  
    glm::vec3 tup = UpVector(glm::vec3(helicopterX, helicopterY, helicopterZ), glm::vec3(lookAtX, lookAtY, lookAtZ), upDegree);
    upX = tup.x; upY = tup.y; upZ = tup.z;
    display();
}

void keyboardDown(unsigned char key, int x, int y){
    // std::cout << char(key) << std::endl;
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
            glm::vec3 tld = RotateUp(glm::vec3(helicopterLightDirectionX, helicopterLightDirectionY, helicopterLightDirectionZ),
                glm::vec3(1, 0, 0), 90.0f);
            helicopterLightDirectionX = tld.x; helicopterLightDirectionY = tld.y; helicopterLightDirectionZ = tld.z;
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
        parLightEnable = !parLightEnable;
        break;

        case '[':{
            if(candleStatus){
                candleStatus = false;
                glDisable(CANDLE_LIGHT);
            }
            else{
                candleStatus = true;
                glEnable(CANDLE_LIGHT);
            }
            break;
        }

        case ']':{
            if(fixableLightStatus){
                fixableLightStatus = false;
                glDisable(FIXABLE_LIGHT);
            }
            else{
                fixableLightStatus = true;
                glEnable(FIXABLE_LIGHT);
            }
            break;
        }

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
        case '4':{
            fogColorIndex = (fogColorIndex + 1) % 7;
            break;
        }
        case '5':{
            showFog = !showFog;
            if(showFog){
                glEnable(GL_FOG);
            }
            else{
                glDisable(GL_FOG);
            }
            break;
        }
        case '8':{
            helicopterZ = 200;
            lookAtZ = helicopterZ - 100;
            glutReshapeWindow(600, 600);

            break;
        }
        case '9':{
            //強制更新螢幕視窗大小成MIRROR_TEXTURE_SIZE
            helicopterX = 0;
            lookAtX = 0;
            helicopterZ = 100;
            lookAtZ = helicopterZ + 100;
            glutReshapeWindow(MIRROR_TEXTURE_SIZE, MIRROR_TEXTURE_SIZE);
        }
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

void CandleLightShinee(int v){
    cnadleLightIndex = (cnadleLightIndex + 1) % 1000;
    glutTimerFunc(50, CandleLightShinee, 0);
}

void ParLightTimeFunc(int value){
    if(!parLightEnable){
        glDisable(PAR_LIGHT0);
        glDisable(PAR_LIGHT1);
        glDisable(PAR_LIGHT2);
        parLightStatus[0] = parLightStatus[1] = parLightStatus[2] = 0;

    }
    else{
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
    }
    glutTimerFunc(2000, ParLightTimeFunc, 0);
}

void BirdFlyAnimation(int value){
    birdFlyIndex = (birdFlyIndex + 1) % 9;
    birdFlyPos.x += 1.5f;
    if(birdFlyPos.x > 500.0f){
        birdFlyPos.x = -100.0f;
    }
    glutTimerFunc(50, BirdFlyAnimation, 0);
}
int main(int argc, char **argv){
    glutInit(&argc, argv);
    // glfwInit();
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
    glutTimerFunc(2000, ParLightTimeFunc, 0);
    glutTimerFunc(100, CandleLightShinee, 0);
    glutTimerFunc(100, BirdFlyAnimation, 0);


    glutMainLoop();
    return 0;
}
