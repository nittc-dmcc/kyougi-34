import requests

import json
import time


start = time.time()

get_url='http://localhost:3000/matches/10?token=1'

header={"Content-Type":"application/json"}

response=requests.get(get_url,{'key':'value'},)


file = open("./hozon_loop.json","wb")

for chunk in response.iter_content(100000):
  file.write(chunk)

file.close()
