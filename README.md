# web-frame

This is a high-performance C++ web framework designed to handle HTTP requests efficiently. The framework consists of two main modules: epoll-based reactor network model to manage client-server communication. And web server module parses and responds to HTTP requests, supporting GET and POST methods, automatic URL routing, JSON format, static resource access, and file upload.

## 🚀 Features

- **HTTP Request Parsing**: Supports parsing GET and POST HTTP requests, including handling parameters, headers, cookies, and files.
- **Routing**: Automatically routes incoming HTTP requests based on the requested URL path.
- **Static File Handling**: Supports serving static files like images, CSS, and JS.
- **File Upload**: Supports parsing and saving file uploads.
- **JSON Support**: Can parse and respond with JSON data.
- **High-Performance**: Uses an epoll-based reactor model for handling multiple client connections efficiently.

## 📂 Core Components

### Server

The server listens for incoming HTTP requests and binds routes to appropriate handlers.

- `start()`: Starts the server and begins accepting client requests.
- `bind(const std: :string& path, server_handler handler)`: Binds a handler function to a specific URI path for handling requests.
- `handle(const Request& req)` : Processes an incoming request and returns the corresponding response.

### Request

Represents an HTTP request, with methods for parsing headers, body, and parameters.

- `parseHeader(const char* buf, int len)`: Parses the header of the HTTP request from a buffer.
- `parseBody(const char* buf, int len)`: Parses the body of the HTTP request from a buffer.

### Response

Represents an HTTP response and allows setting response data such as HTML, JSON, or static files.

- `data(Type type, const std::string& data)`: Sets the response data with the specified type (HTML, JSON, etc.).
- `pageNotFound()`: Returns a default “Page Not Found” message.
- `render(const std::string& filename)`: Renders an HTML template from a file.

### FileUpload

Handles file uploads and provides methods to retrieve file data, save it to disk, and extract file information.

- `parse(const char* buf, int len)`: Parses the file from the request body.
- `save(const std::string& filename)`: Saves the uploaded file to the specified path.

## 🖥️ Getting Started

1. Clone the Repository

```sh
git clone https://github.com/jiafie7/web-frame.git
cd web-frame
```

2. Build the Project

```sh
mkdir build
cd build
cmake ..
make

# start server
./main

# access through server IP address and port
<IP>:<port>/
```

## 📝 License

This project is licensed under the MIT License. See the LICENSE file for more information.

## 🤝 Contributing

Contributions, bug reports, and feature requests are welcome. Feel free to fork the repository, open issues, or submit pull requests.
