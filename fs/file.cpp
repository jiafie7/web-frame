#include "fs/file.h"
#include "fs/directory.h"

using namespace melon::fs;

File::File(const std::string& path)
{
  m_path = Directory::normalizePath(path);
}
       
std::string File::getPath() const
{
  return m_path;
}

std::string File::getDir() const
{
  if (m_path.empty())
    return "";
  char seperator = Directory::seperator();
  std::size_t pos = m_path.find_last_of(seperator);
  if (pos == std::string::npos)
    return "";
  return m_path.substr(0, pos);
}

bool File::create()
{
  if (exists())
    return false;
  
  Directory dir(getDir());
  if (!dir.exists())
    dir.create();

  std::ofstream ofs(m_path);
  return ofs.is_open();
}

bool File::remove()
{
  return unlink(m_path.c_str()) == 0;
}

bool File::copy(const std::string& path)
{
  File file_dist(path);
  if (!file_dist.exists())
    file_dist.create();

  std::ifstream ifs(m_path);
  if (ifs.fail())
    return false;
  
  std::ofstream ofs(path);
  if (ofs.fail())
    return false;
  
  ofs << ifs.rdbuf();
  ofs.flush();
  ofs.close();
  return true;
}

bool File::rename(const std::string& path)
{
  int ret = std::rename(m_path.c_str(), path.c_str());
  if (ret != 0)
    return false;
  m_path = path;
  return true;
}

bool File::exists() const
{
  std::ifstream ifs(m_path);
  return ifs.good();
}

void File::clear()
{
  std::ofstream ofs(m_path, std::ios::out);
  ofs.flush();
  ofs.close();
}

int File::line() const
{
  std::ifstream ifs(m_path);
  int line = 0;
  std::string data;
  while (std::getline(ifs, data))
    ++ line;
  return line;
}

long File::size() const
{
  std::ifstream ifs(m_path);
  ifs.seekg(0, std::ios_base::end);
  return (long)ifs.tellg();
}

time_t File::time() const
{
  struct stat info = {0};
  if (stat(m_path.c_str(), &info) != 0)
  {
    std::cerr << "Stat file error: " << m_path << '\n';
    return 0;
  }
  return info.st_mtime;
}

std::string File::read() const
{
  std::ifstream ifs(m_path);
  std::ostringstream oss;
  oss << ifs.rdbuf();
  return oss.str();
}

bool File::write(const std::string& data) const
{
  std::ofstream ofs(m_path);
  if (ofs.fail())
    return false;
  ofs.write(data.c_str(), data.size());
  return true;
}


