#ifndef BLACKDJACKPLUGININTERFACE_H
#define BLACKDJACKPLUGININTERFACE_H

#include <QString>
#include <QtPlugin>
#include <functional>

class GamePluginInterface
{
public:
    virtual ~GamePluginInterface() {};
    virtual QString namePlugin() const = 0;
    virtual void openGameWindow() = 0;
    virtual void closingAction(std::function<void()> fun) = 0;
};

Q_DECLARE_INTERFACE(GamePluginInterface, "gamePluginInterface")


#endif // BLACKDJACKPLUGININTERFACE_H
