#include <parser.h>
#include <cgRender.h>
#include <shader_utils.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

static struct {
    float t;
    float dt;
    bool rotate;
    GLuint program;
    GLint attribute_coord2d;
    vtk_file* data;
    string vertex_shader;
    string fragment_shader;
} singleton_var;

static void setupGlobals()
{
    singleton_var.t = 0;
    singleton_var.dt = 0;
    singleton_var.rotate = GL_FALSE;
}

static bool init_resources(void)
{
    GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;
    GLuint vs, fs;
    
    if ((vs = shader_create(singleton_var.vertex_shader, GL_VERTEX_SHADER)) == 0)
        return false;
    
    if ((fs = shader_create(singleton_var.vertex_shader, GL_FRAGMENT_SHADER)) == 0)
        return false;
    
    singleton_var.program = glCreateProgram();
    glAttachShader(singleton_var.program,vs);
    glAttachShader(singleton_var.program,fs);
    glLinkProgram(singleton_var.program);
    glGetProgramiv(singleton_var.program,GL_LINK_STATUS, &link_ok);
    if (!link_ok)
    {
        cout << "glLinkProgram: ";
        print_log(singleton_var.program);
        return false;
    }
    
    const char* attribute_name = "coord2d";
    singleton_var.attribute_coord2d = glGetAttribLocation(singleton_var.program, attribute_name);
    if (singleton_var.attribute_coord2d == -1)
    {
        cout << "Could not bind attribute" << attribute_name << endl;
        return false;
    }

    return true;    
}

static void idle_func(void)
{
    singleton_var.t += singleton_var.dt;
    glutPostRedisplay();
}

static void render(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(singleton_var.program);
    glEnableVertexAttribArray(singleton_var.attribute_coord2d);
    
    GLfloat triangle_vertices[] =
    {
        0.0, 0.8,
        -0.8, -0.8,
        0.8, -0.8,
    };
    
    glVertexAttribPointer(
        singleton_var.attribute_coord2d,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        triangle_vertices
    );
    
    glDrawArrays(GL_TRIANGLES,0,3);
    glDisableVertexAttribArray(singleton_var.attribute_coord2d);
    
    glutSwapBuffers();
}

static void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
        case 'q' : case 'Q' :
            exit(EXIT_SUCCESS);
            break;
        case 'r' : case 'R' :
            singleton_var.rotate = GL_TRUE;
            break;
    }
}

static void free_resources(void)
{
    glDeleteProgram(singleton_var.program);
}


int main(int argc, char** argv)
{

    // Check input arguments
    // and print a usage on failure
    if (argc != 5)
    {
        cout << "Usage: " << argv[0] << "and then a .vtk file, "
            << "followed by  the path to the vertex shader and fragment_shader "
            << " lastly 1 (meaning Gourand rendered "
            << "or 2 (meaning texture mapped render)" << endl;
        exit(EXIT_FAILURE);
    }
    
    setupGlobals();
    
    singleton_var.data = parser::parser((string)read_file(argv[1])).get_vtk_file();
    singleton_var.vertex_shader = argv[2];
    singleton_var.fragment_shader = argv[3];
    

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(256,256);
    glutCreateWindow(argv[0]);

    glutIdleFunc(&idle_func);
    glutKeyboardFunc(keyboard);

    GLenum glew_status = glewInit();

    if (!GLEW_VERSION_2_0 || glew_status != GLEW_OK)
    {
        cout << "Error is " << glewGetErrorString(glew_status) << endl;
        exit(EXIT_FAILURE);
    }

    if (init_resources())
    {    
        glutDisplayFunc(render);
        glutMainLoop();
    }
    else
    {
        cout << "Failed to load resources" << endl;
        exit(EXIT_FAILURE);
    }

    free_resources();
    return EXIT_SUCCESS;
}