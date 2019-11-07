typedef struct MaterialType {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
} MaterialType;

MaterialType brass = {{0.33f,0.22f,0.03f,1.0f},
					{0.78f,0.57f,0.11f,1.0f},
					
					{0.99f,0.91f,0.81f,1.0f},
					27.8f};

MaterialType red_plastic = {{0.3f,0.0f,0.0f,1.0f},
						{0.6f,0.0f,0.0f,1.0f},
						{0.8f,0.6f,0.6f,1.0f},
						32.0f};
						
void set_material(GLenum face, MaterialType *material)
{
	glMaterialfv(face,GL_AMBIENT,material->ambient);
	glMaterialfv(face,GL_DIFFUSE,material->diffuse);
	glMaterialfv(face,GL_SPECULAR,material->specular);
	glMaterialf(face,GL_SHININESS,material->shininess);
}
