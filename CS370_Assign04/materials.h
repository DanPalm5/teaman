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
						
MaterialType blue = {{0.1, 0.1, 0.3, 1.0},
					{0.0, 0.0, 0.7, 1.0},
					{0.5,0.50, 0.90, 1.0},
					 32.0f };


MaterialType lime = {{0.0215,	0.1745,	0.0215, 1.0},
					{0.07568, 0.61424, 0.07568, 1.0},
					{0.633, 0.727811, 0.633, 1.0},
										32 };

MaterialType transparent_lime = { { 0.02f, 0.174f, 0.021f, 0.5f },
								{ 0.075f, 0.6142f, 0.07568f, 0.5f },
								{ 0.633f, 0.7278f, 0.633f, 0.5f },
								32.0f };

MaterialType gray = {{ 0.50, 0.50, 0.50, 1.0 },
					{ 0.50, 0.50, 0.50, 1.0 }, 
					{ 0.50, 0.50, 0.50, 1.0 }, 
					32.0f};

void set_material(GLenum face, MaterialType *material)
{
	glMaterialfv(face,GL_AMBIENT,material->ambient);
	glMaterialfv(face,GL_DIFFUSE,material->diffuse);
	glMaterialfv(face,GL_SPECULAR,material->specular);
	glMaterialf(face,GL_SHININESS,material->shininess);
}
