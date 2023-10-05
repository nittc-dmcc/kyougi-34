import requests


import json

post_url='http://localhost:3000/matches/10?token=1'

post_dict = {
  "turn":3,
  "actions":[
    {"type":1,"dir":2},
    {"type":1,"dir":4}
    ]
  }

post_data = json.dumps(post_dict).encode()

post_file = open("post.json","w")

json.dump(post_dict,post_file,indent=2)



r = requests.post(url=post_url, headers=header, data=post_data)
print(r.status_code)
print(r.text)

post_file.close()