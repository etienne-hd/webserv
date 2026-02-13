import time

def send_cgi(content: str, headers: dict = {}) -> None:
	response = ""
	for key, value in headers.items():
		response += f"{key}: {value}\n"
	response += "\n"
	response += content
	print(response, end="")

def main() -> None:
	send_cgi(
		content=f"<html><body><h2>Current Timestamp: {time.time()}</h2></body></html>",
		headers={
			"Status": 200,
			"Content-Type": "text/html"
		}
	)

if __name__ == "__main__":
	main()