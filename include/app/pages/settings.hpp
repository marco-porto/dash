#pragma once

#include <QMap>
#include <QtWidgets>
#include "openauto/Configuration/Configuration.hpp"

#include "app/config.hpp"

#include "app/pages/page.hpp"

class Arbiter;

class SettingsPage : public QTabWidget, public Page {
    Q_OBJECT

   public:
    SettingsPage(Arbiter &arbiter, QWidget *parent = nullptr);

    void init() override;
};

class MainSettingsTab : public QWidget {
    Q_OBJECT

   public:
    MainSettingsTab(Arbiter &arbiter, QWidget *parent = nullptr);

   private:
    QWidget *settings_widget();
    QWidget *dark_mode_row_widget();
    QWidget *brightness_plugin_row_widget();
    QWidget *brightness_row_widget();
    QWidget *brightness_widget();
    QWidget *cursor_row_widget();
    QWidget *volume_row_widget();
    QWidget *controls_widget();
    QWidget *controls_row_widget();

    Arbiter &arbiter;
    Config *config;
};

class LayoutSettingsTab : public QWidget {
    Q_OBJECT

   public:
    LayoutSettingsTab(Arbiter &arbiter, QWidget *parent = nullptr);

   private:
    QWidget *settings_widget();
    QWidget *fullscreen_widget();
    QWidget *fullscreen_on_start_widget();
    QWidget *control_bar_widget();
    QWidget *scale_row_widget();
    QWidget *scale_widget();

    Arbiter &arbiter;
    Config *config;
};

class ActionsSettingsTab : public QWidget {
    Q_OBJECT

   public:
    ActionsSettingsTab(Arbiter &arbiter);

   private:
    QWidget *settings();
    QWidget *action_row(Action *action);
    QWidget *action_input(Action *action);

    Arbiter &arbiter;
    Config *config;
};
