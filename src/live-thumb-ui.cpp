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

YTHWidget::YTHWidget(QWidget *parent) : QWidget(parent) 
{ 
    QVBoxLayout *mainLayout = new QVBoxLayout();
    // QGridLayout *topLayout = new QGridLayout();
    // QGridLayout *outputLayout = new QGridLayout();

    // Test service name finding while live 
    // Create button, layout, and add it 
    QPushButton *nameButton = new QPushButton(QString("Get Service Name"));
    QHBoxLayout *buttonLayout = new QHBoxLayout; 
    buttonLayout->addStretch();
    buttonLayout->addWidget(nameButton);

    // Add properties button
    QPushButton *propButton = new QPushButton(QString("Get Properties"));
    buttonLayout->addWidget(propButton);
    
    // Add to main layout
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    // Connect buttons to functions
    connect(nameButton, &QPushButton::clicked, this, &YTHWidget::getServiceName);
    connect(propButton, &QPushButton::clicked, this, &YTHWidget::getServiceProperties);
}

void YTHWidget::getServiceName()
{
    // TODO find how to release service reference. 
	obs_service_t *service = obs_frontend_get_streaming_service();
	obs_service_t *serviceRef = obs_service_get_ref(service);
	
    blog(LOG_INFO, "Service name: '%s' ", 
        obs_service_get_url(serviceRef));
    blog(LOG_INFO, "Service credentials: '%s' '%s' ",
        obs_service_get_username(serviceRef),
        obs_service_get_password(serviceRef));

	// const char *user = obs_service_get_name(service);

	// blog(LOG_INFO, "username: %c", user);
	// if(&user != NULL) {
		// delete(user);
	// }

	// Figured it out
	obs_service_release(serviceRef);
}

void YTHWidget::getServiceProperties() 
{
    obs_service_t *service = obs_frontend_get_streaming_service();
    obs_service_t *serviceRef = obs_service_get_ref(service);

    blog(LOG_INFO, "Service name: '%s' ",
        obs_service_get_name(serviceRef));
    
    obs_data_t *settings = obs_service_get_settings(serviceRef);

    obs_service_release(serviceRef);
}