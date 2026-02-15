import time
import os

def send_cgi(content: str, headers: dict = {}) -> None:
	response = ""
	for key, value in headers.items():
		response += f"{key}: {value}\n"
	response += "\n"
	response += content
	print(response, end="")

def main() -> None:
	headers = {
		"Location": "/",
		"Set-Cookie": "username=deleted; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT"
	}

	send_cgi(
		content="",
		headers=headers
	)

if __name__ == "__main__":
	main()