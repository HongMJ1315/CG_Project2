#ifndef load_model_H
#define load_model_H

#include <math.h>
#include <time.h>

#include <GL/freeglut.h>
#include <GL/glext.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>


#define INTERVAL 15

class Model{
private:
    float r, g, b;
    static int count_char(std::string &str, char ch){
        int c = 0;
        int length = str.length() - 1;
        for(int i = 0; i < length; i++){
            if(str[i] == ch)
                c++;
        }
        return c;
    }

    static bool has_double_slash(std::string &str){
        int length = str.length() - 2;
        for(int i = 0; i < length; i++){
            if(str[i] == '/' && str[i + 1] == '/')
                return true;
        }
        return false;
    }


    class Face{
    public:
        int edge;
        int *vertices;
        int *texcoords;
        int normal;

        Face(int edge, int *vertices, int *texcoords, int normal = -1){
            this->edge = edge;
            this->vertices = vertices;
            this->texcoords = texcoords;
            this->normal = normal;
        }
    };

    std::string prefix;
    std::map<std::string, int> map_material;

    std::vector<float *> vertices;
    std::vector<float *> texcoords;
    std::vector<float *> normals;
    std::vector<Face> faces;

    GLuint list;

    void add_face_3v(std::string &line){
        int v0, v1, v2;
        sscanf(line.c_str(), "f %d %d %d", &v0, &v1, &v2);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        faces.push_back(Face(3, v, NULL));
    }

    void add_face_3vt(std::string &line){
        int v0, v1, v2, t0, t1, t2;
        sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &v0, &t0, &v1, &t1, &v2, &t2);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        int *t = new int[3]{t0 - 1, t1 - 1, t2 - 1};
        faces.push_back(Face(3, v, t));
    }

    void add_face_3vn(std::string &line){
        int v0, v1, v2, n;
        sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v0, &n, &v1, &n, &v2, &n);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        faces.push_back(Face(3, v, NULL, n - 1));
    }

    void add_face_3vtn(std::string &line){
        int v0, v1, v2, t0, t1, t2, n;
        sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        int *t = new int[3]{t0 - 1, t1 - 1, t2 - 1};
        faces.push_back(Face(3, v, t, n - 1));
    }

    void add_face_4v(std::string &line){
        int v0, v1, v2, v3;
        sscanf(line.c_str(), "f %d %d %d %d", &v0, &v1, &v2, &v3);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        faces.push_back(Face(4, v, NULL));
    }

    void add_face_4vt(std::string &line){
        int v0, v1, v2, v3, t0, t1, t2, t3;
        sscanf(line.c_str(), "f %d/%d %d/%d %d/%d %d/%d", &v0, &t0, &v1, &t1, &v2, &t2, &v3, &t3);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        int *t = new int[4]{t0 - 1, t1 - 1, t2 - 1, t3 - 1};
        faces.push_back(Face(4, v, t));
    }

    void add_face_4vn(std::string &line){
        int v0, v1, v2, v3, n;
        sscanf(line.c_str(), "f %d//%d %d//%d %d//%d %d//%d", &v0, &n, &v1, &n, &v2, &n, &v3, &n);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        faces.push_back(Face(4, v, NULL, n - 1));
    }

    void add_face_4vtn(std::string &line){
        int v0, v1, v2, v3, t0, t1, t2, t3, n;
        sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n, &v3,
            &t3, &n);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        int *t = new int[4]{t0 - 1, t1 - 1, t2 - 1, t3 - 1};
        faces.push_back(Face(4, v, t, n - 1));
    }

