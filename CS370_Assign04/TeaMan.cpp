// CS370 - Fall 2018
// Assign04 - Textured Tea Man
//Daniel Palmieri 12:30-1:45 T/TR
//Dr. Babcock - To toggle animation of robot, use A. If you get tired of this robot, try pressing E. (I improved upon my blocks from assignment 2 :) )
//																						 P.S. - you can press E again to "undo" what you just did. 

#ifdef OSX
	#include <GLUT/glut.h>
#else
	#include <GL/glew.h>
	#include <GL/glut.h>
#endif
#include <SOIL/SOIL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lighting.h"
#include "materials.h"
#include "robot.h"
#include "treenode.h"

// Shader file utility functions
#include "shaderutils.h"

// Shader files
GLchar* defaultVertexFile = "basicvert.vs";
GLchar* defaultFragmentFile = "basicfrag.fs";
GLchar* lightVertexFile = "lightvert.vs";
GLchar* lightFragmentFile = "lightfrag.fs";
GLchar* texVertexFile = "texturevert.vs";
GLchar* texFragmentFile = "texturefrag.fs";

// Shader objects
GLuint defaultShaderProg;
GLuint lightShaderProg;
GLuint textureShaderProg;
GLuint numLights_param;
GLint texSampler;

// Initial light position
GLfloat light_pos[4] = { 0.0, 30.0, 10.0, 1.0 };


// Global light variables
GLint numLights = 1;

// Texture constants
#define NO_TEXTURES 1
#define SHIRT 0
#define X 0
#define Y 1
#define Z 2
#define ANG_RANGE 60
#define DEG_PER_SEC (360.0/60.0)
#define DEG2RAD (3.14159/180.0)

// Texture indices
GLuint tex_ids[NO_TEXTURES];

// Texture files
char texture_files[NO_TEXTURES][20] = { "shirt_z.png" };

// Global camera vectors
GLfloat eye[3] = {0.0f,0.0f,25.0f};
GLfloat at[3] = {0.0f,0.0f,0.0f};
GLfloat up[3] = {0.0f,1.0f,0.0f};
int startx=0;
int starty=0;
// Global spherical coord values
GLfloat azimuth = 0.0f;
GLfloat daz = 2.0f;
GLfloat elevation = 90.0f;
GLfloat del = 2.0f;
GLfloat radius = 25.0f;

// Global screen dimensions
GLint ww,hh;

// Cube vertices
GLfloat cube[][3] = { { -1.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, 1.0f },
{ -1.0f, -1.0f, 1.0f }, { -1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, -1.0f },
{ 1.0f, 1.0f, 1.0f }, { -1.0f, 1.0f, 1.0f } };


// Cube texture coords
/*
408/582 = 0.701030928
41/313 = 0.130990415
204/582 = 0.350515464
495/582 = 0.850515464
*/
GLfloat cube_tex[][2] = { {0.701030928, 0.130990415},{0.701030928, 0}, {1,0}, {1,0.130990415}, //top
						{0.701030928, 0}, {1,0}, {1, 0.130990415}, {0.701030928, 0.130990415}, //bottom
						{0.850515464, 0.130990415}, {0.850515464, 1}, {0.701030928, 1}, {0.701030928, 0.130990415},	//left
						{ 1, 0.130990415 }, { 1,1 }, {0.850515464 ,1}, {0.850515464, 0.130990415},  //right
						{.350515464,0},{.350515464,1 }, { 0,1 }, { 0, 0 }, //front
						{0.350515464,0}, {0.701030928, 0}, {0.701030928, 1}, {0.350515464, 1} };//back
						

// Robot nodes
treenode head;
treenode torso;

treenode left_shoulder;
treenode right_shoulder;

treenode upper_left_arm;
treenode upper_right_arm;
treenode lower_left_arm;
treenode lower_right_arm;

treenode upper_left_leg;
treenode upper_right_leg;
treenode lower_left_leg;
treenode lower_right_leg;

treenode box;
treenode teapot;

// Rotation angles
GLfloat teapot_theta = 0.0f;
GLfloat left_shoulder_theta = 0.0f;
GLfloat right_shoulder_theta = 0.0f;

GLfloat upper_left_arm_theta = 0.0f;
GLfloat upper_right_arm_theta = 0.0f;
GLfloat lower_left_arm_theta = 0.0f;
GLfloat lower_right_arm_theta = 0.0f;

