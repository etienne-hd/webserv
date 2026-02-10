# webserv
a static HTTP server

## Todo
- [ ] Path resolver
- [ ] CGI
- [ ] Content-Type in headers
- [ ] Working HTTP Error (404, 413, 429)
- [ ] Keep Alive Handler
- [ ] Check Allowed Method
- [ ] Redirection
- [ ] File upload
- [ ] Directory Listing
- [ ] Default error page

## Configuration
```json
[
	{
		"name": "my awesome website",
		"keepalive_timeout": 30,
		"max_request_size": 65536,
		"listen": "0.0.0.0:8080",
		"locations": [
			{
				"uri": "/",
				"path": "./path/to/location"
			},
			{
				"uri": "/error",
				"path": "./path/to/location/error"
			}
		],
		"document_index": "index.html",
		"error_pages": [
			{
				"error_code": 404,
				"path": "/error/404.html"
			},
			{
				"error_code": 429,
				"path": "/error/429.html"
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
		"file_upload_directory": "./files",
		"cgi_enabled": true,
		"cgi_rules": [
			{
				"extension": ".py",
				"path": "/bin/python"
			}
		]
	}
]
```