#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

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

MyGLWidget::~MyGLWidget()
{
}

// -------------------------------------------- INITIALIZE ---------------------------------------------

void MyGLWidget::initializeGL () {

  LL2GLWidget::initializeGL();
  connect(&timer, SIGNAL(timeout()), this, SLOT(animar()));
}

// --------------------------------------------- CARREGA SHADERS ----------------------------------------

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Identificador per als  atributs
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");

  // Identificadors dels uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc  = glGetUniformLocation (program->programId(), "Proj");
  viewLoc  = glGetUniformLocation (program->programId(), "View");
  colorUnif = glGetUniformLocation(program->programId(), "vecColores");
}




// ---------------------------------------------- CREAR BUFFERS ------------------------------------------

void MyGLWidget::creaBuffersTerra ()
{
  // VBO amb la posició dels vèrtexs
  glm::vec3 posTerra[4] = {
        glm::vec3(-10.0, 0.0, -10.0),
        glm::vec3(-10.0, 0.0,  10.0),
        glm::vec3( 10.0, 0.0, -10.0),
        glm::vec3( 10.0, 0.0,  10.0)
  };

  glm::vec3 c(0.65, 0.2, 0.05);
  glm::vec3 colTerra[4] = { c, c, c, c };

  // VAO
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  GLuint VBO_Terra[2];
  glGenBuffers(2, VBO_Terra);

  // geometria
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posTerra), posTerra, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // color
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colTerra), colTerra, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
}

void MyGLWidget::creaBuffersModels ()
{
  // Càrrega dels models
  models[ROAD].load("./models/RoadCurve.obj");
  models[PIPE].load("./models/GreenPipe.obj");
  models[CAR].load("./models/RacingCarRetro.obj");

  // Creació de VAOs i VBOs per pintar els models
  glGenVertexArrays(NUM_MODELS, &VAO_models[0]);

  float profunditatDesitjada[NUM_MODELS] = {2,1,2}; // nota: PIPE té alçada = profunditat

  for (int i = 0; i < NUM_MODELS; i++)
  {
	  // Calculem la capsa contenidora del model
	  calculaCapsaModel (models[i], escalaModels[i], profunditatDesitjada[i], centreBaseModels[i]);
      if (i == 1) HmaxEscena = escalaModels[PIPE] * float(3.0);

	  glBindVertexArray(VAO_models[i]);

	  GLuint VBO[2];
	  glGenBuffers(2, VBO);

	  // geometria
	  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3,
		   models[i].VBO_vertices(), GL_STATIC_DRAW);
	  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	  glEnableVertexAttribArray(vertexLoc);

	  // color
	  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*models[i].faces().size()*3*3,
		   models[i].VBO_matdiff(), GL_STATIC_DRAW);
	  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	  glEnableVertexAttribArray(colorLoc);
  }

  glBindVertexArray (0);
}

// ---------------------------------------------- PAINT ---------------------------------------------------

void MyGLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //uniform de los colores de los coches
  const glm::vec3 colorescoche[3] {glm::vec3(0,0,1), glm::vec3(0,1,0), glm::vec3(1,0,0)};

  // Car
  glBindVertexArray(VAO_models[CAR]);
  for(int i = 0; i < 3; i++) {
    if(i == 0) CarTransform(radioAzul, Yazul);
    else if(i == 1) CarTransform(radioVerde, Yverde);
    else CarTransform(radioRojo, Yrojo);
    glm::vec3 vecColores = colorescoche[i];
    glUniform3fv(colorUnif, 1, &vecColores[0]);
    glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);
  }
  glm::vec3 neutro = glm::vec3(1,1,1);
  glUniform3fv(colorUnif, 1, &neutro[0]);


  // Road
  glBindVertexArray (VAO_models[ROAD]);
  //el valor de la nueva escala será la escala actual de la carretera * la distancia entre el centro de la carretera y el eje X (en valor abs)
  float distEscala = 5;
  float angulo = 0.f;
  for(int i = 0; i < 4; i++) {
    glm::vec3 posicion(5.0, 0.01, -5.0);
    // i = 1 segundo cuadrante ; i = 2 tercer cuadrante; ... (si no se toca el angulo es del 1er cuadrante)
    if(i == 1) {
      angulo = -90.f;
      posicion = glm::vec3(5.0,0.01,5.0);
    }
    else if (i == 2) {
      angulo = 180.f;
      posicion = glm::vec3(-5.0,0.01,5.0);
    }
    else if (i == 3) {
      angulo = 90.f;
      posicion = glm::vec3(-5.0,0.01,-5.0);
    }
    RoadTransform(posicion,angulo, distEscala);
    glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);
  }

  // Pipe
  glBindVertexArray (VAO_models[PIPE]);
  PipeTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);

  // Terra
  glBindVertexArray (VAO_Terra);
  TerraTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

// ---------------------------------------------- TRANSFORMS -----------------------------------------------

