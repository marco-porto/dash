#include <BluezQt/Device>
#include <BluezQt/PendingCall>
#include <QLabel>
#include <QLayoutItem>
#include <QScrollArea>

#include <aasdk_proto/ButtonCodeEnum.pb.h>
#include <aasdk_proto/VideoFPSEnum.pb.h>
#include <aasdk_proto/VideoResolutionEnum.pb.h>

#include "openauto/Configuration/AudioOutputBackendType.hpp"
#include "openauto/Configuration/BluetootAdapterType.hpp"
#include "openauto/Configuration/HandednessOfTrafficType.hpp"

#include "app/action.hpp"
#include "app/config.hpp"
#include "app/session.hpp"
#include "app/window.hpp"
#include "app/pages/settings.hpp"
#include "app/widgets/selector.hpp"
#include "app/widgets/switch.hpp"

SettingsPage::SettingsPage(Arbiter &arbiter, QWidget *parent)
    : QTabWidget(parent)
    , Page(arbiter, "Settings", "tune", false, this)
{
}

void SettingsPage::init()
{
    this->addTab(new MainSettingsTab(this->arbiter), "Main");
    this->addTab(new LayoutSettingsTab(this->arbiter), "Layout");
    this->addTab(new ActionsSettingsTab(this->arbiter), "Actions");
}

MainSettingsTab::MainSettingsTab(Arbiter &arbiter, QWidget *parent)
    : QWidget(parent)
    , arbiter(arbiter)
{
    this->config = Config::get_instance();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(6, 0, 6, 0);

    layout->addWidget(this->settings_widget());
}

QWidget *MainSettingsTab::settings_widget()
{
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    layout->addWidget(this->dark_mode_row_widget(), 1);
    layout->addWidget(this->cursor_row_widget(), 1);
    layout->addWidget(this->volume_row_widget(), 1);
    layout->addWidget(this->brightness_row_widget(), 1);
    layout->addWidget(Session::Forge::br(), 1);
    layout->addWidget(this->controls_row_widget(), 1);

    QScrollArea *scroll_area = new QScrollArea(this);
    Session::Forge::to_touch_scroller(scroll_area);
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(widget);

    return scroll_area;
}

QWidget *MainSettingsTab::dark_mode_row_widget()
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);

    QLabel *label = new QLabel("Dark Mode", widget);
    layout->addWidget(label, 1);

    Switch *toggle = new Switch(widget);
    toggle->scale(this->arbiter.layout().scale);
    toggle->setChecked(this->arbiter.theme().mode == Session::Theme::Dark);
    connect(&this->arbiter, &Arbiter::mode_changed, [toggle, this](Session::Theme::Mode mode){
        toggle->setChecked(mode == Session::Theme::Dark);
    });
    connect(toggle, &Switch::stateChanged, [this](bool state){
        this->arbiter.set_mode(state ? Session::Theme::Dark : Session::Theme::Light);
    });

    layout->addWidget(toggle, 1, Qt::AlignHCenter);

    return widget;
}

QWidget *MainSettingsTab::brightness_row_widget()
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);

    QLabel *label = new QLabel("Brightness", widget);
    layout->addWidget(label, 1);

    layout->addWidget(this->brightness_widget(), 1);

    return widget;
}

QWidget *MainSettingsTab::brightness_widget()
{
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    auto plugins = this->arbiter.system().brightness.plugins();
    Selector *selector = new Selector(plugins, this->arbiter.system().brightness.plugin, this->arbiter.forge().font(14), this->arbiter, widget, Session::System::Brightness::AUTO_PLUGIN);
    connect(selector, &Selector::item_changed, [this](QString item){ this->arbiter.set_brightness_plugin(item); });

    layout->addWidget(selector);
    layout->addWidget(this->arbiter.forge().brightness_slider());

    return widget;
}

QWidget *MainSettingsTab::cursor_row_widget()
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);

    QLabel *label = new QLabel("Cursor", widget);
    layout->addWidget(label, 1);

    Switch *toggle = new Switch(widget);
    toggle->scale(this->arbiter.layout().scale);
    toggle->setChecked(this->arbiter.core().cursor);
    connect(toggle, &Switch::stateChanged, [this](bool state){
        this->arbiter.set_cursor(state);
    });
    layout->addWidget(toggle, 1, Qt::AlignHCenter);

    return widget;
}

QWidget *MainSettingsTab::volume_row_widget()
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);

    QLabel *label = new QLabel("Volume", widget);
    layout->addWidget(label, 1);

    layout->addWidget(this->arbiter.forge().volume_slider(), 1);

    return widget;
}

QWidget *MainSettingsTab::controls_row_widget()
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);

    QLabel *label = new QLabel("Controls", widget);
    layout->addWidget(label, 1);

    layout->addWidget(this->controls_widget(), 1);

    return widget;
}

QWidget *MainSettingsTab::controls_widget()
{
    auto widget = new QWidget();
    auto layout = new QVBoxLayout(widget);

    auto minimize_button = new QPushButton("minimize");
    connect(minimize_button, &QPushButton::clicked, [this]{ this->arbiter.window()->setWindowState(Qt::WindowMinimized); });
    layout->addWidget(minimize_button);

    auto exit_button = new QPushButton("exit");
    connect(exit_button, &QPushButton::clicked, [this]{ qApp->exit(); });
    layout->addWidget(exit_button);

    return widget;
}

LayoutSettingsTab::LayoutSettingsTab(Arbiter &arbiter, QWidget *parent)
    : QWidget(parent)
    , arbiter(arbiter)
{
    this->config = Config::get_instance();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(6, 0, 6, 0);

    layout->addWidget(this->settings_widget());
}

