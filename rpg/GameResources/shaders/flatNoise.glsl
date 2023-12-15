// Copyright (c) 2023. Johan Lind

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in ivec4 aBoneIds;
layout (location = 6) in vec4 aBoneWeights;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 TexCoords;
out vec3 Position;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
    Position = vec3(aPos);
}

/*BEGIN FRAG*/

out vec4 FragColor;
in vec3 Position;
in vec2 TexCoords;

uniform int heightLevels;
uniform vec3 colors[10];
uniform float heights[10];

#define M_PI 3.14159265358979323846

float rand(vec2 co){return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);}
float rand (vec2 co, float l) {return rand(vec2(rand(co), l));}
float rand (vec2 co, float l, float t) {return rand(vec2(rand(co, l), t));}

float perlin(vec2 p, float dim, float time) {
    vec2 pos = floor(p * dim);
    vec2 posx = pos + vec2(1.0, 0.0);
    vec2 posy = pos + vec2(0.0, 1.0);
    vec2 posxy = pos + vec2(1.0);

    float c = rand(pos, dim, time);
    float cx = rand(posx, dim, time);
    float cy = rand(posy, dim, time);
    float cxy = rand(posxy, dim, time);

    vec2 d = fract(p * dim);
    d = -0.5 * cos(d * M_PI) + 0.5;

    float ccx = mix(c, cx, d.x);
    float cycxy = mix(cy, cxy, d.x);
    float center = mix(ccx, cycxy, d.y);

    return center * 2.0 - 1.0;
}

float mod289(float x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 mod289(vec4 x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 perm(vec4 x){return mod289(((x * 34.0) + 1.0) * x);}

float noise(vec3 p){
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + a.zzzz;
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}

void main()
{
    vec3 color;

    vec3 pos = Position + perlin(Position.xy, 1500.0, 5.0) * 0.01;
    //pos += perlin(TexCoords.xy, 600.0, 5.0) * 0.01;


    for (int i = 0; i < heightLevels; ++i) {
        if (pos.y <= heights[i]) {
            float factor = (pos.y - (i == 0 ? heights[0] : heights[i - 1])) / (heights[i] - (i == 0 ? heights[0] : heights[i - 1]));

            color = mix(colors[i], colors[i + 1], factor);

            break;
        }
    }

    color += perlin(TexCoords.xy, 600.0, 5.0) * 0.01;
    color += perlin(TexCoords.xy, 3500.0, 5.0) * 0.01;
    color += perlin(TexCoords.xy, 6000.0, 5.0) * 0.01;


    // HDR tonemapping
    color = color / (color + vec3(1.0));

    // Gamma correction
    //color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}


