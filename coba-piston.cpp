//#include "windows.h"
#include "GL/glut.h"
#include "stdio.h"
#include "math.h"

float R_Z=0.0f, R_X=0.0f, R_Y=0.0f;
float T_Z=-16.0f, T_X=0.0f, T_Y=0.0f;
float view_rotx = 90.0f, view_roty = 180.0f;
int oldMouseX, oldMouseY;
float sud = 0;
float sudlagi = 0.1f;
float suds = 0;
float sudslagi = 0.1f;

bool berputar = false;

float Cx = 1, Cy = 1.10, Cz = 0.5;
float Lx = 1, Ly = 1, Lz = 0;
float Tx = 0, Ty = 1, Tz = 0;

float sudut_x = 0.0f, sudut_y = 0.0f, sudut_z = 0.0f;
float sudut_x2 = 0.0f, sudut_y2 = 0.0f, sudut_z2 = 0.0f;

float angle_depanBelakang = 0.0f;
float angle_depanBelakang2 = 0.0f;
float angle_samping = 0.0f;
float angle_samping2 = 0.0f;
float angle_vertikal = 0.0f;
float angle_vertikal2 = 0.0f;

float linear[] = {0.0f, 0.0f, -1.0f}; // Deklarasi awal vektor untuk maju
float lateral[] = {-1.0f, 0.0f, 0.0f}; // Deklarasi awal vektor untuk gerakan ke kanan
float vertical[] = {0.0f, 1.0f, 0.0f}; // Deklarasi awal vektor untuk gerakan naik

float toRadians(float angle){
    return angle * M_PI / 180;
}

class Vector{
    public: float x, y, z;

    void set_values (float startX, float startY, float startZ){
        x = startX;
        y = startY;
        z = startZ;
    }

    void vectorRotation(Vector refs, float angle){
        Vector temp = refs;
        float magnitude = sqrt(pow(temp.x, 2) + pow(temp.y, 2) + pow(temp.z, 2));
        temp.x = temp.x / magnitude;
        temp.y = temp.y / magnitude;
        temp.z = temp.z / magnitude;
        float dot_product = (x * temp.x)+(y * temp.y)+(z * temp.z);
        float cross_product_x = (y * temp.z) - (temp.z * z);
        float cross_product_y = -((x * temp.z) - (z * temp.x));
        float cross_product_z = (x * temp.y) - (y * temp.x);
        float last_factor_rodrigues = 1.0f - cos(toRadians(fmod(angle, 360.0f)));
        x = (x * cos(toRadians(fmod(angle, 360.0f)))) + (cross_product_x * sin(toRadians(fmod(angle, 360.0f)))) + (dot_product * last_factor_rodrigues * x);
        y = (y * cos(toRadians(fmod(angle, 360.0f)))) + (cross_product_y * sin(toRadians(fmod(angle, 360.0f)))) + (dot_product * last_factor_rodrigues * y);
        z = (z * cos(toRadians(fmod(angle, 360.0f)))) + (cross_product_z * sin(toRadians(fmod(angle, 360.0f)))) + (dot_product * last_factor_rodrigues * z);
    }
};
Vector depanBelakang, samping, vertikal;

