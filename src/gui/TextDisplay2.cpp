#include "TextDisplay2.h"

#include "log/Log.h"

#include <FL/Fl.H>
#include <sstream>
#include <cstring>
#include <ctime>

static Fl_Text_Display::Style_Table_Entry styles[] =
{
    {  FL_DARK2, FL_COURIER, 10 }, // A - time
    {  FL_FOREGROUND_COLOR, FL_HELVETICA, 12 }, // B - normal
    {  FL_DARK2, FL_HELVETICA, 12 }, // C - grey
};

TextDisplay2::TextDisplay2(int x, int y, int w, int h, char const * label):
    Fl_Text_Display(x, y, w, h, label)
{
    textsize(12);

    text_ = new Fl_Text_Buffer();
    buffer(text_);

    style_ = new Fl_Text_Buffer();
    int style_size = sizeof(styles)/sizeof(styles[0]);
    highlight_data(style_, styles, style_size, 'A', 0, 0);

    wrap_mode(WRAP_AT_BOUNDS, 0);
}

TextDisplay2::~TextDisplay2()
{
}

void TextDisplay2::append(std::string const & text, bool interesting)
{
    // scroll to bottom if last line is visible
    bool scrollToBottom = false;
    {
        int line;
        if (position_to_line(insert_position(), &line) != 0)
        {
            scrollToBottom = true;
        }
    }

    // if string is empty we just add one empty line
    if (text.empty())
    {
        text_->append("\n");
        style_->append("\n");
    }
    else
    {
        // time stamp
        char buf[16];
        std::time_t t = std::time(0);
        std::tm tm = *std::localtime(&t);
        std::strftime(buf, 16, "%H:%M:%S ", &tm);

        std::ostringstream oss;
        oss << buf << text << '\n';
        std::string const line = oss.str();

        text_->append(line.c_str());

        // style for time
        std::string const styleTime(::strlen(buf), 'A');
        style_->append(styleTime.c_str());

        // style for rest (text + newline)
        std::string const styleText(text.size(), interesting ? 'B' : 'C');
        style_->append(styleText.c_str());
        style_->append("\n");
    }

    // limit text buffer size
    while (text_->length() > 10000)
    {
        int const posNewline = text_->line_end(0);
        text_->remove(0, posNewline+1);
        style_->remove(0, posNewline+1);
    }

    insert_position(text_->length());
    if (scrollToBottom)
    {
        show_insert_position();
    }
}

int TextDisplay2::handle(int event)
{
    // make mouse wheel scroll in bigger steps
    if (event == FL_MOUSEWHEEL)
    {
        Fl::e_dy *= 5;
    }

    switch (event)
    {
    case FL_PUSH: // handle double click on text with web link
        if (Fl::event_clicks() && Fl::event_button() == FL_LEFT_MOUSE)
        {
            int const pos = xy_to_position(Fl::event_x(), Fl::event_y());
            if (pos < text_->length())
            {
                int posStart;
                if (text_->search_backward(pos, "http", &posStart) && posStart >= text_->line_start(pos))
                {
                    int posSpace;
                    int posNewline;
                    int found = 0;
                    found += text_->findchar_forward(posStart, ' ', &posSpace);
                    found += text_->findchar_forward(posStart, '\n', &posNewline);
                    if (found > 0)
                    {
                        int const posEnd = std::min(posSpace, posNewline);
                        text_->select(posStart, posEnd);
                        char * sel = text_->selection_text();
                        std::string const link = sel;
                        ::free(sel);
                        LOG(DEBUG) << "link: '" << link << "'";

                        std::string const cmd = "xdg-open " + link;
                        std::system(cmd.c_str());

                        return 1;
                    }
                }
            }
        }
        break;
    }

    return Fl_Text_Display::handle(event);
}