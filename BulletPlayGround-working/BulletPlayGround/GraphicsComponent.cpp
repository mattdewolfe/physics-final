#include "GraphicsComponent.h"
#include <GL\include\glut.h>
#include "../SDL2/include/SDL.h"
#include "../SDL2/include/SDL_opengl.h"
#include <math.h>

void GraphicsComponent::DrawCube()
{
	glBegin(GL_QUADS);
		glVertex3f(-m_Width, -m_Height, 0);
		glVertex3f(-m_Width, m_Height, 0);
		glVertex3f(m_Width, m_Height, 0);
		glVertex3f(m_Width, -m_Height, 0);
	glEnd();
}

void GraphicsComponent::DrawPlane()
{
	glBegin(GL_QUADS);
		glVertex4f(-1, -1, 0, 0);
		glVertex4f(-1, 1, 0, 0);
		glVertex4f(1, 1, 0, 0);
		glVertex4f(1, -1, 0, 0);
	glEnd();
}

void GraphicsComponent::DrawSphere()
{
    double radius = m_Radius;
    int nSlice = 10;
    int nStack = 10;
    int i, j;
    double phi; //
    double theta; //long
    float p[31][31][3]; //Vertex
    float *p1,*p2,*p3,*p4;

    if(nSlice > 30) nSlice = 30;
    if(nStack > 30) nStack = 30;

    //Vertex
    for(i = 0;i <= nSlice;i++)
    {   
        phi = 2.0 * M_PI * (double)i / (double)nSlice;
        for(j = 0;j <= nStack;j++)
        {   
            theta = M_PI * (double)j / (double)nStack;
            p[i][j][0] = (float)(radius * sin(theta) * cos(phi));//x
            p[i][j][1] = (float)(radius * sin(theta) * sin(phi));//y
            p[i][j][2] = (float)(radius * cos(theta));           //z
        }
    }

    //Top(j=0)
    for(i = 0;i < nSlice; i++)
    {
        p1 = p[i][0];     p2 = p[i][1];
        p3 = p[i+1][1]; 
        glBegin(GL_TRIANGLES);
        glNormal3fv(p1); glVertex3fv(p1);
        glNormal3fv(p2); glVertex3fv(p2);
        glNormal3fv(p3); glVertex3fv(p3);
        glEnd();
    }
    //Bottom
    j=nStack-1;
    for(i = 0;i < nSlice; i++)
    {
        p1 = p[i][j];     p2 = p[i][j+1];
        p3 = p[i+1][j]; 
        glBegin(GL_TRIANGLES);
        glNormal3fv(p1); glVertex3fv(p1);
        glNormal3fv(p2); glVertex3fv(p2);
        glNormal3fv(p3); glVertex3fv(p3);
        glEnd();
    }

    for(i = 0;i < nSlice;i++){
        for(j = 1;j < nStack-1; j++)
        {
            p1 = p[i][j];     p2 = p[i][j+1];
            p3 = p[i+1][j+1]; p4 = p[i+1][j];
            glBegin(GL_QUADS);
            glNormal3fv(p1); glVertex3fv(p1);
            glNormal3fv(p2); glVertex3fv(p2);
            glNormal3fv(p3); glVertex3fv(p3);
            glNormal3fv(p4); glVertex3fv(p4);
            glEnd();
        }
    }
}

void GraphicsComponent::ToggleColor()
{
	// Convert current color to an int, increment
	int temp = (int)(currentColor) + 1;
	// If int is greater than color enum count, set to 0
	if (temp >= (int)COLOR_Count)
		temp = 0;
	// Then convert back to graphic color and set to currentColor
	currentColor = (GraphicColor)temp;
	switch (currentColor)
	{
	case COLOR_Red:
		color = Entity::EVector3f(1.0, 0.0, 0.0);
		break;
	case COLOR_Green:
		color = Entity::EVector3f(0.0, 1.0, 0.0);
		break;
	case COLOR_Yellow:
		color = Entity::EVector3f(1.0, 0.7, 0.0);
		break;
	case COLOR_Blue:
		color = Entity::EVector3f(0.0, 0.0, 1.0);
		break;
	}
}

void GraphicsComponent::Update(float deltaTime)
{
	
}

void GraphicsComponent::Render()
{
	glColor3f(color.x,color.y,color.z);
    Entity::EVector3f pos = m_Owner->GetPosition();
    glTranslatef(pos.x,pos.y,pos.z);
	switch(type)
	{
	case GST_Sphere:
		DrawSphere();
		break;
	case GST_Cube:
		DrawCube();
		break;
	case GST_Plane:
		DrawPlane();
		break;
	}
}