#pragma once

#include <obs.h>

#include <QDockWidget> 
#include <QImage>
#include <QWidget>
#include <QLabel>

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
    QImage image;
    QLabel *imgLabel = nullptr;

    YTHWidget(QWidget *parent = nullptr);

    // inline YTHWidget(QWidget *parent = nullptr) : QWidget(parent) {}
    // ~YTHWidget();

private:
    /**
     * @brief Take screenshot of specific source. This method duplicates 
     * functionality found in obs-studio's ScreenshotObj, minus the 
     * hardcoded save-to-file. 
     * 
     * @param source The source (or scene) to take a screenshot of.
     * @return true Returns true if new data was saved to image member.
     * @return false Returns false if no new data was saved.
     */
    bool takeSourceScreenshot(obs_source_t *source);

   void getServiceName();
   void getServiceProperties();
};