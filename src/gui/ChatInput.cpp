#include "ChatInput.h"

#include "FL/Fl.H"
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>


ChatInput::ChatInput(int x, int y, int w, int h, size_t historySize):
    Fl_Input(x, y, w, h),
    historySize_(historySize),
    pos_(-1)
{

    callback(ChatInput::callbackText, this);
    when(FL_WHEN_ENTER_KEY);

}

void ChatInput::callbackText(Fl_Widget * w, void * data)
{
    ChatInput * o = static_cast<ChatInput*>(data);
    o->onText();
}

void ChatInput::onText()
{
    std::string msg(value());
    boost::trim(msg);
    if (!msg.empty())
    {
        if (history_.empty() || msg != history_.front()) // avoid adding duplicates
        {
            history_.push_front(msg);
            if (history_.size() > historySize_)
            {
                history_.pop_back();
            }
        }
        pos_ = -1;
        textSignal_(msg);
    }

    value(0);
}

int ChatInput::handle(int event)
{
    switch (event)
    {
        case FL_KEYDOWN:
            if (Fl::event_key() == FL_Down)
            {
                historyDown();
                return 1;
            }
            else if (Fl::event_key() == FL_Up)
            {
                historyUp();
                return 1;
            }
            break;
    }
    return Fl_Input::handle(event);
}

void ChatInput::historyDown()
{
    --pos_;
    if (pos_ >= 0 && pos_ < history_.size())
    {
        replace(0, size(), history_[pos_].c_str());
    }
    else
    {
        pos_ = -1;
        value(0);
    }
}

void ChatInput::historyUp()
{
    ++pos_;
    if (pos_ >= history_.size())
    {
        pos_ = history_.size() - 1;
    }
    if (pos_ >= 0)
    {
        replace(0, size(), history_[pos_].c_str());
    }
}
