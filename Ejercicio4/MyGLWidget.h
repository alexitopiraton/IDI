// MyGLWidget.h
#include "LL4GLWidget.h"
#include <QTimer>

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformTerra ();
    virtual void paintGL ( );
    virtual void modelTransformTrack (float radi, float rotY);
    virtual void viewTransform ();
    virtual void modelTransformMineCart ();
    virtual void iniMaterialTerra ();
    virtual void initializeGL ( );
     virtual void iniEscena ();
     virtual void modelTransformFantasma ();
  private:
    int printOglError(const char file[], int line, const char func[]);
    void enviaFocos();
    void enviaPosFocusEscena();

    // iniEscena
    glm::vec3 Pmax = glm::vec3(15.f, 2.5f, 15.f);
    glm::vec3 Pmin = glm::vec3(-15.f, 0.f, -15.f);
    glm::vec3 VRP, OBS;
    float radioEscena, diametroEscena, FOV, zNear, zFar;

    // keyPressEvent
    float gradosCarro = 0.f;
    bool apaga = false;
    bool timerActivo = false;
    QTimer timer;

    // mouseMoveEvent
    float angleX = glm::radians(-60.f);

    // UNIFORMS
    GLuint colorambienteLoc, colorFocCamLoc, colorFocEscLoc, posFocCamLoc, posFocEscLoc1, posFocEscLoc2;

  public slots:
  void animar();

};
