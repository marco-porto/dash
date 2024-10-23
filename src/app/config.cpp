#include <QApplication>
#include <QDir>

#include "app/config.hpp"

Config::Config()
    : QObject(qApp),
      openauto_config(std::make_shared<openauto::configuration::Configuration>()),
      openauto_button_codes(openauto_config->getButtonCodes()),
      settings()
{
    this->show_aa_connected = this->settings.value("Pages/OpenAuto/show_aa_connected", 100).toBool();
}

Config *Config::get_instance()
{
    static Config config;
    return &config;
}