void initGL()
{
    glShadeModel(GL_FLAT);

    depanBelakang.set_values(0.0f, 0.0f, -1.0f);
    samping.set_values(1.0f, 0.0f, 0.0f);
    vertikal.set_values(0.0f, 1.0f, 0.0f);

    float ambient[] = {1.0f,1.0f,1.0f,1.0f};
    float diffuse[] = {1.0f,1.0f,1.0f,1.0f};
    float specular[] = {0.2f,1.0f,0.2f,1.0f};
    float position[] = {20.0f,30.0f,20.0f,0.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

    // float mambient[] ={0.73f, 0.76f, 0.76f, 0.55f};
    float mambient[] ={0.294f, 0.294f, 0.294f, 1.0f };
    float mdiffuse[] ={0.46f, 0.46f, 0.46f, 0.55f };
    float mspecular[] ={0.662f, 0.662f, 0.662f, 0.55f };
    float mshine =76.8f;
	glMaterialfv(GL_FRONT,GL_AMBIENT,mambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mdiffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mspecular);
	glMaterialf (GL_FRONT,GL_SHININESS,mshine);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}

void ConnectingRodShaft()
{
        GLUquadric *q = gluNewQuadric();
        glPushMatrix();
        glRotatef(90, 0.0f, 1.0f, 0.0f);
        glRotatef(45, 0.0f, 0.0f, 1.0f);
        glTranslatef(0.0f, 0.0f, 0.475f);
        gluCylinder(q, 0.2f, 0.4f, 2.25f, 4.0f, 4.0f);
        glPopMatrix();
}

void ConnectingRod()
{
    float BODY_LENGTH=0.6f;
    float BODY_RADIUS=0.5f;
    float BODY_RADIUS2=0.4f;
    float BODY_RADIUS3=0.8f;
    float BODY_RADIUS4=1.0f;
    int SLICES=120;
    int STACKS=120;
        GLUquadric *q = gluNewQuadric();
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.3f);
        ConnectingRodShaft();
        glPopMatrix();
        gluDisk(q, 0.4f, BODY_RADIUS, SLICES, STACKS);
        gluCylinder(q, BODY_RADIUS, BODY_RADIUS, 0.6f, SLICES, STACKS);
        gluCylinder(q, BODY_RADIUS2, BODY_RADIUS2, 0.6f, SLICES, STACKS);
        glTranslatef(0.0f, 0.0f, 0.6f);
        gluDisk(q, 0.4f, BODY_RADIUS, SLICES, STACKS);
        glTranslatef(3.6f, 0.0f, -0.6f);
        gluDisk(q, BODY_RADIUS3, BODY_RADIUS4, SLICES, STACKS);
        gluCylinder(q, BODY_RADIUS4, BODY_RADIUS4, 0.6f, SLICES, STACKS);
        gluCylinder(q, BODY_RADIUS3, BODY_RADIUS3, 0.6f, SLICES, STACKS);
        glTranslatef(0.0f, 0.0f, 0.6f);
        gluDisk(q, BODY_RADIUS3, BODY_RADIUS4, SLICES, STACKS);

}

void ygMuter() {
    float BODY_RADIUS= 1.0f;
	float BODY_LENGTH= 1.0f;
	int SLICES=30;
	int STACKS=30;
	GLUquadric *q = gluNewQuadric();
	gluCylinder(q, BODY_RADIUS, BODY_RADIUS, BODY_LENGTH, SLICES, STACKS);
	gluDisk(q, 0.0f, BODY_RADIUS, SLICES, STACKS); //lingkaran untuk tutup atas
	glTranslatef(0.0f, 0.0f, BODY_LENGTH);
	gluDisk(q, 0.0f, BODY_RADIUS, SLICES, STACKS); //lingkaran untuk tutup bawah
}

void nyambunginPemutar(){
    float BODY_RADIUS= 0.1f;
	float BODY_LENGTH= 1.0f;
	int SLICES=30;
	int STACKS=30;
	GLUquadric *q = gluNewQuadric();
	gluCylinder(q, BODY_RADIUS, BODY_RADIUS, BODY_LENGTH, SLICES, STACKS);
	gluDisk(q, 0.0f, BODY_RADIUS, SLICES, STACKS); //lingkaran untuk tutup atas
	glTranslatef(0.0f, 0.0f, BODY_LENGTH);
	gluDisk(q, 0.0f, BODY_RADIUS, SLICES, STACKS); //lingkaran untuk tutup bawah
}

void Piston()
    {
    float BODY_LENGTH=2.4f;
    float BODY_RADIUS=1.2f;
    double clip_plane[]= {0.0,0.0,1.0,0.0};
    int SLICES=36;
    int STACKS=36;
        GLUquadric *q = gluNewQuadric();
        gluCylinder(q, BODY_RADIUS, BODY_RADIUS, BODY_LENGTH, SLICES, STACKS);
        gluDisk(q, 1.0f, BODY_RADIUS, SLICES, STACKS);
        glClipPlane(GL_CLIP_PLANE0,clip_plane);
        glEnable(GL_CLIP_PLANE0);
        gluSphere(q, 1.0f, SLICES, STACKS);
        gluDeleteQuadric(q);
        glDisable(GL_CLIP_PLANE0);
    }

void vectorMovement(float toMove[], float magnitude, float direction){
    float speedX = toMove[0]*magnitude*direction;
    float speedY = toMove[1]*magnitude*direction;
    float speedZ = toMove[2]*magnitude*direction;
    Cx += speedX;
    Cy += speedY;
    Cz += speedZ;
    Lx += speedX;
    Ly += speedY;
    Lz += speedZ;
}

