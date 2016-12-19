//
// Created by iFinVer on 2016/11/22.
//

#ifndef MYOPENGLES_GL_SHADERS_H
#define MYOPENGLES_GL_SHADERS_H

#include <GLES2/gl2.h>

const GLfloat VERTICES_COORD[] =
        {
                -1.0f, 1.0f,
                -1.0f, -1.0f,
                1.0f, -1.0f,
                1.0f, 1.0f,
        };

//无镜像，左上-左下-右下-右上  左上-左下-右下-右上
//循环一次是为了处理旋转问题，
const GLfloat TEXTURE_COORD[] =
        {
                0.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f,
        };

class ShaderBase {
public:
    const char *vertexShader;
    const char *fragmentShader;

    ShaderBase() {
        vertexShader =
                "precision highp float;                                   \n"
                        "attribute highp vec2 aPosition;                  \n"
                        "attribute highp vec2 aTexCoord;                  \n"
                        "uniform highp int uRotation;                     \n"
                        "uniform highp float uScaleX;                     \n"
                        "uniform highp float uScaleY;                     \n"
                        "varying highp vec2 vTexCoord;                    \n"
                        "void main(){                                     \n"
                        "   vTexCoord = aTexCoord;                        \n"
                        "   vec2 rotPos = aPosition;                      \n"
                        "   if(uRotation == 1){                           \n"
                        "       rotPos = aPosition * mat2(0,-1,1,0);      \n"
                        "   }else if(uRotation == 2){                     \n"
                        "       rotPos = aPosition * mat2(-1,0,0,-1);     \n"
                        "   }else if(uRotation == 3){                     \n"
                        "       rotPos = aPosition * mat2(0,1,-1,0);      \n"
                        "   }                                             \n"
                        "                                                 \n"
                        "   mat2 scaleMtx = mat2(uScaleX,0,0,uScaleY);    \n"
                        "   gl_Position = vec4(scaleMtx * rotPos,1.0,1.0);\n"
                        "}                                                \n";
    }
};

class ShaderNV21 : public ShaderBase {
public:
    ShaderNV21() {
        fragmentShader =
                        "#extension GL_OES_EGL_image_external : require     \n"
                        "precision highp float;                             \n"
                        "varying highp vec2 vTexCoord;                      \n"
                        "uniform sampler2D yTexture;                        \n"
                        "uniform sampler2D uvTexture;                       \n"
                        "uniform highp int uRotation;                       \n"
                        "uniform int mirror;                                \n"
                        "                                                   \n"
                        "vec4 getBaseColor(in vec2 coord){                  \n"
                        "   float r,g,b,y,u,v;                              \n"
                        "   y = texture2D(yTexture,coord).r;                \n"
                        "   vec4 uvColor = texture2D(uvTexture,coord);      \n"
                        "   u = uvColor.a - 0.5;                            \n"
                        "   v = uvColor.r - 0.5;                            \n"
                        "   r = y + 1.13983*v;                              \n"
                        "   g = y - 0.39465*u - 0.58060*v;                  \n"
                        "   b = y + 2.03211*u;                              \n"
                        "   return vec4(r, g, b, 1.0);                      \n"
                        "}                                                  \n"
                        "                                                   \n"
                        "vec2 mirrorUV(){                                   \n"
                        "   vec2 mirrorCoord = vTexCoord;                   \n"
                        "   if(mirror == 1){                                \n"
                        "       if(uRotation == 1 || uRotation == 3){       \n"
                        "           mirrorCoord.y = 1.0 - mirrorCoord.y;    \n"
                        "       }else{                                      \n"
                        "           mirrorCoord.x = 1.0 - mirrorCoord.x;    \n"
                        "       }                                           \n"
                        "   }                                               \n"
                        "   return mirrorCoord;                             \n"
                        "}                                                  \n"
                        "                                                   \n"
                        "void main(){                                       \n"
                        "   vec2 mirrorCoord = mirrorUV();                  \n"
                        "   vec4 color = getBaseColor(mirrorCoord);         \n"
                        "   gl_FragColor = color;                           \n"
                        "}                                                  \n";
    }
};
class ShaderRGB : public ShaderBase {
public:
    ShaderRGB() {
        fragmentShader =
                        "#extension GL_OES_EGL_image_external : require     \n"
                        "precision highp float;                             \n"
                        "varying highp vec2 vTexCoord;                      \n"
                        "uniform sampler2D sTexture;                        \n"
                        "uniform highp int uRotation;                       \n"
                        "uniform int mirror;                                \n"
                        "                                                   \n"
                        "vec2 mirrorUV(){                                   \n"
                        "   vec2 mirrorCoord = vTexCoord;                   \n"
                        "   if(mirror == 1){                                \n"
                        "       if(uRotation == 1 || uRotation == 3){       \n"
                        "           mirrorCoord.y = 1.0 - mirrorCoord.y;    \n"
                        "       }else{                                      \n"
                        "           mirrorCoord.x = 1.0 - mirrorCoord.x;    \n"
                        "       }                                           \n"
                        "   }                                               \n"
                        "   return mirrorCoord;                             \n"
                        "}                                                  \n"
                        "                                                   \n"
                        "void main(){                                       \n"
                        "   vec2 mirrorCoord = mirrorUV();                  \n"
                        "   gl_FragColor = texture2D(sTexture,mirrorCoord); \n"
                        "}                                                  \n";
    }
};

class ShaderPoint {
public:
    const char *vertexShader;
    const char *fragmentShader;

    ShaderPoint() {
        vertexShader =
                "precision highp float;                                         \n"
                        "attribute highp vec2 aPosition;                        \n"
                        "attribute highp float aScaleX;                         \n"
                        "attribute highp float aScaleY;                         \n"
                        "void main(){                                           \n"
                        "   highp mat2 aScaleMtx = mat2(aScaleX,0,0,aScaleY);   \n"
                        "   gl_Position = vec4(aScaleMtx * aPosition,1.0,1.0);  \n"
                        "   gl_PointSize = 10.0;                                \n"
                        "}                                                      \n";

        fragmentShader =
                "precision highp float;                     \n"
                        "uniform vec4 color;                \n"
                        "void main(){                       \n"
                        "   gl_FragColor = color;           \n"
                        "}                                  \n";

    }
};

#endif