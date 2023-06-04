#pragma once

class DeltaTime {
private:
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currentFrame = 0.0f;
    static DeltaTime *obj;
    DeltaTime() {}
public:
    ~DeltaTime() {};
    static void calculateDeltaTime();
    static float getDeltaTime();
};