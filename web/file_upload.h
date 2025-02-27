#pragma once

#include <cstring>
#include <fstream>
#include <map>
#include <string>

#include "fs/file.h"
using namespace melon::fs;

#include "utility/log_system.h"
using namespace melon::utility;

namespace melon
{
  namespace web
  {
    class FileUpload
    {
    public:
      FileUpload();
      ~FileUpload();

      // the name attribute of html form
      std::string name() const;
      // the filename of upload file
      std::string filename() const;
      // the type of upload file
      std::string type() const;
      // the binary content of upload file
      const char* data() const; 
      // the size of upload file 
      int size() const;
      // the extension of upload file
      std::string extension() const;

      // parse request body for the file upload request
      void parse(const char* buf, int len);
      // the save path of upload file
      bool save(const std::string& filename);

    private:
      std::string m_name;
      std::string m_filename;
      std::string m_type;
      std::string m_extension;
      std::string m_boundary;
      std::map<std::string, std::string> m_headers;
      const char* m_buf;
      int m_len;
      
      // the beginning and end positions of the actual file binary content in the request body
      int m_start;
      int m_end;
    };
  }
}
