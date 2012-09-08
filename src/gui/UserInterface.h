#pragma once

#include "Cache.h"

#include <FL/Fl.H>
#include <string>

// forwards
class Model;
class Battle;
class User;
class SpringDialog;
class LoginDialog;
class RegisterDialog;
class AgreementDialog;
class LoggingDialog;
class ProgressDialog;
class TextDialog;
class ChannelsWindow;
class BattleList;
class BattleRoom;
class Tabs;
class ChatSettingsDialog;
class SoundSettingsDialog;

class Fl_Double_Window;
class Fl_Browser;
class Fl_Group;
class Fl_Menu_Bar;
class Fl_Tile;

class UserInterface
{
public:
    UserInterface(Model & model);
    virtual ~UserInterface();

    int run(int argc, char** argv);

    void addCallbackEvent(Fl_Awake_Handler handler, void *data);

private:
    Model & model_;
    Cache cache_;

    Fl_Double_Window * mainWindow_;
    Fl_Menu_Bar * menuBar_;

    ChannelsWindow * channelsWindow_;

    SpringDialog * springDialog_;
    LoginDialog * loginDialog_;
    RegisterDialog * registerDialog_;
    AgreementDialog * agreementDialog_;
    LoggingDialog * loggingDialog_;
    ProgressDialog * progressDialog_;
    TextDialog * autoJoinChannelsDialog_;
    ChatSettingsDialog * chatSettingsDialog_;
    SoundSettingsDialog * soundSettingsDialog_;

    Fl_Tile * tile_; // whole app window client area
    Fl_Tile * tileLeft_; // chat and battle list
    Tabs * tabs_;
    BattleList * battleList_;
    BattleRoom * battleRoom_;

    void loadAppIcon();
    void reloadMapsMods();
    void quit();

    // Model signal handlers
    void connected(bool connected);
    void loginResult(bool success, std::string const & info);
    void joinBattleFailed(std::string const & reason);
    void downloadDone(std::string const & name, bool success);

    // other signal handlers
    void autoJoinChannels(std::string const & text);
    void springProfileSet(std::string const & profile);

    // FLTK callbacks
    //
    static void menuLogin(Fl_Widget* w, void* d);
    static void menuDisconnect(Fl_Widget* w, void* d);
    static void menuRegister(Fl_Widget* w, void* d);
    static void menuRenameAccount(Fl_Widget* w, void* d);
    static void onQuit(Fl_Widget* w, void* d);
    static void onTest(Fl_Widget* w, void* d);
    static void menuRefresh(Fl_Widget *w, void* d);
    static void menuGenerateCacheFiles(Fl_Widget *w, void* d);
    static void menuSpring(Fl_Widget *w, void* d);
    static void menuLogging(Fl_Widget *w, void* d);
    static void menuJoinChannel(Fl_Widget *w, void* d);
    static void menuChannels(Fl_Widget *w, void* d);
    static void menuBattleListFilter(Fl_Widget *w, void* d);
    static void mainWindowCallback(Fl_Widget * w, void * p); // used to stop Escape key from exiting the program
    static void menuChannelsAutoJoin(Fl_Widget *w, void* d);
    static void menuChatSettings(Fl_Widget *w, void* d);
    static void menuSoundSettings(Fl_Widget *w, void* d);

    void enableMenuItem(void(*cb)(Fl_Widget*, void*), bool enable);

};
