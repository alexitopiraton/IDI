// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
using namespace std;

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {

}

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}



/* ----------------------------------------------------------------------------------------------------------
 *                                        INITIALIZE
 * ----------------------------------------------------------------------------------------------------------
 */
glm::mat4 TGCarro = glm::mat4(1.f);
glm::mat4 View;
glm::vec3 colorFocusCamara = glm::vec3(0.8, 0.8, 0.8);


void MyGLWidget::initializeGL() {
    LL4GLWidget::initializeGL();

    colorambienteLoc = glGetUniformLocation (program->programId(), "luzAmbiente");
    colorFocCamLoc = glGetUniformLocation (program->programId(), "colorFocusCamara");
    colorFocEscLoc = glGetUniformLocation (program->programId(), "colorFocusEscena");
    posFocCamLoc = glGetUniformLocation (program->programId(), "posFocusCamara");
    posFocEscLoc1 = glGetUniformLocation (program->programId(), "posFocusEscena1");
    posFocEscLoc2 = glGetUniformLocation (program->programId(), "posFocusEscena2");

    connect(&timer, SIGNAL(timeout()), this, SLOT(animar()));

}

void MyGLWidget::iniMaterialTerra ()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.1,0,0);
  diff = glm::vec3(0.4,0,0);
  spec = glm::vec3(0.8,0.8,0.8);
  shin = 100;
}



/* ----------------------------------------------------------------------------------------------------------
 *                                        PAINTGL
 * ----------------------------------------------------------------------------------------------------------
 */



void MyGLWidget::enviaFocos() {
  glm::vec3 colorFocusEscena = glm::vec3(0.6, 0.6, 0);
  glm::vec3 luzAmbiente = glm::vec3(0.1, 0.1, 0.1);
  // posicion del foco en SCO (relativo a la camara)
  glm::vec4 posFocusCamara = glm::vec4(1,1,1, 1);


  glUniform3fv (colorambienteLoc, 1, &luzAmbiente[0]);
  glUniform3fv (colorFocCamLoc, 1, &colorFocusCamara[0]);
  glUniform3fv (colorFocEscLoc, 1, &colorFocusEscena[0]);
  glUniform4fv (posFocCamLoc, 1, &posFocusCamara[0]);
}

void MyGLWidget::enviaPosFocusEscena() {
  glm::vec4 posFocusEscena1 = glm::vec4(0.5, 0.5, -2.0, 1.0);
  glm::vec4 posFocusEscena2 = glm::vec4(-0.5, 0.5, -2.0, 1.0);

  posFocusEscena1 = View * TGCarro * posFocusEscena1;         // SCM (relativo al modelo del carro)
  posFocusEscena2 = View * TGCarro * posFocusEscena2;         // SCM (relativo al modelo del carro)
  glUniform4fv (posFocEscLoc1, 1, &posFocusEscena1[0]);
  glUniform4fv (posFocEscLoc2, 1, &posFocusEscena2[0]);
  update();

}

void MyGLWidget::paintGL ()
{

  enviaFocos();
  enviaPosFocusEscena();
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(apaga) {
    // FANTASMA
     glBindVertexArray (VAO_Fantasma);
     modelTransformFantasma ();
    glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);
  }

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // TRACK
  glBindVertexArray (VAO_Track);
  float grados = 0.f;
  for(int i = 0; i < 90; ++i) {
    modelTransformTrack (10,grados);
    grados += 4.f;
    glDrawArrays(GL_TRIANGLES, 0, track.faces().size()*3);
  }
  // MINE CART
  glBindVertexArray (VAO_MineCart);
  modelTransformMineCart ();
  glDrawArrays(GL_TRIANGLES, 0, mineCart.faces().size()*3);

  glBindVertexArray(0);
}



/* ----------------------------------------------------------------------------------------------------------
 *                                       MODEL TRANSFORM
 * ----------------------------------------------------------------------------------------------------------
 */



void MyGLWidget::modelTransformTrack (float radi, float rotY)
{
  glm::mat4 TG = glm::mat4(1.f);
  TG = glm::rotate(TG, float(glm::radians(rotY)), glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(radi,0,0));
  TG = glm::scale(TG, glm::vec3(escalaTrack, escalaTrack, escalaTrack));
  TG = glm::rotate(TG, glm::radians(90.f), glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseTrack);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

}

void MyGLWidget::modelTransformMineCart ()
{
  TGCarro = glm::mat4(1.f);
  TGCarro = glm::rotate(TGCarro, gradosCarro, glm::vec3(0,1,0));
  TGCarro = glm::translate(TGCarro, glm::vec3(10,0,0));
  TGCarro = glm::rotate(TGCarro, float(glm::radians(0.0)), glm::vec3(0,1,0));
  TGCarro = glm::scale(TGCarro, glm::vec3(escalaMineCart, escalaMineCart, escalaMineCart));
  TGCarro = glm::translate(TGCarro, -centreBaseMineCart);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TGCarro[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG = glm::mat4(1.f);  // Matriu de transformació
  TG = glm::scale(TG, glm::vec3(30/10));
  TG = glm::translate(TG, glm::vec3(-5,0,-5));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFantasma ()
{
  glm::mat4 TG = glm::mat4(1.0f);
  TG = glm::rotate(TG, gradosCarro, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(10,1,0));
  TG = glm::scale(TG, glm::vec3(3*escalaFantasma/4));
  TG = glm::rotate(TG, glm::radians(180.f), glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseFantasma);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}



/* ----------------------------------------------------------------------------------------------------------
 *                                          VIEW TRANSFORM
 * ----------------------------------------------------------------------------------------------------------
 */

void MyGLWidget::iniEscena () {
   //  VRP = (Pmax+Pmin)/glm::vec3(2.f);
     diametroEscena = glm::distance(Pmin,Pmax);
   //  radioEscena = float(diametroEscena/2);
   // OBS = glm::vec3(0, diametroEscena, 0) + VRP;
   // FOV = 2*glm::radians(asin(radioEscena/diametroEscena));
   // zNear = diametroEscena - radioEscena;
   // zFar = diametroEscena + radioEscena;

   centreEsc = (Pmax+Pmin)/glm::vec3(2.f);
   radiEsc = float(diametroEscena/2);
}

void MyGLWidget::viewTransform ()
{
   View = glm::mat4(1.f);
   // View = glm::lookAt(OBS, VRP, glm::vec3(0,0,-1));
   View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
   View = glm::rotate(View, -angleX, glm::vec3(1,0,0));
   View = glm::rotate(View, angleY, glm::vec3(0, 1, 0));
   View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}



/* ----------------------------------------------------------------------------------------------------------
 *                                          EVENTS
 * ----------------------------------------------------------------------------------------------------------
 */
void MyGLWidget::animar() {
  makeCurrent();
  gradosCarro += glm::radians(5.f);
  update();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX -= (e->y() - yClick) * M_PI / alt;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: {
    gradosCarro += glm::radians(5.f);

    break;
	}
  case Qt::Key_L: {
      apaga = not apaga;
      if(apaga) {
        glClearColor(0, 0, 0, 1.0);
        colorFocusCamara = glm::vec3(0.0);
      }
      else {
        glClearColor(0.5, 0.7, 1.0, 1.0);
        colorFocusCamara = glm::vec3(0.8, 0.8, 0.8);
      }
    break;
	}
  case Qt::Key_R: {
    angleX = glm::radians(-60.f);
    angleY = 0.f;
    break;
	}
  case Qt::Key_S: {
       if(not timerActivo) {
        timerActivo = true;
        timer.start(50);
      }
      else {
        timerActivo = false;
        timer.stop();
      }
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}
