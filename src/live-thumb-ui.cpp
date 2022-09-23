#include "live-thumb-ui.h"

#include <obs-frontend-api.h>
#include <obs.h>

#include <QMessageBox>
#include <QCheckBox> 
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QString> 
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

YTHWidget::YTHWidget(QWidget *parent) : QWidget(parent) 
{ 
    QVBoxLayout *mainLayout = new QVBoxLayout();
    // QGridLayout *topLayout = new QGridLayout();
    // QGridLayout *outputLayout = new QGridLayout();

    // Test service name finding while live 
    // Create button, layout, and add it 
    QPushButton *nameButton = new QPushButton(QString("Get Service/Output Name"));
    QHBoxLayout *buttonLayout = new QHBoxLayout; 
    buttonLayout->addStretch();
    buttonLayout->addWidget(nameButton);

    // Add properties button
    QPushButton *propButton = new QPushButton(QString("Get Properties"));
    buttonLayout->addWidget(propButton);

    // Add Image Label
    imgLabel = new QLabel("");
    mainLayout->addWidget(imgLabel);

    // Add to main layout
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    // Connect buttons to functions
    connect(nameButton, &QPushButton::clicked, this, &YTHWidget::getServiceName);
    connect(propButton, &QPushButton::clicked, this, &YTHWidget::getServiceProperties);
}

bool YTHWidget::takeSourceScreenshot(obs_source_t *source) {
    obs_enter_graphics();

    gs_texrender_t *texrender = nullptr;
    gs_stagesurf_t *stagesurf = nullptr;

    // Get preview source
    obs_source_t *sourceStrongRef = obs_source_get_ref(source);
    if(!sourceStrongRef) {
        blog(LOG_WARNING, "Could not get strong reference for preview source!");
        return false; 
    }

    // Get height and width of source
    uint32_t cx = obs_source_get_base_width(sourceStrongRef);
    uint32_t cy = obs_source_get_base_height(sourceStrongRef);
    if(!cx || !cy) {
        blog(LOG_WARNING, "Could not get width or height!");
        return false;
    }

#ifdef _WIN32
    enum gs_color_space space = obs_source_get_color_space(sourceStrongRef, 0, nullptr);
    if (space == GS_CS_709_EXTENDED) {
        space = GS_CS_709_SCRGB; // Convert for JXR
    }
#else
    const enum gs_color_space space = GS_CS_SRGB;
#endif 
    const enum gs_color_format format = gs_get_format_from_space(space);

    // TODO diagnose error "device_copy_texture (D3D11): Source texture is NULL" in logs
    texrender = gs_texrender_create(format, GS_ZS_NONE);
    stagesurf = gs_stagesurface_create(cx, cy, format);

    gs_stage_texture(stagesurf, gs_texrender_get_texture(texrender));

    if (gs_texrender_begin_with_color_space(texrender, cx, cy, space)) {
        vec4 zero;
        vec4_zero(&zero);

        gs_clear(GS_CLEAR_COLOR, &zero, 0.0f, 0);
        gs_ortho(0.0f, (float)cx, 0.0f, (float)cy, -100.0f, 100.0f);

        gs_blend_state_push();
        gs_blend_function(GS_BLEND_ONE, GS_BLEND_ZERO);

        obs_source_inc_showing(sourceStrongRef);
        obs_source_video_render(sourceStrongRef);
        obs_source_dec_showing(sourceStrongRef);

        gs_blend_state_pop();
        gs_texrender_end(texrender);
    }

    gs_stage_texture(stagesurf, gs_texrender_get_texture(texrender));

    uint8_t *videoData = nullptr;
    uint32_t videoLinesize = 0;

    blog(LOG_INFO, "LOG: begin copy");
    if (gs_stagesurface_map(stagesurf, &videoData, &videoLinesize)) {
        blog(LOG_INFO, "LOG: in copy");
        image = QImage(cx, cy, QImage::Format::Format_RGBX8888);

        int linesize = image.bytesPerLine();
        blog(LOG_INFO, "%d", linesize);
        for (int y = 0; y < (int)cy; y++) {
            memcpy(image.scanLine(y), videoData + (y * videoLinesize), linesize);
            // blog(LOG_INFO, "%s", image.scanLine(y));
        }
    }

    // cleanup
    gs_stagesurface_unmap(stagesurf);
    gs_stagesurface_destroy(stagesurf);
    gs_texrender_destroy(texrender);
    obs_source_release(source);
    obs_leave_graphics();

    image.save("C:/Users/Logan/Videos/Screenshot 2022-09-21 21-45-26.png");

    return true;
}

void YTHWidget::getServiceName()
{
	obs_service_t *service = obs_frontend_get_streaming_service();
	obs_service_t *serviceRef = obs_service_get_ref(service);
	
    blog(LOG_INFO, "Service name: '%s' ", 
        obs_service_get_url(serviceRef));
    blog(LOG_INFO, "Service credentials: '%s' '%s' ",
        obs_service_get_username(serviceRef),
        obs_service_get_password(serviceRef));

	obs_service_release(serviceRef);

    // TESTING take screenshot
    obs_source_t *source = obs_frontend_get_current_scene();
    if(takeSourceScreenshot(source)) {
        blog(LOG_INFO, "Screenshot taken!");
    } else {
        blog(LOG_INFO, "Could not take screenshot!");
    }
}

void YTHWidget::getServiceProperties() 
{
    obs_service_t *service = obs_frontend_get_streaming_service();
    obs_service_t *serviceRef = obs_service_get_ref(service);

    blog(LOG_INFO, "Service name: '%s' ",
        obs_service_get_name(serviceRef));
    
    // obs_data_t *settings = obs_service_get_settings(serviceRef);

    obs_service_release(serviceRef);
}