GLfloat upper_left_leg_theta = 0.0f;
GLfloat upper_right_leg_theta = 0.0f;
GLfloat lower_left_leg_theta = 0.0f;
GLfloat lower_right_leg_theta = 0.0f;

// Animation variables
GLint time = 0;
GLint lasttime = 0;
GLint fps = 30;
GLint spin_teapot = 0;
GLfloat rpm = 20.0f;
GLfloat sps = 20.0f;
GLint animate = 0;
GLint shoulder_dir = 1;
GLint elbow_dir = 1;
GLint leg_dir = 1;
GLint knee_dir = 1;
GLint explode = 0;
GLfloat explode_dist = 0.0f;
GLint num_robots_created = 0;

// Camera rotation variables

// Billboard variables
unsigned char* background_image;
GLint width = 500;
GLint height = 500;
GLint channels;
GLchar* filename = "robotfactory_2.JPG";

// quadric object for ears
GLUquadricObj* quadric;

void display();
void render_Scene();
void reshape(int w, int h);
void keyfunc(unsigned char key, int x, int y);
void idlefunc();
void mousefunc(int btn, int state, int x, int y);
void mousemove(int x, int y);
void traverse(treenode *node);
void create_scene_graph();
void load_image();
void draw_background();
bool load_textures();
void texturecube();
void texquad(GLfloat v1[], GLfloat v2[], GLfloat v3[], GLfloat v4[], GLfloat t1[], GLfloat t2[], GLfloat t3[], GLfloat t4[]);
void draw_torso();
void draw_head();
void draw_shoulder();
void draw_lower_arm();
void draw_upper_arm();
void draw_lower_leg();
void draw_upper_leg();
void draw_box();
void draw_teapot();
void update_torso();
void update_head();
void update_left_shoulder();
void update_right_shoulder();
void update_lower_left_arm();
void update_lower_right_arm();
void update_upper_left_arm();
void update_upper_right_arm();
void update_lower_left_leg();
void update_lower_right_leg();
void update_upper_left_leg();
void update_upper_right_leg();
void update_box();
void update_teapot();

