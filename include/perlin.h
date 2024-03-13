#include <cmath>
#include <cstdlib>


class perlin{
    // 一個簡單的線性插值函數
    float lerp(float a, float b, float t){
        return a + (b - a) * t;
    }

    // 一個簡單的平滑函數
    float smooth(float t){
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    // 一個簡單的雜湊函數，用於生成梯度向量
    int hash(int x, int y){
        int h = x * 31 + y;
        h = (h * 17) ^ (h >> 16);
        return h & 0xff;
    }

    // 一個簡單的梯度函數，用於計算點與梯度向量的內積
    float grad(int x, int y, float dx, float dy){
        int h = hash(x, y);
        float u = h < 128 ? dx : -dx;
        float v = h & 8 ? dy : -dy;
        return u + v;
    }

    // 一個生成perlin noise的函數
    // 接受一個浮點數陣列，陣列長度，亂數種子作為參數
    // 使這個陣列產生0到1之間的perlin noise
public:
    void perlin_noise(float *array, int length, int seed){
        // 設置亂數種子
        srand(seed);

        // 設置一個單位長度，用於控制noise的頻率
        float unit = 0.1f;

        // 遍歷陣列中的每個元素
        for(int i = 0; i < length; i++){
            // 計算當前元素對應的x座標
            float x = i * unit;

            // 計算x座標的整數部分和小數部分
            int x0 = (int) floor(x);
            int x1 = x0 + 1;
            float tx = x - x0;

            // 計算x座標的平滑值
            float sx = smooth(tx);

            // 隨機生成一個y座標，用於模擬二維空間
            int y = rand();

            // 計算四個頂點的梯度值
            float n0 = grad(x0, y, tx, 0);
            float n1 = grad(x1, y, tx - 1, 0);

            // 根據平滑值進行插值，得到最終的noise值
            float noise = lerp(n0, n1, sx);

            // 將noise值映射到0到1之間，並賦值給陣列元素
            array[i] = (noise + 1) / 2;
        }
    }

};