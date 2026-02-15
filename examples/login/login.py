import time
import os

def send_cgi(content: str, headers: dict = {}) -> None:
	response = ""
	for key, value in headers.items():
		response += f"{key}: {value}\n"
	response += "\n"
	response += content
	print(response, end="")

def not_authenticated() -> tuple[str, dict]:
	headers = {
		"Content-Type": "text/html; charset=UTF-8"
	}

	content = """
	<html>
		<body>
			<form>
				<label for="username">username:</label><br>
				<input type="text" id="username" name="username"><br>
				<input type="submit" value="Submit">
			</form>
		</body>
	</html>
	"""

	return content, headers

def invalid_username() -> tuple[str, dict]:
	headers = {
		"Content-Type": "text/html; charset=UTF-8"
	}

	content = """
	<html>
		<body>
			<a>Error, please provide a valid username!</a>
			<form>
				<label for="username">username:</label><br>
				<input type="text" id="username" name="username"><br>
				<input type="submit" value="Submit">
			</form>
		</body>
	</html>
	"""

	return content, headers

def login(username: str) -> tuple[str, dict]:
	headers = {
		"Content-Type": "text/html; charset=UTF-8",
		"Set-Cookie": f"username={username}"
	}

	content = already_logged(username)[0]

	return content, headers

def already_logged(username: str) -> tuple[str, dict]:
	headers = {
		"Content-Type": "text/html; charset=UTF-8",
	}

	content = f"""
	<html>
		<body>
			<p>Logged as {username}!</p>

			<form method="get" action="logout.py">
				<button type="submit">Logout</button>
			</form>

		</body>
	</html>
	"""

	return content, headers

def get_parameters() -> dict:
	parameters_raw = os.getenv("QUERY_STRING")
	parameters = {}
	for parameter in parameters_raw.split("&"):
		if "=" in parameter:
			parameters[parameter.split("=")[0]] = parameter.split("=")[1]
		else:
			parameters[parameter] = None
	return parameters

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
	parameters = get_parameters()
	
	if "username" in cookies:
		content, headers = already_logged(cookies["username"])
	elif "username" in parameters:
		if (parameters["username"].strip() == ""):
			content, headers = invalid_username()
		else:
			content, headers = login(parameters["username"])
	else:
		content, headers = not_authenticated()
	send_cgi(
		content=content,
		headers=headers
	)

if __name__ == "__main__":
	main()