#version 430

out vec3 pos;
out vec3 vel;
out vec3 acc;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inVel;
layout(location = 2) in vec3 inAcc;

layout(std430, binding = 3) buffer pointMass
{
    vec3 point;
    float enabled;
    float deltaTime;
};

uniform uint particleCount;
layout(std430, binding = 4) buffer pointsData
{
    float points[];
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 position;
bool test = true;
void main() {
    vel = inVel;
    pos = inPos;

    int index = -3;
    int currIndex; 
    vec3 totalForceVector = vec3(0.0f,0.0f,0.0f);
    for (int i=0; i<particleCount; i++) {
        vec3 currPos = vec3(points[index], points[index+1], points[index+2]);
        index += 3;
        if (inPos == currPos) {
            currIndex = index;
            continue;
        }
        vec3 direction = currPos-inPos;
        float len2 = dot(direction, direction);
        direction = normalize(direction);
        // vec3 force = direction * ((1.0f * 75.0f * 1.0f)/pow(len < 5.0f ? 5.0f : len, 2.0f));
        vec3 force = direction * ((200.0f * 1.0f * 1.0f)/(len2 < 50.0f ? 50.0f: len2));

        totalForceVector += (force);
    }
    acc = ((totalForceVector) * deltaTime);
    vel += (acc * deltaTime);
    // if (length(vel) > 150.0f) {
    //     vel = normalize(vel) * 150.0f;
    // }
    
    // vec3 currPos = vec3(inPos.x, inPos.y, inPos.z);
    // vec3 direction = point - currPos;
    // float len = length(direction);
    // if (len < 0.06) {
    //     len = 0.06;
    // }
    // float force = ((90000000.0f) * (1.0f))/(pow(len, 2));
    // direction = normalize(direction) * force;    
    // acc = (direction * deltaTime * enabled);
    // vel += acc * deltaTime;
    
    vel *= pow(0.98f, deltaTime);
    
    pos += (vel * deltaTime);
    points[currIndex] = pos.x;
    points[currIndex+1] = pos.y;
    points[currIndex+2] = pos.z;
    gl_Position = projection * view * model * vec4(pos, 1.0);
    gl_PointSize = 1.1f;
}