int main(int argc, char* argv[])
{
	// Initialize glut
	glutInit(&argc, argv);

	// Initialize window
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(500, 500);
	glutCreateWindow("Robot");

#ifndef OSX
	// Initialize GLEW - MUST BE DONE AFTER CREATING GLUT WINDOW
	glewInit();
#endif

	// Define callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyfunc);
	glutIdleFunc(idlefunc);
	glutMouseFunc(mousefunc);
	glutMotionFunc(mousemove);

	// Compute initial cartesian camera position
	eye[X] = (GLfloat)(radius * sin(azimuth * DEG2RAD) * sin(elevation * DEG2RAD));
	eye[Y] = (GLfloat)(radius * cos(elevation * DEG2RAD));
	eye[Z] = (GLfloat)(radius * cos(azimuth * DEG2RAD) * sin(elevation * DEG2RAD));

	// Set background color
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Set initial ambient light
	GLfloat background[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	set_AmbientLight(background);

	// Create quadric
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	// Load shader programs
	defaultShaderProg = load_shaders(defaultVertexFile,defaultFragmentFile);
	lightShaderProg = load_shaders(lightVertexFile, lightFragmentFile);
	textureShaderProg = load_shaders(texVertexFile, texFragmentFile);

	// Associate shader parameters
	numLights_param = glGetUniformLocation(lightShaderProg,"numLights");
	texSampler = glGetUniformLocation(textureShaderProg, "texMap");

	// Load textures
	if (!load_textures())
	{
		exit(0);
	}

	// load image
	load_image();

	// Build scene graph
	create_scene_graph();

	// Start graphics loop
	glutMainLoop();
	return 0;
}

// Display routine
void display()
{
	// Clear background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw background image
	draw_background();

	// Set projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Adjust viewing volume (orthographic)
	GLfloat xratio = 1.0f;
	GLfloat yratio = 1.0f;
	// If taller than wide adjust y
	if (ww <= hh)
	{
		yratio = (GLfloat)hh / (GLfloat)ww;
	}
	// If wider than tall adjust x
	else if (hh <= ww)
	{
		xratio = (GLfloat)ww / (GLfloat)hh;
	}
	glOrtho(-25.0f*xratio, 25.0f*xratio, -10.0f*yratio, 25.0f*yratio, 0.0f, 50.0f);

	// Set modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[X], eye[Y], eye[Z], at[X], at[Y], at[Z], up[X], up[Y], up[Z]);

	// Render scene
	render_Scene();

	// Flush buffer
	glFlush();

	// Swap buffers
	glutSwapBuffers();
}

void render_Scene() 
{
	// Turn on and position LIGHT0
	set_PointLight(GL_LIGHT0,&white_light,light_pos);

	// render the scene graph
	traverse(&torso);

}

// Keyboard callback
void keyfunc(unsigned char key, int x, int y)
{
	// T to spin teapot
	if (key == ' ')
	{
		spin_teapot = !spin_teapot;
	}

	if (key == 'A' || key == 'a')
	{
		animate = !animate;
	}

	if (key == 'E' || key == 'e') 
	{
		explode = !explode;
		num_robots_created++;
	}

	// Esc to quit
	if (key == 27)
	{
		exit(0);
	}
}

// Idle callback
void idlefunc()
{
	// Time-based Animations
	
	// Get total elapsed time
	time = glutGet(GLUT_ELAPSED_TIME);

	// Update if past desired interval
	if (time - lasttime > 1000.0f / fps)
	{
		if (spin_teapot) {
			teapot_theta += 6.0f * rpm * (time - lasttime) / 1000.0f;
			if (teapot_theta > 360.0f)
			{
				teapot_theta -= 360.0f;
			}
			update_teapot();
		}

		if (animate) {
			// move arms and legs
				// compute shoulder movements
			right_shoulder_theta+= 5.0f * shoulder_dir * sps* (time - lasttime) / 1000.0f;
			left_shoulder_theta -= 5.0f * shoulder_dir *  sps * (time - lasttime) / 1000.0f;
				// leg movements
			upper_right_leg_theta -= 2.5f * leg_dir * sps * (time - lasttime) / 1000.0f;
			upper_left_leg_theta += 2.5f * leg_dir *sps* (time - lasttime) / 1000.0f;

			lower_right_leg_theta += 1.0f * knee_dir * sps * (time - lasttime) / 1000.0f;
			lower_left_leg_theta -= 1.0 * knee_dir * sps * (time - lasttime) / 1000.0f;


				// arm movements
			lower_right_arm_theta += 1.0f * elbow_dir * sps * (time - lasttime) / 1000.0f;
			lower_left_arm_theta -= 1.0f * elbow_dir * sps * (time - lasttime) / 1000.0f;


			if (right_shoulder_theta > ANG_RANGE)
			{
				shoulder_dir *= -1;
				elbow_dir *= -1;
				knee_dir *= -1;
			}
			if (right_shoulder_theta < -ANG_RANGE) 
			{
				shoulder_dir *= -1;
				elbow_dir *= -1;
				knee_dir *= -1;
			}

			if (right_shoulder_theta / 2 > ANG_RANGE/2)
			{
				leg_dir *= -1;
			}
			if (right_shoulder_theta / 2 < -ANG_RANGE/2)
			{
				leg_dir *= -1;
			}

			update_right_shoulder();
			update_left_shoulder();
			update_upper_right_leg();
			update_upper_left_leg();
			update_lower_right_arm();
			update_lower_left_arm();
			update_lower_right_leg();
			update_lower_left_leg();

		}
		// exploding if e is pressed
		if (explode) 
		{
			// compute movement (time based)
			explode_dist += 4.0f * sps * (time - lasttime) / 1000.0f;


			update_head();
			update_right_shoulder();
			update_left_shoulder();
			update_upper_right_leg();
			update_upper_left_leg();
			update_lower_right_arm();
			update_lower_left_arm();
			update_lower_right_leg();
			update_lower_left_leg();
			update_box();
		
		}
		// move it back
		if (!explode) 
		{
			if (explode_dist > 0.0f)
			{
				explode_dist = -explode_dist;
			}
			else if (explode_dist <= 0) 
			{
				explode_dist = 0;
			}
			update_head();
			update_right_shoulder();
			update_left_shoulder();
			update_upper_right_leg();
			update_upper_left_leg();
			update_lower_right_arm();
			update_lower_left_arm();
			update_lower_right_leg();
			update_lower_left_leg();
			update_box();
		}
		// Update lasttime (reset timer)
		lasttime = time;

		glutPostRedisplay();
	}
}

// Mouse callback
void mousefunc(int btn, int state, int x, int y)
{
	// Rotate around x and y axes for left button
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		startx = x;
		starty = y;
	}

}

