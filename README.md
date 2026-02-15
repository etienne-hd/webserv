*This project has been created as part of the 42 curriculum by ehode, ncorrear*

# Webserv

## Description

**Webserv** is a custom HTTP/1.1 web server written in C++ as part of the 42 curriculum.  
The goal of this project is to understand how a real web server works internally by reimplementing its core features from scratch (similar in spirit to <entity type="organization">NGINX</entity> or <entity type="organization">Apache HTTP Server</entity>).

This project focuses on:

- Low-level network programming (sockets, bind, listen, accept)
- Non-blocking I/O and multiplexing with select
- HTTP request parsing and response generation
- CGI execution
- File handling and upload management

The server is fully configurable via a JSON configuration file and supports multiple advanced HTTP features.

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

---

## Configuration Example

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
        "error_code": 429,
        "uri": "/error/429.html"
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

## Instructions

### Compilation

```bash
make
```

This will generate the executable:

```bash
./webserv
```

### Execution

```bash
./webserv config.json
```

### Accessing the Server

Once running:

```
http://localhost:8080
```

You can test using:

* Browser
* `curl`
* `telnet`
* `ab`

## Architecture Overview

The server is based on:

* Non-blocking sockets (`select`)
* Event-driven architecture
* One main loop handling:

  * Incoming connections
  * Request parsing
  * Response building
  * CGI execution
  * Keep-alive lifecycle

---

## Supported HTTP Methods

* `GET`
* `POST`
* `DELETE`

## Resources

### HTTP & Networking

* RFC 7230 – Hypertext Transfer Protocol (HTTP/1.1)
* <entity type="organization">Mozilla Developer Network</entity> (MDN) – HTTP documentation
  [https://developer.mozilla.org/en-US/docs/Web/HTTP](https://developer.mozilla.org/en-US/docs/Web/HTTP)
* Beej’s Guide to Network Programming
  [https://beej.us/guide/bgnet/](https://beej.us/guide/bgnet/)

### Web Servers Reference

* <entity type="organization">NGINX</entity> documentation
  [https://nginx.org/en/docs/](https://nginx.org/en/docs/)
* <entity type="organization">Apache HTTP Server</entity> documentation
  [https://httpd.apache.org/docs/](https://httpd.apache.org/docs/)

### CGI
* CGI specification (RFC 3875)

---

### AI

We used it just to make this README.