/**
  * @file mainwindow.h
  *
  * @brief Hlavičkový soubor pro mainwindow.cpp
  * @author xsebek02 xsimon14
  *
  */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QGraphicsView>
#include <QtGui/QMessageBox>
#include "diagramscene.h"

class QActionGroup;
class QGraphicsView;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    QList<DiagramScene*> scenes;
    QList<QGraphicsView*> views;
    QActionGroup *actionGroup;

public slots:
    void addTab();
    void closeTab(int tab);

};

#endif // MAINWINDOW_H
