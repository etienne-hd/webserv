# webserv [![wakatime](https://wakatime.com/badge/user/b5c6762d-cdfb-4336-9c84-87845f0e1976/project/aa5b5550-0e22-4976-a033-b70e71c28b60.svg)](https://wakatime.com/badge/user/b5c6762d-cdfb-4336-9c84-87845f0e1976/project/aa5b5550-0e22-4976-a033-b70e71c28b60)

*webserv is a 42 project: You must recreate a static web server (like nginx).*

- **125/100** ✅

![Illustration](/images/demo.png)

## Features

- HTTP/1.1 support
- Keep-Alive connection handling
- Redirections (3xx)
- Static file serving
- Custom error pages
- Directory listing
- File upload
- File deletion
- CGI execution (e.g. Python scripts)
- Configurable max body size
- CGI timeout handling
- Logger (DEBUG, INFO, WARNING, ERROR, CRITICAL)
- File logging

## Configuration
Below is a working configuration example:

```json
[
  {
    "name": "my awesome website",
    "keepalive_timeout": 30,
    "max_body_size": 8080,
    "listen": "0.0.0.0:8080",
    "locations": [
      {
        "uri": "/",
        "path": "/path/to/location"
      },
      {
        "uri": "/error",
        "path": "/path/to/location/error"
      }
    ],
    "document_index": "/index.html",
    "error_pages": [
      {
        "error_code": 404,
        "uri": "/error/404.html"
      },
      {
        "error_code": 500,
        "uri": "/error/500.html"
      }
    ],
    "allowed_methods": ["GET", "POST"],
    "redirections": [
      {
        "uri": "/github",
        "redirect": "https://github.com/etienne-hd/"
      }
    ],
    "directory_listing_enabled": true,
    "file_on_directory": "/file_on_directory.html",
    "file_upload_enabled": true,
    "file_upload_directory": "/files",
    "cgi_enabled": true,
    "cgi_rules": [
      {
        "extension": ".py",
        "path": "/bin/python"
      }
    ],
    "cgi_timeout": 15
  }
]
```

![42](https://img.shields.io/badge/-42-black?style=for-the-badge&logo=42&logoColor=white)