// Mouse motion callback
void mousemove(int x, int y)
{
	// Adjust x and y rotation angles
	elevation += (y - starty);
	if (elevation > 179.8)
	{
		elevation = 179.8f;
	}
	else if (elevation < 0.0)
	{
		elevation = 0.2f;
	}

	azimuth += (x - startx);
	if (azimuth > 360.0)
	{
		azimuth -= 360.0;
	}
	else if (azimuth < 0.0)
	{
		azimuth += 360.0;
	}

	// Compute cartesian camera position
	eye[X] = (GLfloat)(radius*sin(azimuth*DEG2RAD)*sin(elevation*DEG2RAD));
	eye[Y] = (GLfloat)(radius*cos(elevation*DEG2RAD));
	eye[Z] = (GLfloat)(radius*cos(azimuth*DEG2RAD)*sin(elevation*DEG2RAD));

	// Update mouse reference position
	startx = x;
	starty = y;

	// Redraw display
	glutPostRedisplay();
}

// Reshape callback
void reshape(int w, int h)
{
	// Set new screen extents
	glViewport(0, 0, w, h);
	
	// Store new extents
	ww = w;
	hh = h;
}

// Tree traversal routine
void traverse(treenode *node)
{
	// Stop when at bottom of branch
	if (node == NULL)
	{
		return;
	}

	// Apply local transformation and render
	glPushMatrix();
	glMultMatrixf(node->m);
	glUseProgram(node->shaderProg);
	node->f();

	// Recurse vertically if possible (depth-first)
	if (node->child != NULL)
	{
		traverse(node->child);
	}

	// Remove local transformation and recurse horizontal
	glPopMatrix();
	if (node->sibling != NULL)
	{
		traverse(node->sibling);
	}
}

// Routine to load background billboard image
void load_image()
{
	background_image = SOIL_load_image(filename, &width, &height, &channels, SOIL_LOAD_AUTO);
}

void draw_background() 
{
	// Disable blending, and depth test for background image
	glDisable(GL_BLEND);
	glDepthMask(GL_FALSE);

	// Set 2D projection for image
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw image (inverting)
	glRasterPos2f(0.0f, 0.0f);
	glPixelZoom(1.0, 1.0f);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, background_image);

	// Reset zoom factors
	glPixelZoom(1.0f, 1.0f);

	// Reenable lighting, blending, and depth test
	glEnable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