void cameraRotation(Vector refer, double angle){
    float M = sqrt(pow(refer.x, 2) + pow(refer.y, 2) + pow(refer.z, 2));
    float Up_x1 = refer.x / M;
    float Up_y1 = refer.y / M;
    float Up_z1 = refer.z / M;
    float VLx = Lx - Cx;
    float VLy = Ly - Cy;
    float VLz = Lz - Cz;
    float dot_product = (VLx * Up_x1) + (VLy * Up_y1) + (VLz * Up_z1);
    float cross_product_x = (Up_y1 * VLz) - (VLy * Up_z1);
    float cross_product_y = -((Up_x1 * VLz) - (Up_z1 * VLx));
    float cross_product_z = (Up_x1 * VLy) - (Up_y1 * VLx);
    float last_factor_rodrigues = 1.0f - cos(toRadians(angle));
    float Lx1 = (VLx * cos(toRadians(angle))) + (cross_product_x * sin(toRadians(angle))) + (dot_product * last_factor_rodrigues * VLx);
    float Ly1 = (VLy * cos(toRadians(angle))) + (cross_product_y * sin(toRadians(angle))) + (dot_product * last_factor_rodrigues * VLy);
    float Lz1 = (VLz * cos(toRadians(angle))) + (cross_product_z * sin(toRadians(angle))) + (dot_product * last_factor_rodrigues * VLz);

    Lx = Lx1+Cx;
    Ly = Ly1+Cy;
    Lz = Lz1+Cz;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

        gluLookAt(Cx, Cy, Cz, // eye pos
                Lx, Ly, Lz, // Look At
                Tx, Ty, Tz);  // up

    glTranslatef(-T_X, T_Y, T_Z);
    glRotatef(view_rotx, 1.0f, 0.0f, 0.0f);
    glRotatef(view_roty, 0.0f, 0.0f, 1.0f);


    glRotatef(R_X, 1.0f, 0.0f, 0.0f);
    glRotatef(R_Y, 0.0f, 1.0f, 0.0f);
    glRotatef(R_Z, 0.0f, 0.0f, 1.0f);

    glPushMatrix();
    Piston();
    glPopMatrix();

    if(berputar){
        sud = sud + sudlagi;
        if(sud > 0.5){
            sudlagi =-0.1;
            suds = 0.5f;
        } else if(sud < -0.5){
            sudlagi = 0.1;
            suds = -0.5;
        }

    }
    //if(berputar){
    //    sud = suds + sudslagi;
    //    if(suds > 90){
    //        sudslagi =-0.1;
    //    } else if(suds < -90){
    //        sudslagi = 0.1;
    //    }
   // }

    glPushMatrix();
    glTranslatef(0.3f, 0.0f + suds, 1.4f + sud);
    glRotatef(90, 0.0f, -1.0f, 0.0f);
    //glRotatef(45 + suds, 0.0f, 0.0f, 1.0f);
    ConnectingRod();
    glPopMatrix();

    //glPushMatrix();
    //glTranslatef(2.5f, 0.0f, 5.5f);
    //glRotatef(90, 0.0f, -1.0f, 0.0f);
    //glRotatef(90 + sud, 0.0f, 0.0f, 1.0f);
    //ygMuter();
    //glTranslatef(-0.3f, 0.5f, 0.0f);
    //nyambunginPemutar();
    //glPopMatrix();

    //glPushMatrix();
    //glRotatef(90, 0.0f, 0.0f, -1.0f);
    //glTranslatef(-2.2f, 0.5f, 0.0f);
    //ConnectingRod();
    //glPopMatrix();


    glFlush();
    glutSwapBuffers();
}

void timer(int value)
{
	glutPostRedisplay();
    glutTimerFunc(15, timer, 0);
}

