#include <GL/glut.h>

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 設定投影矩陣
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1.0, 100.0);  // 垂直視角、寬高比、近平面、遠平面

    // 設定模型視景矩陣
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0,  // 相機位置
        0.0, 0.0, 0.0,  // 觀察目標位置
        0.0, 1.0, 0.0);  // 上方向

    // 繪製透視視景體的邊框
    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_LINE_LOOP);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glEdgeFlag(GL_FALSE);
    glVertex3f(1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glEnd();
    glutSwapBuffers();
}

void reshape(int w, int h){
    glViewport(0, 0, w, h);
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Perspective Projection View Volume");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}
