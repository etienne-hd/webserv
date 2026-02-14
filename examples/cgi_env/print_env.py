import os
import json

def send_cgi(content: str, headers: dict = {}) -> None:
	response = ""
	for key, value in headers.items():
		response += f"{key}: {value}\n"
	response += "\n"
	response += content
	print(response, end="")

def main() -> None:
	env = {}
	for key, value in os.environ.items():
		if not key in ["LC_CTYPE"]:
			env[key] = value

	send_cgi(
		content=json.dumps(env),
		headers={
			"Status": 200,
			"Content-Type": "application/json"
		}
	)

if __name__ == "__main__":
	main()