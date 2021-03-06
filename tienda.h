#ifndef TIENDA_H
#define TIENDA_H

#include <QMainWindow>
#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>

#include "producto.h"

#define IVA 12

QT_BEGIN_NAMESPACE
namespace Ui { class Tienda; }
QT_END_NAMESPACE

class Tienda : public QMainWindow
{
    Q_OBJECT

public:
    Tienda(QWidget *parent = nullptr);
    ~Tienda();

private slots:
    void on_inProducto_currentIndexChanged(int index);

    void on_btnAgregar_released();

    void on_actionNuevo_triggered();

    void on_action_Abrir_triggered();

    void on_action_Guardar_triggered();

    void on_actionA_gregar_triggered();

    void on_action_Salir_triggered();

private:
    Ui::Tienda *ui;
    QList<Producto*> m_productos;
    void cargarProductos();
    float m_subtotal;

    void calcular(float stProducto);

    void agregar();
    void abrir();
    void clean();
    void save();
    void save_as();

};
#endif // TIENDA_H