public:
    float pos_x, pos_y, pos_z;
    Model(){
        r = 0, g = 0, b = 0;
    }
    Model(const char *filename){
        load(filename);
    }
    void setColor(float r, float g, float b){
        this->r = r;
        this->g = g;
        this->b = b;
    }
    void load(const char *filename){
        std::string tmp = filename;
        prefix = "";
        int n = tmp.find_last_of('/') + 1;
        if(n > 0)
            prefix = tmp.substr(0, n);

        std::string line;
        std::vector<std::string> lines;
        std::ifstream in(filename);
        if(!in.is_open()){
            printf("Cannot load model %s\n", filename);
            return;
        }

        while(!in.eof()){
            std::getline(in, line);
            lines.push_back(line);
        }
        in.close();

        float a, b, c;
        char str[40];

        pos_x = pos_y = 0.0f;

        float sum_x = 0.0f, sum_y = 0.0f, sum_z = 0.0f;

        for(std::string &line : lines){
            if(line[0] == 'v'){
                if(line[1] == ' '){
                    sscanf(line.c_str(), "v %f %f %f", &a, &b, &c);
                    if(a > 0.0f)
                        sum_x += a;
                    else
                        sum_x -= a;
                    if(b > 0.0f)
                        sum_y += b;
                    else
                        sum_y -= b;
                    if(c > 0.0f)
                        sum_z += c;
                    else
                        sum_z -= c;
                    pos_x += a;
                    pos_y += b;
                    vertices.push_back(new float[3]{a, b, c});
                }
                else if(line[1] == 't'){
                    sscanf(line.c_str(), "vt %f %f", &a, &b);
                    texcoords.push_back(new float[2]{a, b});
                }
                else{
                    sscanf(line.c_str(), "vn %f %f %f", &a, &b, &c);
                    normals.push_back(new float[3]{a, b, c});
                }
            }
            else if(line[0] == 'f'){
                int edge = count_char(line, ' ');
                int count_slash = count_char(line, '/');
                if(count_slash == 0){
                    if(edge == 3)
                        add_face_3v(line);
                    else
                        add_face_4v(line);
                }
                else if(count_slash == edge){
                    if(edge == 3)
                        add_face_3vt(line);
                    else
                        add_face_4vt(line);
                }
                else if(count_slash == edge * 2){
                    if(has_double_slash(line)){
                        if(edge == 3)
                            add_face_3vn(line);
                        else
                            add_face_4vn(line);
                    }
                    else{
                        if(edge == 3)
                            add_face_3vtn(line);
                        else
                            add_face_4vtn(line);
                    }
                }
            }
        }

        bool has_texcoord = false;

        list = glGenLists(1);
        glNewList(list, GL_COMPILE);
        for(Face &face : faces){

            if(face.edge == -1){
                has_texcoord = false;

                continue;
            }
            if(face.normal != -1)
                glNormal3fv(normals[face.normal]);

            if(has_texcoord){
                // glColor3f(r, g, b);
                glBegin(GL_POLYGON);
                for(int i = 0; i < face.edge; i++){

                    glTexCoord2fv(texcoords[face.texcoords[i]]);
                    glVertex3fv(vertices[face.vertices[i]]);
                }
                glEnd();
            }
            else{
                // glColor3f(r, g, b);
                glBegin(GL_POLYGON);
                for(int i = 0; i < face.edge; i++){
                    glTexCoord2fv(texcoords[face.texcoords[i]]);
                    glVertex3fv(vertices[face.vertices[i]]);
                }
                glEnd();
            }

        }
        glEndList();

        printf("Model: %s\n", filename);
        printf("Vertices: %d\n", vertices.size());
        printf("Texcoords: %d\n", texcoords.size());
        printf("Normals: %d\n", normals.size());
        printf("Faces: %d\n", faces.size());

        sum_x /= vertices.size();
        sum_y /= vertices.size();
        sum_z /= vertices.size();
        pos_x /= vertices.size();
        pos_x = -pos_x;
        pos_y /= vertices.size();
        pos_y = -pos_y;
        pos_z = -sqrt(sum_x * sum_x + sum_y * sum_y + sum_z * sum_z) * 15;

        printf("Pos_X: %f\n", pos_x);
        printf("Pos_Y: %f\n", pos_y);
        printf("Pos_Z: %f\n", pos_z);


        map_material.clear();

        for(float *f : vertices)
            delete f;
        vertices.clear();
        for(float *f : texcoords)
            delete f;
        texcoords.clear();
        for(float *f : normals)
            delete f;
        normals.clear();
        faces.clear();
    }

    void draw(){ glCallList(list); }
};

#endif
