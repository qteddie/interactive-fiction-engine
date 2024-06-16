import json

json_str = '{"text":"2e�!","next":"dragon_encounter"}'
try:
    data = json.loads(json_str)
    print("Valid JSON:", data)
except json.JSONDecodeError as e:
    print("Invalid JSON:", e)