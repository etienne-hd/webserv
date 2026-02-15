import time
import os

def send_cgi(content: str, headers: dict = {}) -> None:
	response = ""
	for key, value in headers.items():
		response += f"{key}: {value}\n"
	response += "\n"
	response += content
	print(response, end="")

def clicker(count: int) -> tuple[str, dict]:
	headers = {
		"Content-Type": "text/html; charset=UTF-8",
	}

	content = f"""
	<html>
		<body>
			<p>Click count {count}</p>

			<form method="get" action="click.py">
				<button type="submit">CLICK</button>
			</form>
			<form method="get" action="reset.py">
				<button type="submit">RESET</button>
			</form>

		</body>
	</html>
	"""

	return content, headers

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
		content, headers = clicker(0)
	else:
		try:
			content, headers = clicker(int(cookies["click_count"]))
		except:
			content, headers = clicker(0)

	send_cgi(
		content=content,
		headers=headers
	)

if __name__ == "__main__":
	main()