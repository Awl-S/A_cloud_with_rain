#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>

/* Global variables */
char title[] = "3D Shapes with animation";
GLfloat anglePyramid = 0.0f;  // Rotational angle for pyramid [NEW]
GLfloat angleCube = 0.0f;     // Rotational angle for cube [NEW]
int refreshMills = 15;        // refresh interval in milliseconds [NEW]

/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void drawCloudSphere(float radius, int numSlices, int numStacks, float posX, float posY, float posZ) {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glutSolidSphere(radius, numSlices, numStacks);
    glPopMatrix();
}

void drawRaindrop(float posX, float posY, float posZ, float length, float thickness) {
    glBegin(GL_LINES);
    glVertex3f(posX, posY, posZ);
    glVertex3f(posX, posY - length, posZ);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -7.0f);
    glRotatef(angleCube, 0.0f, 1.0f, 0.0f);

    // Облака (несколько сфер)
    glColor3f(0.76f, 0.85f, 0.91f); // Светло-голубой цвет
    drawCloudSphere(0.5f, 20, 20, 0.0f, 0.0f, 0.0f);      // Центральная сфера
    drawCloudSphere(0.4f, 20, 20, -0.6f, 0.2f, 0.0f);    // Левая сфера
    drawCloudSphere(0.4f, 20, 20, 0.6f, 0.2f, 0.0f);     // Правая сфера
    drawCloudSphere(0.4f, 20, 20, 0.0f, 0.2f, 0.5f);     // Верхняя сфера
    drawCloudSphere(0.4f, 20, 20, 0.0f, 0.2f, -0.5f);    // Нижняя сфера
    drawCloudSphere(0.3f, 20, 20, -0.8f, -0.1f, 0.2f);   // Облако слева
    drawCloudSphere(0.35f, 20, 20, 0.8f, -0.15f, 0.4f);  // Облако справа
    drawCloudSphere(0.25f, 20, 20, -0.4f, 0.3f, -0.5f);  // Облако сверху
    drawCloudSphere(0.3f, 20, 20, 0.5f, 0.1f, 0.3f);     // Облако внизу
    drawCloudSphere(0.35f, 20, 20, -0.2f, -0.15f, -0.4f);// Облако слева внизу

    // Raindrops
    glColor3f(0.0f, 0.0f, 1.0f); // Blue color for raindrops

    float raindropLength = 0.1f;
    float raindropThickness = 0.01f; // Thickness of the raindrop

    for (int i = 0; i < 100; i++) {
        float cloudPosX = 0.0f;
        float cloudPosY = -0.5f;
        float cloudPosZ = 0.0f;
        float cloudRadius = 0.5f;

        // Рассчитываем ограничивающий прямоугольник облака
        float minX = cloudPosX - cloudRadius;
        float maxX = cloudPosX + cloudRadius;
        float minY = cloudPosY - cloudRadius;
        float maxY = cloudPosY + cloudRadius;
        float minZ = cloudPosZ - cloudRadius;
        float maxZ = cloudPosZ + cloudRadius;

        // Генерируем случайные позиции для капель дождя в пределах ограничивающего прямоугольника
        float posX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxX - minX) + minX;
        float posY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (maxY - minY) + minY;
        float posZ = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (-minZ + maxZ) - maxZ;  // Изменено для сдвига капель назад

        // Двигаем капли дождя вниз до конца экрана
        while (posY > -20.0f) {
            drawRaindrop(posX, posY, posZ, raindropLength, raindropThickness);
            posY -= 0.6f; // Скорость падения капли дождя
        }    }

    glutSwapBuffers();

    angleCube += 0.2f;
}

/* Called back when timer expired [NEW] */
void timer(int value) {
    glutPostRedisplay();      // Post re-paint request to activate display()
    glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
    glutInitWindowSize(640, 480);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow(title);          // Create window with the given title
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);       // Register callback handler for window re-size event
    initGL();                       // Our own OpenGL initialization
    glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
}