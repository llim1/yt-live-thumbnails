#pragma once

#include <obs.hpp>

#include <QDockWidget> 
#include <QImage>
#include <QWidget>
#include <QLabel>

class YTHDock : public QDockWidget 
{
    Q_OBJECT

    // TODO Fully implement like UI/qt-display.hpp
    OBSDisplay display;

public: 
    inline YTHDock(QWidget *parent = nullptr) : QDockWidget(parent) {} 
    // ~YTHDock();
};


class YTHWidget : public QWidget 
{
    Q_OBJECT 

    obs_display_t *display;

public:
    YTHWidget(QWidget *parent = nullptr);

    // inline YTHWidget(QWidget *parent = nullptr) : QWidget(parent) {}
    ~YTHWidget() { display = nullptr; }

    // uint32_t backgroundColorf = GREY_COLOR_BACKGROUND;

private:
    QImage image;
    QLabel *imgLabel;

    // void createDisplay(bool force = false);
    // static void drawPreview(void *data, uint32_t cx, uint32_t cy);

    // static void drawPreview(void *data, uint32_t cx, uint32_t cy);

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

    void updateThumbnail();
    void getServiceName();
    void getServiceProperties();
};