// Routine to load textures using SOIL
bool load_textures()
{
	for (int i = 0; i < NO_TEXTURES; i++)
	{
		// TODO: Load images
		tex_ids[i] = SOIL_load_OGL_texture(texture_files[i], SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

		// Set texture properties if successfully loaded
		if (tex_ids[i] != 0)
		{
			// TODO: Set scaling filters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

			// TODO: Set wrapping modes
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		// Otherwise texture failed to load
		else
		{
			return false;
		}
	}
	return true;
}

// Routine to create model tree
void create_scene_graph()
{
	// creating head node
	head.texture = NO_TEXTURES;
	head.child = NULL;
	head.material = brass;
	head.sibling = &left_shoulder;
	head.shaderProg = lightShaderProg;
	head.f = draw_head;
	update_head();

	// creating torso node
	torso.texture = SHIRT;
	torso.material = brass;
	torso.sibling = &box;
	torso.child = &head;
	torso.shaderProg = textureShaderProg;
	torso.f = draw_torso;
	update_torso();

	// creating shoulder nodes
		// left shoulder
	left_shoulder.texture = NO_TEXTURES;
	left_shoulder.material = red_plastic;
	left_shoulder.sibling = &right_shoulder;
	left_shoulder.child = &upper_left_arm;
	left_shoulder.shaderProg = lightShaderProg;
	left_shoulder.f = draw_shoulder;
	update_left_shoulder();
		
	// right shoulder
	right_shoulder.texture = NO_TEXTURES;
	right_shoulder.material = red_plastic;
	right_shoulder.sibling = &upper_right_leg;
	right_shoulder.child = &upper_right_arm;
	right_shoulder.shaderProg = lightShaderProg;
	right_shoulder.f = draw_shoulder;
	update_right_shoulder();

	// creating arm nodes

		// upper arm
		upper_left_arm.texture = NO_TEXTURES;
		upper_left_arm.child = &lower_left_arm;
		upper_left_arm.material = blue;
		upper_left_arm.sibling = NULL;
		upper_left_arm.shaderProg = lightShaderProg;
		upper_left_arm.f = draw_upper_arm;
		update_upper_right_arm();

		upper_right_arm.texture = NO_TEXTURES;
		upper_right_arm.child = &lower_right_arm;
		upper_right_arm.material = blue;
		upper_right_arm.sibling = NULL;
		upper_right_arm.shaderProg = lightShaderProg;
		upper_right_arm.f = draw_upper_arm;
		update_upper_left_arm();


		// lower arm
		lower_left_arm.texture = NO_TEXTURES;
		lower_left_arm.child = NULL;
		lower_left_arm.material = brass;
		lower_left_arm.sibling = NULL;
		lower_left_arm.shaderProg = lightShaderProg;
		lower_left_arm.f = draw_lower_arm;
		update_lower_left_arm();

		lower_right_arm.texture = NO_TEXTURES;
		lower_right_arm.child = NULL;
		lower_right_arm.material = brass;
		lower_right_arm.sibling = NULL;
		lower_right_arm.shaderProg = lightShaderProg;
		lower_right_arm.f = draw_lower_arm;
		update_lower_right_arm();


	// creating leg nodes

		// upper leg
		upper_left_leg.texture = NO_TEXTURES;
		upper_left_leg.child = &lower_left_leg;
		upper_left_leg.material = blue;
		upper_left_leg.sibling = NULL;
		upper_left_leg.shaderProg = lightShaderProg;
		upper_left_leg.f = draw_upper_leg;
		update_upper_left_leg();

		upper_right_leg.texture = NO_TEXTURES;
		upper_right_leg.child = &lower_right_leg;
		upper_right_leg.material = blue;
		upper_right_leg.sibling = &upper_left_leg;
		upper_right_leg.shaderProg = lightShaderProg;
		upper_right_leg.f = draw_upper_leg;
		update_upper_right_leg();

		// lower leg
		lower_left_leg.texture = NO_TEXTURES;
		lower_left_leg.child = NULL;
		lower_left_leg.material = brass;
		lower_left_leg.sibling = NULL;
		lower_left_leg.shaderProg = lightShaderProg;
		lower_left_leg.f = draw_lower_leg;
		update_lower_left_leg();

		lower_right_leg.texture = NO_TEXTURES;
		lower_right_leg.child = NULL;
		lower_right_leg.material = brass;
		lower_right_leg.sibling = NULL;
		lower_right_leg.shaderProg = lightShaderProg;
		lower_right_leg.f = draw_lower_leg;
		update_lower_right_leg();


		// creating box node
		box.texture = NO_TEXTURES;
		box.child = &teapot;
		box.material = transparent_lime;
		box.sibling = NULL;
		box.shaderProg = lightShaderProg;
		box.f = draw_box;
		update_box();
	
		//teapot
		teapot.texture = NO_TEXTURES;
		teapot.child = teapot.sibling = NULL;
		teapot.material = gray;
		teapot.shaderProg = lightShaderProg;
		teapot.f = draw_teapot;
		update_teapot();
}

// Routine to draw textured cube
void texturecube()
{
	// Top face
	texquad(cube[4], cube[7], cube[6], cube[5], cube_tex[0], cube_tex[1], cube_tex[2], cube_tex[3]);

	// Bottom face
	texquad(cube[0], cube[1], cube[2], cube[3], cube_tex[4], cube_tex[5], cube_tex[6], cube_tex[7]);

	// Left face
	texquad(cube[2], cube[6], cube[7], cube[3], cube_tex[8], cube_tex[9], cube_tex[10], cube_tex[11]);

	// Right face
	texquad(cube[0], cube[4], cube[5], cube[1], cube_tex[12], cube_tex[13], cube_tex[14], cube_tex[15]);

	// Front face
	texquad(cube[1], cube[5], cube[6], cube[2], cube_tex[16], cube_tex[17], cube_tex[18], cube_tex[19]);

	// Back face
	texquad(cube[0], cube[3], cube[7], cube[4], cube_tex[20], cube_tex[21], cube_tex[22], cube_tex[23]); 

}

// Routine to draw quadrilateral face
void texquad(GLfloat v1[], GLfloat v2[], GLfloat v3[], GLfloat v4[], GLfloat t1[], GLfloat t2[], GLfloat t3[], GLfloat t4[])
{
	// Draw face 
	glBegin(GL_POLYGON);
		glTexCoord2fv(t1);
			glVertex3fv(v1);
		glTexCoord2fv(t2);
			glVertex3fv(v2);
		glTexCoord2fv(t3);
			glVertex3fv(v3);
		glTexCoord2fv(t4);
			glVertex3fv(v4);
	glEnd();
}

// function to draw torso
void draw_torso() 
{
	glUniform1i(texSampler, 0);
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, tex_ids[SHIRT]);
		glDisable(GL_BLEND);
		glScalef(TORSO_WIDTH, TORSO_HEIGHT, TORSO_DEPTH);
		texturecube();
	glPopMatrix();
}

// function to draw head
void draw_head()
{
	// head
	glUniform1i(numLights_param, numLights);
	glPushMatrix();
		set_material(GL_FRONT_AND_BACK, &head.material);
		glScalef(HEAD_XSCALE, HEAD_YSCALE, HEAD_ZSCALE);
		glutSolidSphere(HEAD_RADIUS, 50, 50);
	glPopMatrix();

	// ears
	glPushMatrix();
		set_material(GL_FRONT_AND_BACK, &gray);
		glTranslatef(0, 0, -HEAD_ZSCALE * 2.5);
		gluCylinder(quadric, EAR_RADIUS, EAR_RADIUS, EAR_HEIGHT, 50, 50);
	glPopMatrix();

	glPushMatrix();
		set_material(GL_FRONT_AND_BACK, &gray);
		glTranslatef(0, 0, HEAD_ZSCALE * 1.25);
		gluCylinder(quadric, EAR_RADIUS, EAR_RADIUS, EAR_HEIGHT, 50, 50);
	glPopMatrix();

}

// function to draw shoulders
void draw_shoulder()
{
	glPushMatrix();
		set_material(GL_FRONT_AND_BACK, &left_shoulder.material);
		glScalef(SHOULDER_JOINT_SCALE, SHOULDER_JOINT_SCALE, SHOULDER_JOINT_SCALE);
		glutSolidSphere(JOINT_RADIUS, JOINT_SLICES, JOINT_STACKS);
	glPopMatrix();
}


// function to draw lower arm (includes joint)
void draw_lower_arm()
{
	// lower arm
	glPushMatrix();
		set_material(GL_FRONT_AND_BACK, &lower_left_arm.material);
		glTranslatef(0, 0, 0);
		glScalef(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT, LOWER_ARM_DEPTH);
		glutSolidCube(1.0);
	glPopMatrix();

	// elbow joint
	glPushMatrix();
		set_material(GL_FRONT_AND_BACK, &lime);
		glTranslatef(0, TORSO_HEIGHT/3, 0);
		glScalef(ELBOW_JOINT_SCALE, ELBOW_JOINT_SCALE, ELBOW_JOINT_SCALE);
		glutSolidSphere(JOINT_RADIUS, JOINT_SLICES, JOINT_STACKS);
	glPopMatrix();
}

// function to draw upper arm (includes shoulder joint)
void draw_upper_arm()
{
	glPushMatrix();
		set_material(GL_FRONT_AND_BACK, &upper_left_arm.material);
		glScalef(UPPER_ARM_WIDTH, UPPER_ARM_HEIGHT, UPPER_ARM_DEPTH);
		glutSolidCube(1.0);
	glPopMatrix();

}

// function to draw lower leg
void draw_lower_leg()
{
	glPushMatrix();
		set_material(GL_FRONT_AND_BACK, &lower_left_leg.material);
		glTranslatef(0, 0, 0);
		glScalef(LOWER_ARM_WIDTH, LOWER_ARM_HEIGHT, LOWER_ARM_DEPTH);
		glutSolidCube(1.0);
	glPopMatrix();
}

// function to draw upper leg
void draw_upper_leg()
{
	glPushMatrix();
		set_material(GL_FRONT_AND_BACK, &upper_left_leg.material);
		glTranslatef(0, 0, 0.0);
		glScalef(UPPER_LEG_WIDTH, UPPER_LEG_HEIGHT, UPPER_LEG_DEPTH);
		glutSolidCube(1.0);
	glPopMatrix();
}

// function to draw box
void draw_box() 
{
	// enable alpha blending for box
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// transparent box
	glPushMatrix();
		set_material(GL_FRONT_AND_BACK, &box.material);
		glDepthMask(GL_FALSE);
		glScalef(1, 0.7, 1);
		glutSolidCube(BOX_SIZE);
		glDepthMask(GL_TRUE);
	glPopMatrix();

	// disable blending - we only want the box to be translucent
	glDisable(GL_BLEND);
}

void draw_teapot() 
{
	// teapot inside box
	glPushMatrix();
		set_material(GL_FRONT_AND_BACK, &teapot.material);
		glutSolidTeapot(TEAPOT_SIZE);
	glPopMatrix();
}

// END OF DRAWING FUNCTIONS - BEGIN UPDATE FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// function to update head movements
void update_head()
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(0, HEAD_YDIST +explode_dist, 0);
		glGetFloatv(GL_MODELVIEW_MATRIX, head.m);
	glPopMatrix();
}

