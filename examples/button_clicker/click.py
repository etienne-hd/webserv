import time
import os

def send_cgi(content: str, headers: dict = {}) -> None:
	response = ""
	for key, value in headers.items():
		response += f"{key}: {value}\n"
	response += "\n"
	response += content
	print(response, end="")


def click(count: int) -> tuple[str, dict]:
	headers = {
		"Location": "/",
		"Set-Cookie": f"click_count={count}"
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
	cookies = get_cookies()
	
	if not "click_count" in cookies:
		content, headers = click(0)
	else:
		try:
			content, headers = click(int(cookies["click_count"]) + 1)
		except:
			content, headers = click(0)

	send_cgi(
		content=content,
		headers=headers
	)

if __name__ == "__main__":
	main()