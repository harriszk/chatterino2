#pragma once

#include "widgets/BaseWindow.hpp"

#include <pajlada/signals/signal.hpp>

class QCheckBox;

namespace chatterino {

class Channel;
using ChannelPtr = std::shared_ptr<Channel>;
class Label;

class UserInfoPopup final : public BaseWindow
{
    Q_OBJECT

public:
    UserInfoPopup();

    void setData(const QString &name, const ChannelPtr &channel);

protected:
    virtual void themeChangedEvent() override;

private:
    void installEvents();
    void updateUserData();

    void loadAvatar(const QUrl &url);
    void loadProfileBanner(const QUrl &url);
    bool isMod_;
    bool isBroadcaster_;

    QString userName_;
    QString userId_;
    ChannelPtr channel_;

    pajlada::Signals::NoArgSignal userStateChanged_;

    std::shared_ptr<bool> hack_;

    struct {
        Button *avatarButton = nullptr;

        // RippleEffectLabel2 *viewLogs = nullptr;
        QPixmap profileBanner;
        Label *nameLabel = nullptr;
        Label *viewCountLabel = nullptr;
        Label *followerCountLabel = nullptr;
        Label *createdDateLabel = nullptr;

        QCheckBox *follow = nullptr;
        QCheckBox *ignore = nullptr;
        QCheckBox *ignoreHighlights = nullptr;
    } ui_;

    class TimeoutWidget : public BaseWidget
    {
    public:
        enum Action { Ban, Unban, Timeout };

        TimeoutWidget();

        pajlada::Signals::Signal<std::pair<Action, int>> buttonClicked;

    protected:
        void paintEvent(QPaintEvent *event) override;
    };
};

}  // namespace chatterino
