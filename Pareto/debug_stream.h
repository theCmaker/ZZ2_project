#ifndef DEBUG_STREAM_H
#define DEBUG_STREAM_H


#include <iostream>
#include <streambuf>
#include <string>

#include <QTextEdit>

//=======================================================================
// class pour rediriger la srotie d'application vers un champ de texte..
//=======================================================================
class debug_stream : public std::basic_streambuf<char>
{

 public:
     debug_stream(std::ostream &stream, QTextEdit* text_edit):m_stream(stream)
     {
          log_window = text_edit;
          m_old_buf = stream.rdbuf();
          stream.rdbuf(this);
     }
     ~debug_stream()
     {
          // output anything that is left
          if (!m_string.empty())
               log_window->append(m_string.c_str());
          m_stream.rdbuf(m_old_buf);
     }

protected:
     virtual int_type overflow(int_type v)
     {
      if (v == '\n')
      {
       log_window->append(m_string.c_str());
       m_string.erase(m_string.begin(), m_string.end());
      }
      else
       m_string += v;

      return v;
     }

     virtual std::streamsize xsputn(const char *p, std::streamsize n)
     {
      m_string.append(p, p + n);

      int pos = 0;
      while (pos != (int) std::string::npos)
      {
       pos = m_string.find('\n');
       if (pos != (int) std::string::npos)
       {
        std::string tmp(m_string.begin(), m_string.begin() + pos);
        log_window->append(tmp.c_str());
        m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
       }
      }
      return n;
     }

private:
     std::ostream &m_stream;
     std::streambuf *m_old_buf;
     std::string m_string;
     QTextEdit* log_window;
};



#endif // DEBUG_STREAM_H
