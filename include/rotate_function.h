#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define PI glm::pi<float>()
#define ESP 1e-6

glm::mat3 RotationMatrix(glm::vec3 axis, float theta){
    axis = glm::normalize(axis);
    float a = cos(theta / 2.0f);
    float b = -axis.x * sin(theta / 2.0f);
    float c = -axis.y * sin(theta / 2.0f);
    float d = -axis.z * sin(theta / 2.0f);
    float aa = a * a, bb = b * b, cc = c * c, dd = d * d;
    float bc = b * c, ad = a * d, ac = a * c, ab = a * b, bd = b * d, cd = c * d;
    glm::mat3 rot_mat;
    rot_mat[0][0] = aa + bb - cc - dd;
    rot_mat[0][1] = 2 * (bc + ad);
    rot_mat[0][2] = 2 * (bd - ac);
    rot_mat[1][0] = 2 * (bc - ad);
    rot_mat[1][1] = aa + cc - bb - dd;
    rot_mat[1][2] = 2 * (cd + ab);
    rot_mat[2][0] = 2 * (bd + ac);
    rot_mat[2][1] = 2 * (cd - ab);
    rot_mat[2][2] = aa + dd - bb - cc;
    return rot_mat;
}

glm::vec3 RotateMatrix(glm::vec3 O, glm::vec3 P, float degree){
    float t = degree * PI / 180.0f;
    glm::vec3 OP = P - O;
    glm::vec3 Z(0, 0, 1);
    glm::vec3 OM(0, 1, 0);
    glm::vec3 axis = glm::cross(OP, OM);
    axis = glm::normalize(axis);
    glm::mat3 R = RotationMatrix(axis, t);
    glm::vec3 P_prime = R * OP + O;
    return P_prime;
}

glm::vec3 RotateUp(glm::vec3 l, glm::vec3 v, float degree){
    degree = degree * PI / 180.0f;
    glm::vec3 result = v * cos(degree) + glm::cross(l, v) * sin(degree) + l * glm::dot(l, v) * (1 - cos(degree));
    return result;
}

glm::vec3 MoveCameraUD(glm::vec3 O, glm::vec3 P, float degree){
    glm::vec3 result = RotateMatrix(O, P, degree);
    glm::vec3 tresult = result - O;
    glm::vec3 tv(0, 1, 0);
    float cos_theta = glm::dot(tresult, tv) / (glm::length(tresult) * glm::length(tv));
    float angle = acos(cos_theta) * 180.0f / PI;
    if(angle < ESP || 180.0f - angle < ESP)
        return P;
    else
        return result;
}

glm::vec3 MoveCameraLR(glm::vec3 O, glm::vec3 P, float degree){
    float center_x = O.x, center_z = O.z;
    float tox = P.x - center_x, toz = P.z - center_z;
    degree = degree * PI / 180.0f;
    float tx = tox * cos(degree) - toz * sin(degree);
    float tz = tox * sin(degree) + toz * cos(degree);
    return glm::vec3(tx + center_x, P.y, tz + center_z);
}

glm::vec3 UpVector(glm::vec3 O, glm::vec3 P, float degree){
    glm::vec3 l = P - O;
    glm::vec3 v(l.z, 0, -l.x);
    l = glm::normalize(l);
    v = glm::normalize(v);
    glm::vec3 result = RotateUp(l, v, degree);
    return result;
}
