#include "fs/directory.h"

using namespace melon::fs;

Directory::Directory()
{
  m_path = Directory::normalizePath(".");
}

Directory::Directory(const std::string& path)
{
  m_path = Directory::normalizePath(path);
}

std::string Directory::getPath() const
{
  return m_path;
}

char Directory::seperator()
{
#ifdef WIN32
  return '\\';
#else
  return '/';
#endif
}

bool Directory::exists() const
{
  if (m_path.empty())
    return false;

  struct stat info = {0};
  if (stat(m_path.c_str(), &info) != 0)
    return false;
  
  if (info.st_mode & S_IFDIR)
    return true;
  return false;
}

bool Directory::create() const
{
  char seperator = Directory::seperator();
  std::vector<std::string> path_list = String::split(m_path, seperator);
  
  std::string dir_path;
  for (const std::string& path : path_list)
  {
    if (path.empty())
      continue;
    if (dir_path.empty())
      dir_path += path;
    else
      dir_path += seperator + path;
      
    dir_path = Directory::adjustPath(dir_path);

    Directory dir(dir_path);
    if (dir.exists())
      continue;

    if (mkdir(dir_path.c_str()) != 0)
      return false;
  }
  return true;
}

// delete current directory
bool Directory::remove() const
{
  DIR* dir = opendir(m_path.c_str());
  if (dir == nullptr)
    return false;
  
  char seperator = Directory::seperator();
  struct dirent* entry;
  while ((entry = readdir(dir)) != nullptr)
  {
    std::string name = entry->d_name;
    if (name == "." || name == "..")
      continue;
    std::string fullname = m_path + seperator + name;
    
    struct stat info = {0};
    if (stat(fullname.c_str(), &info) != 0)
    {
      std::cerr << "stat file error: " << fullname << '\n';
      return false;
    }
  
    // Recursively delete subdirectories
    if (S_ISDIR(info.st_mode))
    {
      Directory tmp(fullname);
      tmp.remove();
    }
    else // delete file
    {
      unlink(fullname.c_str());
    }
  }
  rmdir(m_path.c_str());
  return true;
}

// delete subdirectories and files in current directory
void Directory::clear()
{
  DIR* dir = opendir(m_path.c_str());
  if (dir == nullptr)
    return;

  char seperator = Directory::seperator();
  struct dirent* entry;
  while ((entry = readdir(dir)) != nullptr)
  {
    std::string name = entry->d_name;
    if (name == "." || name == "..")
      continue;
    
    std::string fullname = m_path + seperator + name;
    struct stat info = {0};
    if (stat(fullname.c_str(), &info) != 0)
    {
      std::cerr << "stat file error: " + fullname << '\n';
      return;
    }

    if (S_ISDIR(info.st_mode))
    {
      Directory tmp(fullname);
      tmp.remove();
    }
    else
    {
      unlink(fullname.c_str());
    }
  }
}

bool Directory::rename(const std::string& path)
{
  if (std::rename(m_path.c_str(), path.c_str()) != 0 )
    return false;
  m_path = Directory::normalizePath(path);
  return true;
}

std::vector<File> Directory::file() const
{
  std::vector<File> files;
  DIR* dir = opendir(m_path.c_str());
  if (dir == nullptr)
    return files;

  char seperator = Directory::seperator();
  struct dirent* entry;
  while ((entry = readdir(dir)) != nullptr)
  {
    std::string name = entry->d_name;
    if (name == "." || name == "..")
      continue;

    std::string fullname = m_path + seperator + name;
    struct stat info = {0};
    if (stat(fullname.c_str(), &info) != 0)
    {
      std::cerr << "stat file error: " + fullname << '\n';
      return files;
    }

    if (S_ISDIR(info.st_mode))
    {
      Directory tmp(fullname);
      std::vector<File> sub_dir = tmp.file();
      files.insert(files.end(), sub_dir.begin(), sub_dir.end());
    }
    else
    {
      files.push_back(File(fullname));
    }
  }
  return files;
}

bool Directory::copy(const std::string& path)
{
  Directory dir(path);
  if (!dir.exists()) 
    if (!dir.create())
    {
      std::cout << "false";
      return false;
    }

  std::vector<File> files = file();
  for (File& file : files)
  {
    std::string file_src = file.getPath();
    std::string file_dist = dir.getPath() + file_src.substr(m_path.length());

    if (!file.copy(file_dist))
      return false;
  }

  return true;
}

int Directory::count() const
{
  std::vector<File> files = file();
  return (int)files.size();
}

int Directory::line() const
{
  int line = 0;
  std::vector<File> files = file();
  for (const File& file : files)
    line += file.line();
  return line;
}

long Directory::size() const
{
  long size = 0;
  std::vector<File> files = file();
  for (const File& file : files)
    size += file.size();
  return size;
}

bool Directory::isAbsolutePath(const std::string& path)
{
  if (path.empty())
    return false;
  char seperator = Directory::seperator();
  std::string filepath = path;

#ifdef WIN32
  std::replace(filepath.begin(), filepath.end(), '/', seperator);
#endif

  std::vector<std::string> output = String::split(filepath, seperator);

#ifdef WIN32
  if (output[0].find(':') != std::string::npos)
#else
  if (output[0].empty())
#endif
    return true;
  else
    return false;
}

std::string Directory::normalizePath(const std::string& path)
{
  char seperator = Directory::seperator();
  std::string filepath = path;

#ifdef WIN32
  std::replace(filepath.begin(), filepath.end(), '/', seperator);
#endif

  std::vector<std::string> abs_path_list;
  if (!isAbsolutePath(path))
  {
    char abs_path[PATH_MAX] = {0};
    getcwd(abs_path, PATH_MAX);
    abs_path_list = String::split(abs_path, seperator);
  }

  // generate absolute path
  for (const std::string& dir : String::split(filepath, seperator))
  {
    if (dir.empty())
      continue;
    if (dir == ".")
      continue;
    else if (dir == "..")
      abs_path_list.pop_back();
    else
      abs_path_list.push_back(dir);
  } 

  std::string temp = String::join(abs_path_list, seperator);
  return adjustPath(temp);
}
        
std::string Directory::adjustPath(const std::string& path)
{
  char seperator = Directory::seperator();
  std::string filepath = path;

#ifdef WIN32
  if (filepath[filepath.size() - 1] == ':')
    filepath += seperator;
#else
  if (filepath[0] != seperator)
    filepath = std::string(1, seperator) + filepath;
#endif

  return filepath;
}

int Directory::mkdir(const char* path)
{
#ifdef WIN32
  return ::_mkdir(path);
#else
  return ::mkdir(path, 0755);
#endif
}

int Directory::rmdir(const char* path)
{
#ifdef WIN32
  return ::_rmdir(path);
#else
  return ::rmdir(path);
#endif
}

int Directory::unlink(const char* path)
{
#ifdef WIN32
  return ::_unlink(path);
#else
  return ::unlink(path);
#endif
}

char* Directory::getcwd(char* buf, int len)
{
#ifdef WIN32
  return ::_getcwd(buf, len);
#else
  return ::getcwd(buf, len);
#endif
}

