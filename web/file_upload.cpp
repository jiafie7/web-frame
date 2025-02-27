#include "web/file_upload.h"
using namespace melon::web;

#include "fs/file.h"
using namespace melon::fs;

FileUpload::FileUpload()
  : m_buf(nullptr)
  , m_len(0)
  , m_start(0)
  , m_end(0)
{}
     
FileUpload::~FileUpload()
{}

std::string FileUpload::name() const
{
  return m_name;
}

std::string FileUpload::filename() const
{
  return m_filename;
}

std::string FileUpload::type() const
{
  return m_type;
}

const char* FileUpload::data() const 
{
  return m_buf;
}

int FileUpload::size() const
{
  return m_end - m_start;
}

std::string FileUpload::extension() const
{
  return m_extension;
}

void FileUpload::parse(const char* buf, int len)
{
  // buf is the m_body of HttpTask
  m_buf = buf; 
  m_len = len;

  const char* s = buf;
  const char* e = buf + len - 1;
  const char* i = s;

  // parse from the field value "boundary"
  while ((i < e) && (strchr("\r\n", *i) == nullptr))
    ++ i;
  // boundary only have 4 '-'
  m_boundary = std::string(s, 2, i - s - 2);
  
  // move to next line
  while ((i < e) && (strchr("\r\n", *i) != nullptr)) 
    ++ i;
  s = i;

  // parse Form's Header Field
  while (i < e)
  {
    // header field name
    while ((i < e) && (strchr(": ", *i) == nullptr))
      ++ i;
    std::string name = std::string(s, 0, i - s);

    // skip ": "
    while ((i < e) && (strchr(": ", *i) != nullptr))
      ++ i;
    s = i;

    // header field value
    while ((i < e) && (strchr("\r\n", *i) == nullptr))
      ++ i;
    std::string value = std::string(s, 0, i - s);
    m_headers[name] = value;

    if (name == "Content-Disposition")
    {
      // name attribute of form
      std::size_t pos = value.find("name=");
      std::size_t start = pos + 6; 
      std::size_t end = value.find("\"", start);
      m_name = value.substr(start, end - start);

      // file name of upload file
      pos = value.find("filename=");
      start = pos + 10;
      end = value.find("\"", start);
      m_filename = value.substr(start, end - start);

      // file extension of upload file
      std::size_t ext = m_filename.find(".");
      m_extension = m_filename.substr(ext);
    }
    else if (name == "Content-Type")
    {
      m_type = value;
    }

    if (strncmp(i, "\r\n\r\n", 4) == 0)
    {
      i += 4;
      break;
    }

    // move to next line
    while ((i < e) && (strchr("\r\n", *i) != nullptr))
      ++ i;
    s = i;
  }

  // the beginning and end position of file binary content
  m_start = i - m_buf;
  std::string str = "--" + m_boundary + "--";
  const char* t = e - str.size();
  m_end = t - m_buf;
}

bool FileUpload::save(const std::string& filename)
{
  File file(filename);
  if(!file.create())
  {
    log_error("create file error: %s", filename.c_str());
    return false;
  }

  std::ofstream ofs(filename);
  if (ofs.fail())
  {
    log_error("open file failed: %s", filename.c_str());
    return false;
  }

  // write the uploaded file to disk
  ofs.write(m_buf + m_start, m_end - m_start);
  ofs.flush();
  return true;
}
