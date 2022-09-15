#pragma once

#include <QDockWidget> 
#include <QWidget>

class YTHDock : public QDockWidget 
{
    Q_OBJECT

public: 
    inline YTHDock(QWidget *parent = nullptr) : QDockWidget(parent) {} 
    // ~YTHDock();
};


class YTHWidget : public QWidget 
{
    Q_OBJECT 

public:
    YTHWidget(QWidget *parent = nullptr);

    // inline YTHWidget(QWidget *parent = nullptr) : QWidget(parent) {}
    // ~YTHWidget();

private:
   void getServiceName();
   void getServiceProperties();
};