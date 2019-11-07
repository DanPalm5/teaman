// Node structure
struct treenode
{
	MaterialType material;
	GLfloat color[4];
	GLuint texture;
	void (*f)();
	GLfloat m[16];
	treenode* sibling;
	treenode* child;
	GLuint shaderProg;
};