void reshape(GLsizei width, GLsizei height)
{
if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(30, 6, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 1.0f, 20.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouseControl(int button, int state, int x, int y){
    oldMouseX = x;
    oldMouseY = y;
}

void mouseMotion(int x, int y){
    int getX = x;
    int getY = y;
    float thetaY = 360.0f*(getX - oldMouseX)/640;
    float thetaX = 360.0f*(getY - oldMouseY)/480;
    oldMouseX = getX;
    oldMouseY = getY;
    view_rotx += thetaX;
    view_roty += thetaY;
}

void keyFunction(unsigned char key, int x, int y){
switch(key){
case 79: // Rotasi sumbu Z+ dengan tombol O
        R_Z = R_Z + 15.0f;
	break;
case 85: // Rotasi sumbu Z- dengan tombol U
        R_Z = R_Z - 15.0f;
	break;
case 73: // Rotasi sumbu Y+ dengan tombol I
        R_Y = R_Y + 15.0f;
	break;
case 75: // Rotasi sumbu Y- dengan tombol K
        R_Y = R_Y - 15.0f;
	break;
case 76: // Rotasi sumbu X+ dengan tombol L
        R_X = R_X + 15.0f;
	break;
case 74: // Rotasi sumbu X- dengan tombol J
        R_X = R_X - 15.0f;
	break;


case 81: // Translasi sumbu Z+ dengan tombol Q
        T_Z = T_Z + 0.5f;
	break;
case 69: // Translasi sumbu Z- dengan tombol E
        T_Z = T_Z - 0.5f;
	break;
case 87: // Translasi sumbu Y+ dengan tombol W
        T_Y = T_Y + 0.5f;
	break;
case 83: // Translasi sumbu Y- dengan tombol S
        T_Y = T_Y - 0.5f;
	break;
case 68: // Translasi sumbu X- dengan tombol D
        T_X = T_X - 0.5f;
	break;
case 65: // Translasi sumbu X+ dengan tombol A
        T_X = T_X + 0.5f;
	break;


// camera control
case 90: // Z
        angle_vertikal += 1.5f;
        samping.vectorRotation(vertikal, angle_vertikal - angle_vertikal2);
        //memutar vector sumbu z terhadap x (target, patokan)
        depanBelakang.vectorRotation(vertikal, angle_vertikal - angle_vertikal2);
        cameraRotation(vertikal, angle_vertikal - angle_vertikal2); // look at
        angle_vertikal2 = angle_vertikal;
    break;
case 88: // X
        angle_vertikal -= 1.5f;
        samping.vectorRotation(vertikal, angle_vertikal - angle_vertikal2);
        depanBelakang.vectorRotation(vertikal, angle_vertikal - angle_vertikal2);
        cameraRotation(vertikal, angle_vertikal - angle_vertikal2);
        angle_vertikal2 = angle_vertikal;
    break;
case 67: // C
        angle_samping += 1.5f;
        depanBelakang.vectorRotation(samping, angle_samping - angle_samping2);
        cameraRotation(samping, angle_samping - angle_samping2);
        angle_samping2 = angle_samping;
    break;
case 86: // V
        angle_samping -= 1.5f;
        depanBelakang.vectorRotation(samping, angle_samping - angle_samping2);
        cameraRotation(samping, angle_samping - angle_samping2);
        angle_samping2 = angle_samping;
    break;
case 66: // B
        angle_depanBelakang += 1.5f;
        vertikal.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
        cameraRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
        angle_depanBelakang2 = angle_depanBelakang;
    break;
case 78: // N
        angle_depanBelakang -= 1.5f;
        vertikal.vectorRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
        cameraRotation(depanBelakang, angle_depanBelakang - angle_depanBelakang2);
        angle_depanBelakang2 = angle_depanBelakang;
    break;


case 32: // space (reset)
      /*  R_Z=0.0f, R_X=0.0f, R_Y=0.0f;
        T_Z=-16.0f, T_X=0.0f, T_Y=0.0f;

        view_rotx = 90.0f, view_roty = 180.0f;

        Cx = 1, Cy = 1.10, Cz = 0.5;
        Lx = 1, Ly = 1, Lz = 0;
        Tx = 0, Ty = 1, Tz = 0;

        angle_depanBelakang = 0.0f;
        angle_depanBelakang2 = 0.0f;
        angle_samping = 0.0f;
        angle_samping2 = 0.0f;
        angle_vertikal = 0.0f;
        angle_vertikal2 = 0.0f;

        depanBelakang.set_values(0.0f, 0.0f, -1.0f);
        samping.set_values(1.0f, 0.0f, 0.0f);
        vertikal.set_values(0.0f, 1.0f, 0.0f);
        */
        berputar = !berputar;
    break;

    //simulasi
case 71: //G
    //while(true){
    //    T_X=-0.1f;
    //}
    R_Z = R_Z - 90.0f;
    T_X = T_X - 0.5f;
    T_Y = T_Y - 1.0f;
    R_Y = R_Y - 15.0f;
    T_X = T_X + 0.5f;
    T_Y = T_Y + 1.0f;
    break;

case 82: //R untuk reset jika butuh
    R_Z=0.0f;
    R_X=0.0f;
    R_Y=0.0f;
    T_Z=-16.0f;
    T_X=0.0f;
    T_Y=0.0f;
    view_rotx = 90.0f;
    view_roty = 180.0f;
    Cx = 1;
    Cy = 1.10;
    Cz = 0.5;
    Lx = 1;
    Ly = 1;
    Lz = 0;
    Tx = 0;
    Ty = 1;
    Tz = 0;

    sudut_x = 0.0f;
    sudut_y = 0.0f;
    sudut_z = 0.0f;
    sudut_x2 = 0.0f;
    sudut_y2 = 0.0f;
    sudut_z2 = 0.0f;

    angle_depanBelakang = 0.0f;
    angle_depanBelakang2 = 0.0f;
    angle_samping = 0.0f;
    angle_samping2 = 0.0f;
    angle_vertikal = 0.0f;
    angle_vertikal2 = 0.0f;
    break;

	}
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("3d-control");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyFunction);
    initGL();
    glutMouseFunc(mouseControl);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
