#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::inicializar_atributos_aspas() {
  colores_aspas[0] = glm::vec3(1.0,0.0,0.0);      // ROJO
  colores_aspas[1] = glm::vec3(0.0,0.0,1.0);      // AZUL
  colores_aspas[2] = glm::vec3(0.0,1.0,0.0);      // VERDE
  colores_aspas[3] = glm::vec3(0.0,1.0,1.0);      // CIAN
  colores_aspas[4] = glm::vec3(1.0,0.0,1.0);      // ROSA
  colores_aspas[5] = glm::vec3(1.0,1.0,0.0);      // AMARILLO

  pivote = glm::vec3(0.0);
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  inicializar_atributos_aspas();
  carregaShaders();
  creaBuffersPal();
  creaBuffersAspa();
}


void MyGLWidget::modelTransformPal(glm::vec3 posicioBase, float escala)
{
  glm::mat4 TG(1.0f);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformAspa(glm::vec3 posicioPivot, float angle, float escala)
{
  glm::mat4 TG(1.0f);
  TG = rotate (TG, angle, posicioPivot);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  pintaMolinet(glm::vec3(0,0,0), 1.0f);  

}

void MyGLWidget::pintaMolinet(glm::vec3 posicioBase, float escala)
{
  // Pintem Pal
  glBindVertexArray(VAOPal);  
  modelTransformPal(glm::vec3(0.0), 1.0f);
  glDrawArrays(GL_TRIANGLES, 0, 6); 

  // Pintem Aspa  
  for(int i = 0; i < 6; ++i) {
    glBindVertexArray(VAOAspa[i]);
    modelTransformAspa(pivote, 60.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
 }

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

/*
 * Detecta las teclas A, D, 3 y 1
 */
void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_A: 
    	break;
    case Qt::Key_D: 
    	break;		
    case Qt::Key_3: 
    	break;		
    case Qt::Key_1: 
    	break;		
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaBuffersPal ()
{
  glm::vec3 Vertices[6];  // vèrtexs amb X, Y i Z
  
  // minX = -0.05
  // maxX = 0.05
  // minY = -1.0
  // maxY = 0.0
  
  Vertices[0] = glm::vec3(-0.05, -1.0, 0);
  Vertices[1] = glm::vec3( 0.05, -1.0, 0);
  Vertices[2] = glm::vec3( 0.025,  0.0, 0);
  Vertices[3] = glm::vec3( 0.025,  0.0, 0);
  Vertices[4] = glm::vec3(-0.025,  0.0, 0);
  Vertices[5] = glm::vec3(-0.05, -1.0, 0);

  glm::vec3 Colors[6];  // colors amb R, G i B
  
  Colors[0] = glm::vec3(0.5, 0.5, 0.5);
  Colors[1] = glm::vec3(0.5, 0.5, 0.5);
  Colors[2] = glm::vec3(0.5, 0.5, 0.5);
  Colors[3] = glm::vec3(0.5, 0.5, 0.5);
  Colors[4] = glm::vec3(0.5, 0.5, 0.5);
  Colors[5] = glm::vec3(0.5, 0.5, 0.5);

  
  // Creació del Vertex Array Object (VAO) que usarem per pintar el quadrat
  glGenVertexArrays(1, &VAOPal);
  glBindVertexArray(VAOPal);

  // Creació dels Vertex Buffer Objects (VBOs) amb les posicions i els colors dels vèrtexs
  GLuint VBOs[2];
  glGenBuffers(2, &VBOs[0]);
  
  // Omplim el VBO de posicions
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Omplim el VBO de colors
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);


  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersAspa ()
{
  glm::vec3 Vertices[6];  // vèrtexs amb X, Y i Z
  
  // minX = 0.0
  // maxX = 0.65
  // minY = 0.0
  // maxY = 0.3
  
  Vertices[0] = glm::vec3(0.0,  0.0,  0);
  Vertices[1] = glm::vec3(0.65,  0.0,  0);
  Vertices[2] = glm::vec3(0.25, 0.3,  0);
  Vertices[3] = glm::vec3(0.0,  0.0,  0);
  Vertices[4] = glm::vec3(0.25, 0.3,  0);
  Vertices[5] = glm::vec3(0.0,  0.25, 0);

  
  // Creació del Vertex Array Object (VAO) que usarem per pintar el quadrat
  glGenVertexArrays(6, &VAOAspa[0]);
  glm::vec3 Colors[6];  // colors amb R, G i B

  for(int i = 0; i < 6; ++i) {
    Colors[0] = colores_aspas[i];
    Colors[1] = colores_aspas[i];
    Colors[2] = colores_aspas[i];
    Colors[3] = colores_aspas[i] + glm::vec3(0.75,0.75,0.75);
    Colors[4] = colores_aspas[i] + glm::vec3(0.75,0.75,0.75);
    Colors[5] = colores_aspas[i] + glm::vec3(0.75,0.75,0.75);

    glBindVertexArray(VAOAspa[i]);

    // Creació dels Vertex Buffer Objects (VBOs) amb les posicions i els colors dels vèrtexs
    GLuint VBOs[2];
    glGenBuffers(2, &VBOs[0]);

    // Omplim el VBO de posicions
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // Omplim el VBO de colors
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);


    // Desactivem el VAO
    glBindVertexArray(0);
  }
}

/*
 Crea puntero para vertex (vec3 de vertices), color (vec3 de los colores) y TG (mat4 de la transformacion)
 */
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

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");
  
  // Obtenim els identificadors dels uniforms
  TGLoc = glGetUniformLocation(program->programId(), "TG"); 
}