void MyGLWidget::TerraTransform () {
  glm::mat4 TG(1.0f);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform () {
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);
  if(primeraPersona) {
   View = glm::lookAt (OBS, VRP, UP);
   View = glm::rotate(View, -Yverde, glm::vec3(0,1,0));
  }
  else {
    //EULER
    View = glm::translate(View, glm::vec3(0,0,-dist));
    View = glm::rotate(View, glm::radians(45.f) + theta, glm::vec3(1,0,0));
    View = glm::rotate(View, -psi, glm::vec3(0,1,0));
    View = glm::translate(View, -VRP);
  }
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform () {
  glm::mat4 Proj(1.0f);
  Proj = glm::perspective (newFOV, RA, zNear, zFar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::RoadTransform (glm::vec3 pos, float angle, float newEscala) {
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::rotate(TG, glm::radians(angle), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[ROAD] * newEscala));
  TG = glm::translate(TG, -centreBaseModels[ROAD]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::PipeTransform () {
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(escalaModels[PIPE]) * glm::vec3(3.0));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::CarTransform (float radi, float angle) {
  glm::mat4 TG(1.0f);
  TG = glm::rotate(TG, angle, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(radi,0,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[CAR]));
  TG = glm::translate(TG, -centreBaseModels[CAR]);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

// ---------------------------------------------- CALCULOS ------------------------------------------------
void MyGLWidget::iniEscena() {
  POSMAX = glm::vec3(10.0,3.f,10.0);
  POSMIN = glm::vec3(-10.0,0.0,-10.0);
  dist = glm::distance(POSMAX, POSMIN);
  radio = float(dist/2);
}

void MyGLWidget::iniCamera(){
  VRP = (POSMAX + POSMIN) / glm::vec3(2.0);
  OBS = glm::vec3(0,0,dist) + VRP;
  UP = glm::vec3(0,1,0);

  FOV = float(2*asin(radio/dist));
  newFOV = FOV;
  zNear = dist - radio;
  zFar = dist + radio;

  viewTransform();
  projectTransform();
}

void MyGLWidget::calculaCapsaModel (Model &p, float &escala, float profunditatDesitjada, glm::vec3 &centreBase) {
  // Càlcul capsa contenidora i valors transformacions inicials
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = p.vertices()[0];
  miny = maxy = p.vertices()[1];
  minz = maxz = p.vertices()[2];
  for (unsigned int i = 3; i < p.vertices().size(); i+=3)
  {
    if (p.vertices()[i+0] < minx)
      minx = p.vertices()[i+0];
    if (p.vertices()[i+0] > maxx)
      maxx = p.vertices()[i+0];
    if (p.vertices()[i+1] < miny)
      miny = p.vertices()[i+1];
    if (p.vertices()[i+1] > maxy)
      maxy = p.vertices()[i+1];
    if (p.vertices()[i+2] < minz)
      minz = p.vertices()[i+2];
    if (p.vertices()[i+2] > maxz)
      maxz = p.vertices()[i+2];
  }

  escala = profunditatDesitjada/(maxz-minz);
  centreBase[0] = (minx+maxx)/2.0; centreBase[1] = miny; centreBase[2] = (minz+maxz)/2.0;
}

void MyGLWidget::resizeGL (int w, int h) {
  float RAv = float(w) / float(h);
  newFOV = FOV;
  RA = RAv;
  if(RAv < 1)
    newFOV = 2 * atan(tan(FOV/2)/RAv);

  factorAngleY = M_PI / w;
  factorAngleX = M_PI / h;
  projectTransform();
}

void MyGLWidget::iniCamera1p() {
  OBS = glm::vec3(radioVerde,0.5f,0.5f);
  VRP = glm::vec3(radioVerde, 0.5f, -1.f);
  newFOV = glm::radians(60.f);
  zNear = 0.25f;


  viewTransform();
  projectTransform();
}

// --------------------------------------------- ANIMACIÓN -------------------------------------------------

void MyGLWidget::animar() {
  makeCurrent();
  Yazul += glm::radians(4.f);
  Yverde += glm::radians(3.f);
  Yrojo += glm::radians(2.f);

  viewTransform();
  update();
}

// ---------------------------------------------  EVENT -----------------------------------------------------

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();

  switch (event->key()) {
    case Qt::Key_Up: {
      Yazul += glm::radians(4.f);
      Yverde += glm::radians(3.f);
      Yrojo += glm::radians(2.f);

      viewTransform();
      break;
    }
    case Qt::Key_C: { 
      primeraPersona = not primeraPersona;
      if(primeraPersona)
        iniCamera1p();
      else
        iniCamera();
      break;
        }
    case Qt::Key_R: { 
      Yazul = 0.f;
      Yverde = 0.f;
      Yrojo = 0.f;
      primeraPersona = false;
      timerActivo = false;
      psi = 0;
      theta = 0;
      iniCamera();
      timer.stop();
      viewTransform();
      break;
        }  
    case Qt::Key_T: { 
      if(not timerActivo) {
        timerActivo = true;
        timer.start(1);
      }
      else {
        timerActivo = false;
        timer.stop();
      }
      break;
        }          
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e) {
  makeCurrent();
        LL2GLWidget::mousePressEvent(e);
    update();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
  makeCurrent();

  psi -= (e->x() - xClick) * factorAngleX;
  theta += (e->y() - yClick) * factorAngleY;

  xClick = e->x();
  yClick = e->y();
  viewTransform();
  update ();
}