// function to update torso movements
void update_torso()
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(0, TORSO_YDIST, 0);
		glGetFloatv(GL_MODELVIEW_MATRIX, torso.m);
	glPopMatrix();
}

// function to update shoulders
void update_left_shoulder() 
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(0,  TORSO_HEIGHT-1, -TORSO_WIDTH*2.5 - explode_dist);
		glRotatef(left_shoulder_theta, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, left_shoulder.m);
	glPopMatrix();
}


void update_right_shoulder() 
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(0, TORSO_HEIGHT-1, TORSO_WIDTH * 2.5 + explode_dist);
		glRotatef(right_shoulder_theta, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, right_shoulder.m);
	glPopMatrix();
}

// functions to update limbs
	
	// arms
void update_upper_left_arm()
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(0, -UPPER_ARM_HEIGHT+1, -explode_dist);
		glRotatef(upper_left_arm_theta, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, upper_left_arm.m);
	glPopMatrix();
}

void update_upper_right_arm()
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(0, -UPPER_ARM_HEIGHT+1, explode_dist);
		glRotatef(upper_right_arm_theta, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, upper_right_arm.m);
	glPopMatrix();
}

void update_lower_left_arm()
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(0, -LOWER_ARM_HEIGHT-1 + explode_dist, 0);
		glRotatef(lower_left_arm_theta, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, lower_left_arm.m);
	glPopMatrix();
}

