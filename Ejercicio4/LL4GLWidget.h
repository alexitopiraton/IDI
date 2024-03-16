#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class LL4GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    LL4GLWidget (QWidget *parent=0);
    ~LL4GLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);
    // mouse{Press/Release/Move}Event - Són cridades quan es realitza l'event 
    // corresponent de ratolí
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);

    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void iniMaterialTerra ();
    virtual void modelTransformTerra ();
    virtual void modelTransformTrack (float radi, float rotY);
    virtual void modelTransformFantasma ();
    virtual void modelTransformMineCart ();

    virtual void viewTransform ();
    void projectTransform ();
    
    void creaBuffersTrack ();	
    void creaBuffersFantasma ();
    void creaBuffersMineCart ();
    void creaBuffersTerra ();
    void carregaShaders ();


    
    void calculaCapsaModelTrack ();
    void calculaCapsaModelFantasma ();
    void calculaCapsaModelMineCart ();

    // VAO names
    GLuint VAO_Fantasma;
    GLuint VAO_Track;
    GLuint VAO_MineCart;
    GLuint VAO_Terra;

    // Program
    QOpenGLShaderProgram *program;

    // Viewport
    GLint ample, alt;

    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // attribute locations
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;

    // Matriu de posició i orientació
    glm::mat4 View;

    // models
    Model fantasma;
    Model track;
    Model mineCart;
    // paràmetres calculats a partir de les capses contenidores dels models
    glm::vec3 centreBaseTrack;
    glm::vec3 centreBaseFantasma;
    glm::vec3 centreBaseMineCart;
    float escalaTrack, escalaFantasma, escalaMineCart;

    // paràmetres de l'escena
    glm::vec3 centreEsc;
    float ra, radiEsc;

    // Definim els paràmetres del material del terra
    glm::vec3 amb, diff, spec;
    float shin;

    typedef  enum {NONE, ROTATE} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float angleY;
    float factorAngleY;
};

