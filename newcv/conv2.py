import requests
import json
r = requests.post(
  "https://api.haystack.ai/api/image/analyze?output=json&apikey=ab7c08cb1558e6e24b8462e5b2b4ecbf",
  data=open('uglyman3.png', 'rb'))
#print (r.text)
j = json.loads(r.text)
print(j);