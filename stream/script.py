import time
import subprocess
from pyngrok import ngrok
from firebase_admin import credentials, db
import firebase_admin

cred = credentials.Certificate("./bovi-track-firebase-adminsdk-y91yk-922ddd2559.json")
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://bovi-track-default-rtdb.firebaseio.com/'
})

ref = db.reference('stream')

def start_ngrok_flask():
    while True:  # Infinite loop to restart if an error occurs
        try:
            print("Starting ngrok...")
            public_url = ngrok.connect(addr="5000", proto="http", bind_tls=True)
            print(f"Ngrok URL: {public_url}")
            ref.set({"url": public_url.public_url})

            print("Starting Flask server...")
            flask_process = subprocess.Popen(["python", "main.py"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            
            stdout, stderr = flask_process.communicate()
            if stdout:
                print("Flask stdout:", stdout.decode())
            if stderr:
                print("Flask stderr:", stderr.decode())

            while True:
                time.sleep(3600)  # Keep running and update Firebase every hour

        except Exception as e:
            print(f"Error occurred: {e}. Restarting in 10 seconds...")
            time.sleep(10)  # Wait before restarting
        except KeyboardInterrupt:
            print("Stopping Flask and ngrok...")
            flask_process.terminate()
            ngrok.kill()
            break  # Exit loop if user manually stops the program

if __name__ == "__main__":
    start_ngrok_flask()