void update_lower_right_arm()
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(0, -LOWER_ARM_HEIGHT-1 + explode_dist, 0);
		glRotatef(lower_right_arm_theta, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, lower_right_arm.m);
	glPopMatrix();
}


	// legs
void update_upper_left_leg()
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(0, -UPPER_LEG_HEIGHT*1.5, -TORSO_WIDTH - explode_dist);
		glRotatef(upper_left_leg_theta, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, upper_left_leg.m);
	glPopMatrix();
}

void update_upper_right_leg()
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(0, -UPPER_LEG_HEIGHT*1.5 , TORSO_WIDTH + explode_dist);
		glRotatef(upper_right_leg_theta, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, upper_right_leg.m);
	glPopMatrix();
}

void update_lower_left_leg() 
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(0, (-TORSO_HEIGHT / 1.5) + 0.35, -explode_dist);
		glRotatef(lower_left_leg_theta, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, lower_left_leg.m);
	glPopMatrix();
}

void update_lower_right_leg()
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(0, (-TORSO_HEIGHT / 1.5) + 0.35, explode_dist);
		glRotatef(lower_right_leg_theta, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, lower_right_leg.m);
	glPopMatrix();
}

// function to update box
void update_box()
{
	glPushMatrix();
		glLoadIdentity();
		glTranslatef(TEAPOT_X, TEAPOT_Y+explode_dist, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, box.m);
	glPopMatrix();
}

// update teapot
void update_teapot()
{
	glPushMatrix();
		glLoadIdentity();
		glRotatef(teapot_theta, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, teapot.m);
	glPopMatrix();
}