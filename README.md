# webserv
a static HTTP server

## Todo
- [x] Path resolver
- [x] CGI
- [x] Content-Type in headers
- [x] Working HTTP Error (400, 404, 413, 429)
- [x] Keep Alive Handler
- [x] Check Allowed Method
- [x] Redirection
- [x] File upload
- [x] File delete
- [x] Directory Listing
- [x] Default error page
- [x] Better Read
- [x] Request init parameters map
- [ ] Show port on << client overload
- [x] Check uri (.., / at begin)
- [ ] Close read pipe / kill (CGI) on ctrl+c

## Known issue
- [ ] Leak when socket initialization fail
- [ ] Other method (PATCH, PUT) failed

## Configuration
```json
[
	{
		"name": "my awesome website",
		"keepalive_timeout": 30,
		"max_request_size": 8080,
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
		],
		"cgi_timeout": 15 
	}
]
```

## CGI
- https://www.rfc-editor.org/rfc/rfc3875.html