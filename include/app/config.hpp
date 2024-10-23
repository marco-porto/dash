#pragma once

#include "openauto/Configuration/Configuration.hpp"
#include "canbus/ICANBus.hpp"

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


    inline const QStringList &get_launcher_plugins() { return this->launcher_plugins; }
    inline void set_launcher_plugin(QString plugin, bool remove = false)
    {
        if (remove)
            this->launcher_plugins.removeOne(plugin);
        else
            this->launcher_plugins.append(plugin);

        // need to sync idxs
        this->settings.remove("Pages/Launcher");
        for (int i = 0; i < this->launcher_plugins.size(); i++)
            this->settings.setValue(QString("Pages/Launcher/%1").arg(i), this->launcher_plugins[i]);
    }

    static Config *get_instance();

   private:
    QSettings settings;
    bool show_aa_connected;
    QStringList launcher_plugins;
};
