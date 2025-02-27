#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

namespace melon
{
  namespace fs
  {
    class File
    {
      public:
        File() = delete;
        File(const std::string& path);
        ~File() = default;

        std::string getPath() const;
        std::string getDir() const;
        bool create();
        bool remove();
        bool copy(const std::string& path);
        bool rename(const std::string& path);
        bool exists() const;
        void clear();
        int line() const;
        long size() const;
        time_t time() const;
        std::string read() const;
        bool write(const std::string& data) const;

      private:
        std::string m_path;
    };
  }
}
