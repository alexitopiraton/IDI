#include "LL2GLWidget.h"
#include <QTimer>

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void keyPressEvent (QKeyEvent *event);
    virtual void TerraTransform ();
    virtual void iniCamera ();
    virtual void viewTransform ();
    virtual void projectTransform ();
    virtual void creaBuffersTerra();
    virtual void calculaCapsaModel (Model &p, float &escala, float profunditatDesitjada, glm::vec3 &centreBase);
    virtual void paintGL();
    virtual void RoadTransform (glm::vec3 pos, float angle, float newEscala);
    virtual void PipeTransform ();
    virtual void CarTransform (float radi, float angle);
    virtual void creaBuffersModels();
    virtual void carregaShaders();
    virtual void resizeGL (int w, int h);
    virtual void mousePressEvent (QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void iniEscena();
    virtual void initializeGL();

  private:
  
    int printOglError(const char file[], int line, const char func[]);
    void iniCamera1p();

    //uniforms
    GLuint colorUnif;

    //parametros escena
    float HmaxEscena = 0.f;
    float dist,radio;
    glm::vec3 POSMAX, POSMIN;

    //parametros camara
    glm::vec3 OBS,VRP,UP;
    float FOV, zNear, zFar, newFOV;
    float RA = 1.f;

    //parametros eventos
    float radioAzul = 6.f;
    float radioVerde = 7.5f;
    float radioRojo = 9.f;
    float Yazul = 0.f;
    float Yverde = 0.f;
    float Yrojo = 0.f;
    bool primeraPersona = false;

    //eventos
    float psi = 0.f;
    float theta = 0.f;
    bool timerActivo = false;
    QTimer timer;
public slots:
  void animar();
};
