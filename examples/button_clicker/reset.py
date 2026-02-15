import time
import os

def send_cgi(content: str, headers: dict = {}) -> None:
	response = ""
	for key, value in headers.items():
		response += f"{key}: {value}\n"
	response += "\n"
	response += content
	print(response, end="")

def reset() -> tuple[str, dict]:
	headers = {
		"Location": "/",
		"Set-Cookie": "click_count=deleted; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT"
	}

	return "", headers

def get_cookies() -> dict:
	cookies_raw = os.getenv("HTTP_COOKIE")
	cookies = {}
	if cookies_raw is None:
		return cookies
	for cookie in cookies_raw.split("; "):
		if "=" in cookie:
			cookies[cookie.split("=")[0]] = cookie.split("=")[1]
		else:
			cookies[cookie] = None
	return cookies

def main() -> None:
	content, headers = reset()

	send_cgi(
		content=content,
		headers=headers
	)

if __name__ == "__main__":
	main()