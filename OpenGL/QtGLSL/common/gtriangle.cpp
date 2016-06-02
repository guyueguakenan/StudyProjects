#include "gtriangle.h"
#include "glhelper.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
using namespace std;

GTriangle::GTriangle()
{
    _is_inited = false;

    _kPosAttribLocal = 0;
    _scale = 1.0f;
    _color = glm::vec4(1.0,0.0,0.0,1.0);
}

void GTriangle::GetPosData(GLfloat*& pos_data,int& size, int& count)
{
    static GLfloat default_pos_data[] = {
        -1.0, -1.0,  0.0,  1.0,
         0.0,  1.0,  0.0,  1.0,
         1.0, -1.0,  0.0,  1.0
    };
    pos_data = default_pos_data;
    size = sizeof(default_pos_data);
    count = 3;
}

void GTriangle::Init(GLfloat *pos_data, int size, int count)
{
    _is_inited = true;

    int pos_component_count = 4;
    if(!pos_data)
    {
        GetPosData(pos_data, _size, _count);
    }
    else
    {
        _count = count;
        _size  = size;
    }
    pos_component_count = _size / _count / sizeof(GLfloat);

    const char vs[] =
            "#version 410\n"
            "layout (location = 0) in vec4 position;\n"
            "uniform float scale;\n"
            "uniform vec4  color;\n"
            "out vec4 f_color;\n"
            "void main(void)\n"
            "{\n"
            "    mat4 scale_mat = mat4(scale);"
            "    scale_mat[3].w = 1.0f;"
            "    gl_Position = scale_mat*position;\n"
            "    f_color = color;\n"
            "}\n";
    const char fs[] =
            "#version 410\n"
            "in vec4 f_color;\n"
            "out vec4 color;\n"
            "void main(void)\n"
            "{\n"
            "    color = f_color;\n"
            "}\n";
    _program = GLHelper::CreateShaderProgram(vs,fs);
    glUseProgram(_program);
    {
        glGenVertexArrays(1,&_vaobject);
        glBindVertexArray(_vaobject);

        glGenBuffers(1,&_vabuffer);
        glBindBuffer(GL_ARRAY_BUFFER,_vabuffer);
        glBufferData(GL_ARRAY_BUFFER, _size, pos_data, GL_STATIC_DRAW);
        glVertexAttribPointer(_kPosAttribLocal,pos_component_count,GL_FLOAT,GL_FALSE,pos_component_count*sizeof(GLfloat),BUFF_OFFSET(0));
        glEnableVertexAttribArray(_kPosAttribLocal);

        _scaleUniformLocal = GLHelper::GetUniformLocal(_program,"scale");
        _colorUniformLocal = GLHelper::GetUniformLocal(_program,"color");

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }
    glUseProgram(0);
}

void GTriangle::SetScale(float scale)
{
    _scale = scale;
}

void GTriangle::SetColor(glm::vec4 color)
{
    _color = color;
}

void GTriangle::Draw()
{
    if(_is_inited)
    {
        glUseProgram(_program);
        {
            glUniform1f(_scaleUniformLocal,_scale);
            glUniform4fv(_colorUniformLocal,1,glm::value_ptr(_color));

            glBindVertexArray(_vaobject);
            glDrawArrays(GL_TRIANGLES,0,_count);
            glBindVertexArray(0);
        }
        glUseProgram(0);
    }
}
