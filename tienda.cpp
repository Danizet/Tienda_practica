#include "tienda.h"
#include "ui_tienda.h"

#include <QDebug>

Tienda::Tienda(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Tienda)
{
    ui->setupUi(this);
    // Lista de productos
    cargarProductos();
    // Mostrar los productos en el combo
    foreach (Producto *p, m_productos){
        ui->inProducto->addItem(p->nombre());
    }
    // Configurar cabecera de la tabla
    QStringList cabecera = {"Cantidad", "Producto", "P. unitario", "Subtotal"};
    ui->outDetalle->setColumnCount(4);
    ui->outDetalle->setHorizontalHeaderLabels(cabecera);
    // Establecer el subtotal a 0
    m_subtotal = 0;
}

Tienda::~Tienda()
{
    delete ui;
}
/**
 * @brief Tienda::cargarProductos Carga la lista de productos de la tienda
 */
void Tienda::cargarProductos()
{
    // Crear productos "quemados" en el código
    m_productos.append(new Producto(1, "Leche", 0.80));
    m_productos.append(new Producto(2, "Pan", 0.15));
    m_productos.append(new Producto(3, "Queso", 2.50));
    m_productos.append(new Producto(4, "Yogurt", 0.80));
    // Podría leerse de una base de datos, de un archivo o incluso de Internet
}

void Tienda::calcular(float stProducto)
{
    // Calcular valores
    m_subtotal += stProducto;
    float iva = m_subtotal * IVA / 100;
    float total = m_subtotal + iva;
    // Mostrar valores en GUI
    ui->outSubtotal->setText("$ " + QString::number(m_subtotal, 'f', 2));
    ui->outIva->setText("$ " + QString::number(iva, 'f', 2));
    ui->outTotal->setText("$ " + QString::number(total, 'f', 2));
}

void Tienda::agregar()
{
    // Validar que no se agregen productos con 0 cantidad
    int cantidad = ui->inCantidad->value();
    if (cantidad == 0){
        // Mostrar un error en la barra de estado
        ui->statusbar->showMessage("La cantidad no puede ser cero", 3000);
        return;
    }
    // Obtener los datos de la GUI
    int i = ui->inProducto->currentIndex();
    Producto *p = m_productos.at(i);

    // Calcular el subrotal del producto
    float subtotal = p->precio() * cantidad;

    // Agregar los datos a la tabla
    int fila = ui->outDetalle->rowCount();
    ui->outDetalle->insertRow(fila);
    ui->outDetalle->setItem(fila, 0, new QTableWidgetItem(QString::number(cantidad)));
    ui->outDetalle->setItem(fila, 1, new QTableWidgetItem(p->nombre()));
    ui->outDetalle->setItem(fila, 2, new QTableWidgetItem(QString::number(p->precio(),'f',2)));
    ui->outDetalle->setItem(fila, 3, new QTableWidgetItem(QString::number(subtotal,'f',2)));

    // Limpiar datos
    ui->inCantidad->setValue(0);
    ui->inProducto->setFocus();

    // Actualizar subtotales
    calcular(subtotal);
}

void Tienda::abrir()
{

    /*
    // Abrir un cuadro de dialogo para seleccionar el path y archivo a abrir
    QString nombreArchivo1 = QFileDialog::getOpenFileName(this,
                                                          "Abrir archivo de calculo de salarios",
                                                          QDir::home().absolutePath(),
                                                          "Archivo de texto (*.txt)");

    QFile file(nombreArchivo1);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString linea = in.readLine();
        ui->outCAlculos->appendPlainText(linea);
    }
    ui->statusbar->showMessage("Archivo:" + nombreArchivo1 + "abierto", 4000);
    file.close() ;

    // Abrir un cuadro de dialogo para seleccionar el path y archivo a guardadr
    QTextStream ope;
    QString nombreArchivo1 = QFileDialog::getOpenFileName(this,
                                                          "Abrir archivo de productos",
                                                          QDir::current().absolutePath(),
                                                          "Archivos de calculo (*.csv)");

    QFile archivo(nombreArchivo1);
    archivo.setFileName(nombreArchivo1);
    archivo.open(QIODevice::ReadOnly | QIODevice::Text);
    if(nombreArchivo1.isEmpty()){
        return;
    }
    if(!archivo.isOpen()){
        QMessageBox::warning(this,
                             "Aviso",
                             "No se pudo abrir el archivo");
        return;
    }

    QTextStream in(&archivo);
    int rowCount = 0;
    while (!in.atEnd()) {
        ///ui->rowCount(ix);
        /*
        ope.setDevice(&archivo);
        int fila = ui->outDetalle->rowCount();
        int columna = ui->outDetalle->columnCount();
        QString celda;
        for(int i=0; i<fila; i++){
            for(int j=0; j<columna; j++){
                if(j != (columna-1)){
                    celda = ui->outDetalle->item(i,j)->text()+";";
                }else{
                    celda = ui->outDetalle->item(i,j)->text();
                }

                ope << celda;
            }

            ope << "\n";

        }
        celda = in.readLine();
        ///ui->outDetalle->QTableView(celda);
        ui->statusbar->showMessage("Archivo:" + nombreArchivo1 + "abierto", 4000);
        archivo.flush();
        archivo.close();

        }
*/

}

