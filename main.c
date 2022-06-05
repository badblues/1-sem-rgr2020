#include <windows.h>
#include <GL/gl.h>
#include <math.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);

void DisableOpenGL(HWND, HDC, HGLRC); \
float xt=0;
float rtt=1;
int rtt_flag=0;
int phase=0, phase_flag=0, opp_m=1;

void obj_head();
void obj_body();
void obj_leg(int l, int s_l);
void obj_foot(int f, int s_f);
void obj_arm(int p, int s_a);

void moving_phase(int direction);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "Moving Man",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1280,
                          756,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClearColor(0.7f, 0.7f, 1.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glPushMatrix();
            glScalef((float)1/640, (float)1/360, 0.0);
            obj_head();
            obj_arm(phase, -1);
            obj_leg(phase, 1);
            obj_leg(phase, -1);
            obj_foot(phase, 1);
            obj_foot(phase, -1);
            obj_body();
            obj_arm(phase, 1);

            glPopMatrix();
            SwapBuffers(hDC);

            //Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;

                case VK_LEFT:
                    if (rtt_flag==0){rtt=-rtt; rtt_flag=1; phase =0;}
                    xt-=10;
                    moving_phase(-1);

                    break;

                case VK_RIGHT:
                    if (rtt_flag==1){rtt=-rtt; rtt_flag=0; phase=0;}
                    xt+=10;
                    moving_phase(1);
                    break;
            }
        }
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

void moving_phase(int direction){
    if (direction==-1){
        if (phase==11){phase_flag=1;}
        if (phase==0){phase_flag=0;}
        if (phase_flag==0){phase++;}
        else {phase--;}
        if (phase==0){opp_m=-opp_m;}
    }
    else {
        if (phase==11){phase_flag=1;}
        if (phase==0){phase_flag=0;}
        if (phase_flag==0){phase++;}
        else {phase--;}
        if (phase==0){opp_m=-opp_m;}
    }
}

void obj_head(){

    glPushMatrix();
    glTranslatef(xt, 1.0, 0.0);
    float x,y;
    float cnt=64;
    float l=60; //radius
    float a=M_PI * 2/cnt;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.5, 0.0, 0.0);
    glVertex2f(0, 120); //Center
    for (int i=-1; i<cnt; i++){
        x=sin(a*i)*l;
        y=cos(a*i)*l+120;
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();

}

void obj_body(){

    glPushMatrix();
    glTranslatef(xt, 0.0, 0.0);
    glBegin(GL_QUADS);
    glColor3f(0.4, 0.0, 0.0);
    glVertex2f(rtt*(-40), 65);
    glVertex2f(rtt*(-40), -140);
    glVertex2f(rtt*(35), -140);
    glVertex2f(rtt*(35), 65);
    glEnd();
    glPopMatrix();

}

void obj_leg(int l, int s_l){

    glPushMatrix();
    l=l/3;
    switch (l){
        case 0:
            glTranslatef(xt, 0.0, 0.0);
            break;
        case 1:
            glTranslatef(xt-40*s_l*opp_m, 10.0, 0.0);
            glRotatef(s_l*opp_m*15, 0.0, 0.0, 1.0);
            break;
        case 2:
            glTranslatef(xt-80*s_l*opp_m, 0.0, 0.0);
            glRotatef(s_l*opp_m*30, 0.0, 0.0, 1.0);
            break;
        case 3:
            glTranslatef(xt-100*s_l*opp_m, -20.0, 0.0);
            glRotatef(s_l*opp_m*45, 0.0, 0.0, 1.0);
            break;
    }
    glBegin(GL_QUADS);
    glColor3f(1.0-s_l*0.1, 1.0-s_l*0.1, 1.0-s_l*0.1);
    glVertex2f(rtt*(-30), -140);
    glVertex2f(rtt*(-30), -360);
    glVertex2f(rtt*(25), -360);
    glVertex2f(rtt*(25), -140);
    glEnd();
    glPopMatrix();

}

void obj_arm(int p, int s_a){

    glPushMatrix();
    p=p/3;
    switch (p){
        case 0:
            glTranslatef(xt, 0.0, 0.0);
            break;
        case 1:
            glTranslatef(xt+8*s_a*opp_m, 3.0, 0.0);
            glRotatef(s_a*opp_m*20, 0.0, 0.0, 1.0);
            break;
        case 2:
            glTranslatef(xt+20*s_a*opp_m, 15, 0.0);
            glRotatef(s_a*opp_m*40, 0.0, 0.0, 1.0);
            break;
        case 3:
            glTranslatef(xt+40*s_a*opp_m, 20, 0.0);
            glRotatef(s_a*opp_m*60, 0.0, 0.0, 1.0);
            break;
    }
    glBegin(GL_QUADS);
    glColor3f(0.2, 0.0, 0.0);
    glVertex2f(s_a*rtt*(-25), 45);
    glVertex2f(s_a*rtt*(-25), -170);
    glVertex2f(s_a*rtt*(25), -170);
    glVertex2f(s_a*rtt*(25), 45);
    glEnd();
    glPopMatrix();

}

void obj_foot(int f, int s_f){

    glPushMatrix();
    f=f/3;
    switch (f){
        case 0:
            glTranslatef(xt, 0.0, 0.0);
            break;
        case 1:
            glTranslatef(xt-35*s_f*opp_m, -5, 0.0);
            glRotatef(s_f*opp_m*15, 0.0, 0.0, 1.0);
            break;
        case 2:
            glTranslatef(xt-80*s_f*opp_m, 0.0, 0.0);
            glRotatef(s_f*opp_m*30, 0.0, 0.0, 1.0);
            break;
        case 3:
            glTranslatef(xt-100*s_f, -20, 0.0);
            glRotatef(s_f*45, 0.0, 0.0, 1.0);
            break;
    }
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(rtt*(-30), -340);
    glVertex2f(rtt*(-30), -360);
    glVertex2f(rtt*(50), -360);
    glVertex2f(rtt*(50), -340);
    glEnd();
    glPopMatrix();
}