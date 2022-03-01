#include <windows.h>
#include <gl/GL.h>
#include "../headers/opengl/khrplatform.h"
#include "../headers/opengl/glext.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "opengl32.lib")

#include "../headers/mindless_renderer.h"
#include "../headers/mindless_entities.h"
#include "../headers/mindless_matrix.h"
#include "../headers/mindless_trig.h"

//Function pointer alias madness
typedef void (APIENTRY * PFNGLGENBUFFERS)(GLsizei, GLuint*);
typedef void (APIENTRY * PFNGLBINDBUFFER)(GLenum, GLuint);
typedef void (APIENTRY * PFNGLBUFFERDATA)(GLenum, GLsizeiptr, const GLvoid*, GLenum);
typedef void (APIENTRY * PFNGLVERTEXATTRIBPOINTER)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
typedef void (APIENTRY * PFNGLENABLEVERTEXATTRIBARRAY)(GLuint);
typedef GLuint (APIENTRY * PFNGLCREATESHADER)(GLenum);
typedef GLuint (APIENTRY * PFNGLCREATEPROGRAM)(void);
typedef void (APIENTRY * PFNGLSHADERSOURCE)(GLuint, GLsizei, const GLchar**, const GLint*);
typedef void (APIENTRY * PFNGLCOMPILESHADER)(GLuint);
typedef void (APIENTRY * PFNGLDELETESHADER)(GLuint);
typedef void (APIENTRY * PFNGLSPECIALIZESHADER)(GLuint, const GLchar*, GLuint, const GLuint, const GLuint);
typedef void (APIENTRY * PFNGLGETSHADERIV)(GLuint, GLenum, GLint*);
typedef void (APIENTRY * PFNGLATTACHSHADER)(GLuint, GLuint);  
typedef void (APIENTRY * PFNGLGETSHADERINFOLOG)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef void (APIENTRY * PFNGLLINKPROGRAM)(GLuint);
typedef void (APIENTRY * PFNGLVALIDATEPROGRAM)(GLuint);
typedef void (APIENTRY * PFNGLUSEPROGRAM)(GLuint);
typedef void (APIENTRY * PFNGLGENVERTEXARRAYS)(GLsizei, GLuint*);
typedef void (APIENTRY * PFNGLBINDVERTEXARRAY)(GLuint);
typedef GLint (APIENTRY * PFNGLGETUNIFORMLOCATION)(GLuint, const GLchar*);
typedef void (APIENTRY * PFNGLUNIFORMMATRIX4FV)(GLint, GLsizei, GLboolean, const GLfloat*);
typedef void (APIENTRY * PFNGLDETACHSHADER)(GLuint, GLuint);
typedef void (APIENTRY * PFNGLBINDATTRIBLOCATION)(GLuint, GLuint, const GLchar*);
typedef void (APIENTRY * PFNGLUNIFORM4F)(GLint, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (APIENTRY * PFNGLBINDBUFFERBASE)(GLenum, GLuint, GLuint);


PFNGLGENBUFFERS glGenBuffers;
PFNGLBINDBUFFER glBindBuffer;
PFNGLBUFFERDATA glBufferData;
PFNGLVERTEXATTRIBPOINTER glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAY glEnableVertexAttribArray;
PFNGLCREATESHADER glCreateShader;
PFNGLCREATEPROGRAM glCreateProgram;
PFNGLSHADERSOURCE glShaderSource;
PFNGLCOMPILESHADER glCompileShader;
PFNGLDELETESHADER glDeleteShader;
PFNGLSPECIALIZESHADER glSpecializeShader;
PFNGLGETSHADERIV glGetShaderiv;
PFNGLATTACHSHADER glAttachShader;
PFNGLGETSHADERINFOLOG glGetShaderInfoLog;
PFNGLLINKPROGRAM glLinkProgram;
PFNGLVALIDATEPROGRAM glValidateProgram;
PFNGLUSEPROGRAM glUseProgram;
PFNGLGENVERTEXARRAYS glGenVertexArrays;
PFNGLBINDVERTEXARRAY glBindVertexArray;
PFNGLGETUNIFORMLOCATION glGetUniformLocation;
PFNGLUNIFORMMATRIX4FV glUniformMatrix4fv;
PFNGLDETACHSHADER glDetachShader;
PFNGLBINDATTRIBLOCATION glBindAttribLocation;
PFNGLUNIFORM4F glUniform4f;
PFNGLBINDBUFFERBASE glBindBufferBase;


struct renderData 
{
    float PxPerMeter;
    float CameraWidth;
    float CameraHeight;

    float FOV;
    float AspectRatio;

    int Count;

    unsigned int Program;
    unsigned int VertexShaderId;
    unsigned int FragmentShaderId;

    float* RenderVertexBuffer_3D;
    unsigned short* RenderIndexBuffer_3D;
    float* RenderVertexAddress;
    unsigned short* RenderIndexAddress;

    GLuint VAO;
    GLuint VBO;
    GLuint IBO;


    
} RenderData;

void *GetGlAddress(const char name[])
{
    void *p = (void *)wglGetProcAddress(name);
    if(p == 0 ||
        (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
        (p == (void*)-1) )
    {
        HMODULE module = LoadLibraryA("opengl32.dll");
        p = (void *)GetProcAddress(module, name);
    }

    return p;
}

unsigned int CompileShader(const char shaderSource[], unsigned int type) 
{
    GLuint id = glCreateShader(type);

    glShaderSource(id, 1, (const GLchar**)&shaderSource, nullptr);
    glCompileShader(id);

    int compiled;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
    if(compiled == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

//called ONCE
//Get OpenGL Functions from GPU DRIVER
void InitOpenGlFunctions()
{   
    glGenBuffers = (PFNGLGENBUFFERS)GetGlAddress("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFER)GetGlAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATA)GetGlAddress("glBufferData");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTER)GetGlAddress("glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAY)GetGlAddress("glEnableVertexAttribArray");
    glCreateShader = (PFNGLCREATESHADER)GetGlAddress("glCreateShader");
    glCreateProgram = (PFNGLCREATEPROGRAM)GetGlAddress("glCreateProgram");
    glShaderSource = (PFNGLSHADERSOURCE)GetGlAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADER)GetGlAddress("glCompileShader");
    glDeleteShader = (PFNGLDELETESHADER)GetGlAddress("glDeleteShader");
    glSpecializeShader = (PFNGLSPECIALIZESHADER)GetGlAddress("glSpecializeShader");
    glGetShaderiv = (PFNGLGETSHADERIV)GetGlAddress("glGetShaderiv");
    glAttachShader = (PFNGLATTACHSHADER)GetGlAddress("glAttachShader");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOG)GetGlAddress("glGetShaderInfoLog");
    glLinkProgram = (PFNGLLINKPROGRAM)GetGlAddress("glLinkProgram");
    glValidateProgram = (PFNGLVALIDATEPROGRAM)GetGlAddress("glValidateProgram");
    glUseProgram = (PFNGLUSEPROGRAM)GetGlAddress("glUseProgram");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYS)GetGlAddress("glGenVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAY)GetGlAddress("glBindVertexArray");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATION)GetGlAddress("glGetUniformLocation");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FV)GetGlAddress("glUniformMatrix4fv");
    glDetachShader = (PFNGLDETACHSHADER)GetGlAddress("glDetachShader");
    glBindAttribLocation = (PFNGLBINDATTRIBLOCATION)GetGlAddress("glBindAttribLocation");
    glUniform4f = (PFNGLUNIFORM4F)GetGlAddress("glUniform4f");
    glBindBufferBase = (PFNGLBINDBUFFERBASE)GetGlAddress("glBindBufferBase");
    
    RenderData.Program = glCreateProgram();
}

void SetGlProjection()
{
    if(RenderData.Program != 0) 
    {
        float NearPlane = 0.1f;
        float FarPlane = 6;
        RenderData.FOV = 1.5f;
        mat4 projMatrix{};
        
        float YYProjMatrix = 1 / (Tan(0.5f * RenderData.FOV));
        float XXProjMatrix = YYProjMatrix / RenderData.AspectRatio;
        float ZZProjMatrix = (NearPlane + FarPlane) / (NearPlane - FarPlane);
        float WZProjMatrix = (2 * (NearPlane + FarPlane)) / (NearPlane - FarPlane);

        projMatrix.X = {XXProjMatrix, 0, 0, 0};
        projMatrix.Y = {0, YYProjMatrix, 0, 0};
        projMatrix.Z = {0, 0, ZZProjMatrix, -1};
        projMatrix.W = {0, 0, WZProjMatrix, 0};

        GLint loc = glGetUniformLocation(RenderData.Program, "proj_matrix");
        glUniformMatrix4fv(loc, 1, GL_FALSE,  &projMatrix.X.X);
    }
}

void InitOpenGlShaders() 
{
    const char vsSource[] = 
    "#version 460 core\n"

    "layout(location = 0)in vec3 position;\n"

    "uniform mat4 mv_matrix;\n"
    "uniform mat4 proj_matrix;\n"

    "void main()\n"
    "{\n"
        "gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);\n"
    "}\n";

    const char fsSource[] =   
    "#version 460 core\n"
    "layout(location = 0)out vec4 color;\n"
    "void main()\n"
    "{\n"
        "color = vec4(0.0, 0.0, 1.0, 1.0);\n"
    "}\n";

    RenderData.VertexShaderId = CompileShader(vsSource, GL_VERTEX_SHADER);
    RenderData.FragmentShaderId = CompileShader(fsSource, GL_FRAGMENT_SHADER);

    glAttachShader(RenderData.Program, RenderData.VertexShaderId);
    glAttachShader(RenderData.Program, RenderData.FragmentShaderId);

    glLinkProgram(RenderData.Program);
    glValidateProgram(RenderData.Program);

    glUseProgram(RenderData.Program);

    SetGlProjection();
}

void InitBuffers_3D
(
    
)
{
    static float RenderVertexBuffer_3D[24] = {};
    static unsigned short RenderIndexBuffer_3D[12] = {};

    GLuint VAO;
    GLuint VBO;
    GLuint IBO;

    RenderData.Program = glCreateProgram();
    
    //3D VERTEX BUFFER 
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBindAttribLocation(RenderData.Program, 0, "position");
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    RenderData.RenderVertexBuffer_3D = RenderVertexBuffer_3D;
    RenderData.RenderVertexAddress = RenderVertexBuffer_3D;

    int xD = sizeof(RenderVertexBuffer_3D);

    //3D INDEX BUFFER
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    RenderData.RenderIndexBuffer_3D = RenderIndexBuffer_3D;
    RenderData.RenderIndexAddress = RenderIndexBuffer_3D;

    RenderData.VAO = VAO;
    RenderData.VBO = VBO;
    RenderData.IBO = IBO;
}

void Render(HDC WindowDC) 
{
    float vertex_positions[] =
    { 
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
    };

    unsigned short indexData[] =
    {
        0, 1, 2, 
        2, 1, 3,
        4, 0, 6, 
        6, 0, 2,
        7, 5, 6, 
        6, 5, 4,
        3, 1, 7, 
        7, 1, 5,
        4, 5, 0, 
        0, 5, 1,
        3, 7, 2, 
        2, 7, 6
    };

    int xD1 = sizeof(vertex_positions);
    int xD2 = sizeof(indexData);

    memcpy(RenderData.RenderVertexBuffer_3D, &vertex_positions, sizeof(vertex_positions));
    memcpy(RenderData.RenderIndexBuffer_3D, &indexData, sizeof(indexData));


    mat4 mv_matrix;
    mv_matrix.X = {1, 0, 0, 0};
    mv_matrix.Y = {0, 1, 0, 0};
    mv_matrix.Z = {0, 0, 1, 0};
    mv_matrix.W = {0, 0, -6, 1};

    GLint loc = glGetUniformLocation(RenderData.Program, "mv_matrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE,  &mv_matrix.X.X);
    
    //glClear(GL_COLOR_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glEnable(GL_DEPTH_TEST);

    //glDepthFunc(GL_LEQUAL);

    glBindBuffer(GL_ARRAY_BUFFER, RenderData.VBO);
    glBufferData(GL_ARRAY_BUFFER, 96, &RenderData.RenderVertexBuffer_3D[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderData.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 72, &RenderData.RenderIndexBuffer_3D[0], GL_DYNAMIC_DRAW);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);

    SwapBuffers(WindowDC);
}


void glResizeWindow(GLsizei width, GLsizei height)
{
    RenderData.AspectRatio = (float)width / height;
    RenderData.CameraHeight = 35;
    RenderData.CameraWidth = 35 * RenderData.AspectRatio;

    glViewport(0, 0, width, height);
    SetGlProjection();
}

void UpdateBuffers(uEntities* UEntities, entity* Entities)
{
    //TODO Optimize(Don't Clear buffers instead replace entities)
    //clear arrays
    memset(RenderData.RenderVertexBuffer_3D, 0, 8192);
    memset(RenderData.RenderIndexBuffer_3D, 0, 8192);

    //Update Model View matrix;
    glBindBuffer(GL_ARRAY_BUFFER, RenderData.VBO);
    glBufferData(GL_ARRAY_BUFFER, 8192, RenderData.RenderVertexBuffer_3D, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, RenderData.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8192, RenderData.RenderIndexBuffer_3D, GL_DYNAMIC_DRAW);
}


