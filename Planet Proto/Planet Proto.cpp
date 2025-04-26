#include <GL/glut.h>
#include <math.h>

// Rotation states
static double rotate_x = 0.0;   // Planet's rotation
static double orbital_angle = 0.0; // Angle for circular motion
static double PI = 3.14;

GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

void initGL() {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightPosition[] = { 18.0f, 0.0f, 0.5f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glEnable(GL_NORMALIZE);
}

void reshape(int width, int height) {
    if (height == 0)
        height = 1;
    float aspect = (float)width / (float)height;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspect, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void planet() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set up the camera
    gluLookAt(0, 1, 10,  // eye position
        0, 0, 0,   // center position
        0, 1, 0);  // up vector

    // Calculate orbital position of the planet
    double orbit_radius = 3.0;
    double x = orbit_radius * cos(orbital_angle);
    double z = orbit_radius * sin(orbital_angle);

    // Begin transformation for planet with the white belt
    glPushMatrix();
    glTranslatef(x, 0.0f, z); // Move the planet to its orbit position

    // rotate the planet on its own x-axis its changing the Y but if u made the X 1 and y 0 planet will start rotating upside down we dont want that now do we ?
    glRotatef(rotate_x, 0, 1, 0);

    // Draw the planet
    glColor3f(0.0f, 0.7f, 0.6f);
    glutSolidSphere(0.5, 40, 40);

    // --- Draw the belt around the planet ---
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

    glDisable(GL_LIGHTING); //its for the neon belt color (remove it if u dont want the neon effect) 
    glColor3f(1, 1, 1);

    // Rotate the coordinate system so the torus lies in the equator of the planet
    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    // Draw the belt:
    glutSolidTorus(0.02, 0.75, 30, 30); //first parameter = thickness of the belt, second para = the distance between the belt and planet (closer number to 1 = the more far away the belt is)
    glPopMatrix();
    glPopAttrib();
    glPopMatrix();

    glutSwapBuffers();
}

void updateOrbit(int value) {
    // Update orbit position
    orbital_angle += 0.01;
    if (orbital_angle > 2 * PI)
        orbital_angle -= 2 * PI;

    // Optionally, update planet rotation on its own axis
    rotate_x += 0.5;

    glutPostRedisplay();
    glutTimerFunc(16, updateOrbit, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D planet with belt");

    initGL();
    glutDisplayFunc(planet);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, updateOrbit, 0);

    glutMainLoop();
    return 0;
}
