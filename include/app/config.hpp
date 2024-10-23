#pragma once

#include "openauto/Configuration/Configuration.hpp"

#include <QObject>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QVideoFrame>
#include <QWidget>

/*
    NOTE: to only be used for page-specific config

    this is temporary, will soon be migrated to new cool ways
*/

class Config : public QObject {
    Q_OBJECT

   public:
    std::shared_ptr<openauto::configuration::Configuration> openauto_config;
    openauto::configuration::Configuration::ButtonCodes openauto_button_codes;

    Config();


    inline bool get_show_aa_connected() { return this->show_aa_connected; }
    inline void set_show_aa_connected(bool enabled)
    {
        this->show_aa_connected = enabled;
        this->settings.setValue("Pages/OpenAuto/show_aa_connected", this->show_aa_connected);
    }

    static Config *get_instance();

   private:
    QSettings settings;
    bool show_aa_connected;
    QStringList launcher_plugins;
};
