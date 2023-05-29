import requests,requests,json
from flask import Flask,jsonify
url = "http://127.0.0.1:5089/~/in-cse/in-name/User-Patterns/What_Room_Is_Grandma_In/la"
from flask_cors import CORS
app = Flask(__name__)


CORS(app)

payload = {}
headers = {
  'X-M2M-Origin': 'guest:guest',
  'Accept': 'application/json'
}



response = requests.request("GET", url, headers=headers, data=payload)
print(json.loads(response.text))
# print("-------------------")


@app.route("/endpoint")
def post_data():
    response = requests.request("GET", url, headers=headers, data=payload)
    dict=json.loads(response.text)
    print(dict["m2m:cin"]["con"][1])
    jsondict={"data":dict["m2m:cin"]["con"][1]}
    return jsonify(jsondict)

if __name__ == '__main__':
    app.run(debug=True,port=8000)
