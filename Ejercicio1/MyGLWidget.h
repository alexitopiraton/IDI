#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

    virtual void inicializar_atributos_aspas();


  private:
      
    void pintaMolinet (glm::vec3 posicioBase, float escala);  
      
    void creaBuffersPal();
    void creaBuffersAspa();
    
    void carregaShaders();
    
    void modelTransformPal(glm::vec3 posicioBase, float escala);
    void modelTransformAspa(glm::vec3 posicioPivot, float angle, float escala);
    
    // program
    QOpenGLShaderProgram *program;
    
    // attribute locations
    GLuint vertexLoc;
    GLuint colorLoc;
    
    // uniform locations
    GLuint TGLoc;

    // VAOs
    GLuint VAOPal;
    GLuint VAOAspa[6];

    // atr aspas
   glm::vec3 colores_aspas[6];
   glm::vec3 pivote;

    // viewport
    GLint ample, alt;    
};