QWidget *LayoutSettingsTab::settings_widget()
{
    QWidget *widget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(widget);

    layout->addWidget(this->fullscreen_widget(), 1);
    layout->addWidget(this->fullscreen_on_start_widget(), 1);
    layout->addWidget(this->control_bar_widget(), 1);
    layout->addWidget(this->scale_row_widget(), 1);

    QScrollArea *scroll_area = new QScrollArea(this);
    Session::Forge::to_touch_scroller(scroll_area);
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(widget);

    return scroll_area;
}

QWidget *LayoutSettingsTab::fullscreen_widget()
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);

    QLabel *label = new QLabel("Fullscreen Toggler", widget);
    layout->addWidget(label, 1);

    QStringList togglers;
    for (auto toggler : this->arbiter.layout().fullscreen.togglers())
        togglers.append(toggler->name());
    Selector *selector = new Selector(togglers, this->arbiter.layout().fullscreen.curr_toggler->name(), this->arbiter.forge().font(14), this->arbiter, widget);
    connect(selector, &Selector::idx_changed, [this](int idx){
        this->arbiter.set_curr_fullscreen_toggler(idx);
    });
    layout->addWidget(selector, 1);

    return widget;
}

QWidget *LayoutSettingsTab::fullscreen_on_start_widget()
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);

    QLabel *label = new QLabel("Fullscreen on Start", widget);
    layout->addWidget(label, 1);

    Switch *toggle = new Switch(widget);
    toggle->scale(this->arbiter.layout().scale);
    toggle->setChecked(this->arbiter.layout().fullscreen.on_start);
    connect(toggle, &Switch::stateChanged, [this](bool state){ this->arbiter.set_fullscreen_on_start(state); });
    layout->addWidget(toggle, 1, Qt::AlignHCenter);

    return widget;
}

QWidget *LayoutSettingsTab::control_bar_widget()
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);

    QLabel *label = new QLabel("Control Bar", widget);
    layout->addWidget(label, 1);

    Switch *toggle = new Switch(widget);
    toggle->scale(this->arbiter.layout().scale);
    toggle->setChecked(this->arbiter.layout().control_bar.enabled);
    connect(toggle, &Switch::stateChanged, [this](bool state){ this->arbiter.set_control_bar(state); });
    layout->addWidget(toggle, 1, Qt::AlignHCenter);

    return widget;
}

QWidget *LayoutSettingsTab::scale_row_widget()
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);

    QLabel *label = new QLabel("Scale", widget);
    layout->addWidget(label, 1);

    layout->addWidget(this->scale_widget(), 1);

    return widget;
}

QWidget *LayoutSettingsTab::scale_widget()
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);

    QSlider *slider = new QSlider(Qt::Orientation::Horizontal, widget);
    slider->setTracking(false);
    slider->setRange(2, 6);
    slider->setValue(this->arbiter.layout().scale * 4);
    connect(slider, &QSlider::valueChanged, [this, slider](int position){ this->arbiter.set_scale(position / 4.0); });

    QPushButton *lower_button = new QPushButton(widget);
    lower_button->setFlat(true);
    this->arbiter.forge().iconize("remove", lower_button, 32);
    connect(lower_button, &QPushButton::clicked, [slider]{ slider->setValue(slider->value() - 1); });

    QPushButton *raise_button = new QPushButton(widget);
    raise_button->setFlat(true);
    this->arbiter.forge().iconize("add", raise_button, 32);
    connect(raise_button, &QPushButton::clicked, [slider]{ slider->setValue(slider->value() + 1); });

    layout->addWidget(lower_button);
    layout->addWidget(slider, 4);
    layout->addWidget(raise_button);

    return widget;
}

ActionsSettingsTab::ActionsSettingsTab(Arbiter &arbiter)
    : QWidget()
    , arbiter(arbiter)
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(6, 0, 6, 0);

    layout->addWidget(this->settings());
}

QWidget *ActionsSettingsTab::settings()
{
    auto widget = new QWidget();
    auto layout = new QVBoxLayout(widget);

    for (auto action : this->arbiter.core().actions())
        layout->addWidget(this->action_row(action));

    auto scroll_area = new QScrollArea();
    Session::Forge::to_touch_scroller(scroll_area);
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(widget);

    return scroll_area;
}

QWidget *ActionsSettingsTab::action_row(Action *action)
{
    auto widget = new QWidget();
    auto layout = new QHBoxLayout(widget);

    auto label = new QLabel(action->name());

    layout->addWidget(label, 1);
    layout->addWidget(this->action_input(action), 1);

    return widget;
}

QWidget *ActionsSettingsTab::action_input(Action *action)
{
    auto widget = new QWidget();
    auto layout = new QHBoxLayout(widget);

    auto dialog = new ActionDialog(this->arbiter);
    dialog->set_title(action->name());

    auto button = new QPushButton(action->key());
    button->setFont(this->arbiter.forge().font(16, true));
    connect(button, &QPushButton::clicked, [dialog]{ dialog->open(); });

    auto symbol = new QPushButton();
    Session::Forge::symbolize(symbol);
    symbol->setFlat(true);
    symbol->setCheckable(true);
    symbol->setVisible(!action->key().isNull());
    symbol->setChecked(action->key().startsWith("gpio"));
    this->arbiter.forge().iconize("keyboard", "developer_board", symbol, 32);

    auto save = new QPushButton("save");
    connect(save, &QPushButton::clicked, [this, action, dialog, button, symbol]{
        this->arbiter.set_action(action, dialog->key());
        button->setText(action->key());
        symbol->setVisible(!action->key().isNull());
        symbol->setChecked(action->key().startsWith("gpio"));
    });
    dialog->set_button(save);

    layout->addWidget(button, 1);
    layout->addWidget(symbol, 0, Qt::AlignRight);

    return widget;
}