void Tienda::clean()
{
    while (ui->outDetalle->rowCount() > 0){
        ui->outDetalle->removeRow(0);
    }
    // Configurar cabecera de la tabla
    QStringList cabecera = {"Cantidad", "Producto", "P. unitario", "Subtotal"};
    ui->outDetalle->setColumnCount(4);
    ui->outDetalle->setHorizontalHeaderLabels(cabecera);
    // Establecer el subtotal a 0
    m_subtotal = 0;
    // Mostrar mensaje en la barra de estado
    ui->statusbar->showMessage("Nuevos productos a agregar", 3000);
}

void Tienda::save()
{
    // Abrir un cuadro de dialogo para seleccionar el path y archivo a guardadr
    QTextStream sav;
    QString nombreArchivo = QFileDialog::getSaveFileName(this,"Guardar archivo de productos",
                                                         QDir::current().absolutePath() +
                                                         "/productos.csv",
                                                         "Archivos de calculo (*.csv)");
    QFile archivo;
    archivo.setFileName(nombreArchivo);
    archivo.open(QFile::WriteOnly | QFile::Truncate);
    if(!archivo.isOpen()){
        QMessageBox::critical(this,"Aviso","No se pudo guardar el archivo");
        return;
    }
    sav.setDevice(&archivo);
    int fila = ui->outDetalle->rowCount();
    int columna = ui->outDetalle->columnCount();
    QString celda;
    for(int i=0; i<fila; i++){
        for(int j=0; j<columna; j++){
            if(j != (columna-1)){
                celda = ui->outDetalle->item(i,j)->text()+";";
            }else{
                celda = ui->outDetalle->item(i,j)->text();
            }

            sav << celda;
        }

        sav << "\n";
    }

    archivo.flush();
    archivo.close();
}

void Tienda::save_as()
{
    int lala = ui->outDetalle->rowCount();
    if(lala == 0){
        this->close();
    }else {
        QMessageBox::StandardButton reply = QMessageBox::question(
                    this,
                    "Salir","Desea guardar el archivo",
                    QMessageBox::Save | QMessageBox::Close | QMessageBox::Cancel);
        /*
                        "Guardar" ,
                        "No guardar",
                        "Cancelar");
                        */
        if(reply == QMessageBox::Save ) {
            save();
        } else if (reply == QMessageBox::Cancel){
            return;
        } else{
            this->close();
        }
    }
}

/**
 * @brief Tienda::on_inProducto_currentIndexChanged Slot que se dispara al cambiar un elemento del combo
 * @param index Numero del elemento seleccionado en el combo
 * @author yo no :'v
 */
void Tienda::on_inProducto_currentIndexChanged(int index)
{
    // Obtener el precio del producto actual seleccionado
    float precio = m_productos.at(index)->precio();
    // Mostrar el precio del product en la etiqueta
    ui->outPrecio->setText("$ " + QString::number(precio,'f',2)); //decimales (2) del precio
    // Resetear el spinbox de cantidad
    ui->inCantidad->setValue(0);
}


void Tienda::on_btnAgregar_released()
{
    agregar();
}

void Tienda::on_actionNuevo_triggered()
{
    clean();
    /*
    // Limpiar widgets
   /// clean();
    // Lipmiar el texto de los calculos
    ui->outDetalle->clear();
    // Configurar cabecera de la tabla
    QStringList cabecera = {"Cantidad", "Producto", "P. unitario", "Subtotal"};
    ui->outDetalle->setColumnCount(4);
    ui->outDetalle->setHorizontalHeaderLabels(cabecera);
    // Establecer el subtotal a 0
    m_subtotal = 0;
    // Mostrar mensaje en la barra de estado
    ui->statusbar->showMessage("Nuevos productos a agregar", 3000);
*/
}


void Tienda::on_action_Abrir_triggered()
{
    abrir();
}


void Tienda::on_action_Guardar_triggered()
{
    save();
    /*
    // Abrir un cuadro de dialogo para seleccionar el path y archivo a guardar
        QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                             "Guardar productos vendidos",
                                                             QDir::home().absolutePath() +
                                                             "/productos.csv",
                                                             "hoja de texto (*.csv)");

        // Crear un archivo File
        QFile archivo(nombreArchivo);
        // Tratar arch. para escritura
        if(archivo.open(QFile::WriteOnly | QFile::Truncate)){
            // Crear un objeto 'stream' de texto
            QTextStream salida(&archivo);
            //Enviar los datos del resultado a la salida
            //salida << ui->outDetalle->visualItemRect(fila);
            //Mostrar mensaje en la barra de estados
            ui->statusbar->showMessage("Archivo guardado en:" +
                                       nombreArchivo,
                                       4000);
            // Cerrar archivo
            archivo.close();

        }else {
            // Mensaje de error
            QMessageBox::warning(this,
                                 "Guardar archivo",
                                 "No se puede acceder al archivo para guardar los datos");
        }
    */
}


void Tienda::on_actionA_gregar_triggered()
{
    agregar();
}


void Tienda::on_action_Salir_triggered()
{
    save_as();
    ///this->close();
}

