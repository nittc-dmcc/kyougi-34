import requests

import json
import time


start = time.time()

get_url='http://localhost:3000/matches/10?token=imizuac5e182743df5c6c7d2140fe5f9e5b4d0f2e1c77a424fc9d2986c9a270f'

header={"Content-Type":"application/json"}

response=requests.get(get_url,{'key':'value'},)


file = open("./hozon_loop.json","wb")

for chunk in response.iter_content(100000):
  file.write(chunk)

file.close()
