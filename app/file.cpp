#include "app/file.h"
using namespace melon::app;

#include "web/file_upload.h"
using namespace melon::web;

CONTROLLER(File);

ACTION(File, index);
void File::index(const Request& req, Response& resp)
{
  resp.render("file/index.html");
}

ACTION(File, upload);
void File::upload(const Request& req, Response& resp)
{
  // handle file upload request 
  FileUpload file = req.file("file");

  // specify the uploaded file save path
  const std::string& filename = "./upload/" + file.filename();
  file.save(filename);

  // generate upload success info for client
  Json json;
  json["code"] = 0;
  json["data"] = "upload success";
  resp.json(json.toString());
}
