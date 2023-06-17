#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Oyun değişkenleri
GLfloat colors[][3] = {{1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 0.5, 0.0}, {0.5, 0.0, 0.5}};
GLfloat question_color[3];
int score = 0;
int timeLeft = 30;
int previous_x = 0;
int previous_y = 0;
char player_name[100];
int shape_choice;

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Beyaz arka plan
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
}

void draw_text(float x, float y, char *text) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *text);
        text++;
    }
}

void generate_question() {
    int index = rand() % 6;
    question_color[0] = colors[index][0];
    question_color[1] = colors[index][1];
    question_color[2] = colors[index][2];
}

void draw_question() {
    glColor3fv(question_color);

    // Rastgele konum üret
    int x = rand() % 300 + 100;
    int y = rand() % 300 + 100;

    if (shape_choice == 0) { // Daire
        glBegin(GL_POLYGON);
        int num_segments = 50;
        for (int i = 0; i < num_segments; i++) {
            float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
            float dx = 50 * cosf(theta);
            float dy = 50 * sinf(theta);
            glVertex2f(x + dx, y + dy);
        }
        glEnd();
    } else if (shape_choice == 1) { // Kare
        glBegin(GL_QUADS);
        glVertex2i(x, y);
        glVertex2i(x, y + 100);
        glVertex2i(x + 100, y + 100);
        glVertex2i(x + 100, y);
        glEnd();
    }
}

void printScore() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);

    // Arkaplanı çizdir
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_QUADS);
    glVertex2i(100, 200);
    glVertex2i(400, 200);
    glVertex2i(400, 300);
    glVertex2i(100, 300);
    glEnd();

    // Metni çizdir
    glColor3f(1.0, 1.0, 1.0);
    char score_str[30];
    sprintf(score_str, "Oyun Bitti! Toplam Skor: %d", score);

    int text_width = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)score_str);
    int x = (500 - text_width) / 2;
    int y = 250;

    draw_text(x, y, score_str);

    glFlush();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Soruyu çizdir
    draw_question();

    // Skoru ekrana yazdir
    glColor3f(0.0, 0.0, 0.0);
    char score_str[20];
    sprintf(score_str, "Score: %d", score);
    draw_text(10.0, 10.0, score_str);

    // Süreyi ekrana yazdir
    char time_str[20];
    sprintf(time_str, "Time: %d", timeLeft);
    draw_text(400.0, 10.0, time_str);

    glFlush();
}

void timer(int value) {
    if (timeLeft > 0) {
        timeLeft--;
        glutPostRedisplay();
        glutTimerFunc(1000, timer, 0);
    } else {
        glutDisplayFunc(printScore);
        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Mouse týklamasý iþle
        GLint viewport[4];
        GLubyte pixel[3];

        glGetIntegerv(GL_VIEWPORT, viewport);
        glReadPixels(x, viewport[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

        if (pixel[0] == (GLubyte)(question_color[0] * 255) &&
            pixel[1] == (GLubyte)(question_color[1] * 255) &&
            pixel[2] == (GLubyte)(question_color[2] * 255)) {
            score++;
        }

        generate_question();
        glutPostRedisplay();
    }
}

void getPlayerName() {
    printf("Oyuncu Adini Giriniz: ");
    scanf("%s", player_name);
}

void getShapeChoice() {
    printf("Hangi sekillerin cikmasini istersiniz\n");
    printf("0 - Daire\n");
    printf("1 - Kare\n");
    printf("Seçiminizi yapin (0 veya 1): ");
    scanf("%d", &shape_choice);
}

int main(int argc, char **argv) {
    srand(time(NULL));

    getPlayerName();
    getShapeChoice();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Renk tiklama oyunu");

    init();
    generate_question();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutTimerFunc(1000, timer, 0);

    glutMainLoop();

    return 0;
}
