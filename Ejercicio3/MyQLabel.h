#include <QLabel>
#include <QTimer>

class MyQLabel: public QLabel {
    Q_OBJECT

public:
    MyQLabel(QWidget *parent);
    QTimer *timer;
    QTimer *timer2;

public slots:
    void start();
    void reset();
    void iniciar_secuencia();
    void boton_rojo();
    void boton_verde();
    void boton_azul();
    void boton_amarillo();
    void mensajes_endgame();


signals:
    //[des]habilitar botones colores
    void deshabilitar_color();
    void habilitar_color();
    //hide/show start
    void hide_start();
    void show_start();
    // send info
    void print_stage(const QString &);
    void print_score(const QString &);
    void send_info(const QString &);

private:
    void end_game();
    void next_stage();
    void sumaScore();

    QVector<int> secuencia;
    bool neutro;
    int stage;
    int pos;
    int score;
    int end;
    bool resetTimers;
    bool open;
};
