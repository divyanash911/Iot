import requests,requests
from flask import Flask,jsonify
url = "http://127.0.0.1:5089/~/in-cse/in-name/User-Patterns/What_Room_Is_Grandma_In"
from flask_cors import CORS
app = Flask(__name__)


CORS(app)

payload = {}
headers = {
  'X-M2M-Origin': 'guest:guest',
  'Accept': 'application/json'
}



response = requests.request("GET", url, headers=headers, data=payload)
print(response.text.replace(" ","").replace("\r\n","").split(":"))
# print("-------------------")


@app.route("/endpoint")
def post_data():
    response = requests.request("GET", url, headers=headers, data=payload)
    for i in response.text.replace(" ","").replace("\r\n","").split(":"):
      if 'lt' in i:
          ind= response.text.replace(" ","").replace("\r\n","").split(":").index(i)
          new= response.text.replace(" ","").replace("\r\n","").split(":")[ind+1]
          new.split(",")
          jsondict={"data":new[0]}
          print(jsondict)
    return jsonify(jsondict)

if __name__ == '__main__':
    app.run(debug=True,port=8000)

