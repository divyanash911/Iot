import requests,jsonify,requests
from flask import Flask
url = "http://127.0.0.1:5089/~/in-cse/in-name/AE-TEST/Node-1/Data/la"
from flask_cors import CORS
app = Flask(__name__)


CORS(app)

payload = {}
headers = {
  'X-M2M-Origin': 'guest:guest',
  'Accept': 'application/json'
}

response = requests.request("GET", url, headers=headers, data=payload)


print(response.text)

@app.route("/endpoint")
def post_data():
    return str(response.text)

if __name__ == '__main__':
    app.run(debug=True,port=8000)
