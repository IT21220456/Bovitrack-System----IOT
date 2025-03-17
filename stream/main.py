from flask import Flask, Response, request
import cv2
import joblib
import numpy as np
import time
from sklearn.calibration import LabelEncoder
import speedtest
from firebase_admin import credentials, db
import firebase_admin

app = Flask(__name__)

cred = credentials.Certificate("./bovi-track-firebase-adminsdk-y91yk-922ddd2559.json")
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://bovi-track-default-rtdb.firebaseio.com/'
})

resolution_model = joblib.load('./models/resolution/Resolution_model.pkl')
resolution_label_encoder = joblib.load('./models/resolution/label_encoder_resolution.pkl')
fps_model = joblib.load('./models/fps/fps_model.pkl')
fps_label_encoder = joblib.load('./models/fps/label_encoder_fps.pkl')
buffering_rate_model = joblib.load('./models/buffering_rate/buffering_Rate_model.pkl')
# label_encoder = LabelEncoder()
# video_quality_mapping = ['SD', 'HD', 'Ultra HD']
# label_encoder.fit(video_quality_mapping)

st = speedtest.Speedtest()

connection_speed = 1
fps= 60
buffering_rate = 0.00001
video_resolution=480

# Global variable to control the stream
isStop = False

# Default values
DEFAULT_FPS = 60
DEFAULT_BUFFERING_RATE = 0.00001
DEFAULT_VIDEO_RESOLUTION = 360

# Define resolution mappings
resolutions = {
    360: (480, 360),
    720: (1280, 720),
    1080: (1920, 1080),
}

ref = db.reference('function2')
    
def predict_fps(connection_speed, buffering_rate, resolution):
    print("Predicting FPS")
    resolution_encoded = resolution_label_encoder.transform([resolution])[0]
    input_data = np.array([[connection_speed, buffering_rate, resolution_encoded]])
    prediction = fps_model.predict(input_data)  
    predicted_fps = fps_label_encoder.inverse_transform(prediction)[0]
    return predicted_fps

def predict_buffering_rate(connection_speed, fps, resolution):
    print("Predicting Buffering Rate")
    resolution_encoded = resolution_label_encoder.transform([resolution])[0]
    fps_encoded = fps_label_encoder.transform([fps])[0] 
    input_data = np.array([[connection_speed, resolution_encoded, fps_encoded]]) 
    predicted_buffering = buffering_rate_model.predict(input_data)[0]
    return predicted_buffering

def predict_resolutions(connection_speed, fps, buffering_rate):
    print("Predicting Resolution")
    input_data = np.array([[connection_speed, buffering_rate, fps]])
    prediction = resolution_model.predict(input_data)
    predicted_resolution = resolution_label_encoder.inverse_transform(prediction)[0]
    return predicted_resolution

def generate_frames():
    # Initialize the webcam
    # camera = cv2.VideoCapture(0)  # Use 0 for the default webcam
    global is_stop
    # camera = cv2.VideoCapture(0)  # Use 0 for the default webcam
    camera = cv2.VideoCapture(0, cv2.CAP_MSMF)
    if not camera.isOpened():
        raise RuntimeError("Could not open webcam.")
    
    # Set the webcam resolution dynamically
    width, height = resolutions[video_resolution]
    camera.set(cv2.CAP_PROP_FRAME_WIDTH, width)
    camera.set(cv2.CAP_PROP_FRAME_HEIGHT, height)
    while True:
        # Capture frame-by-frame
        success, frame = camera.read()
        if not success:
            break
        else:
            # # Adjust frame quality based on the quality setting
            # if quality == 'Ultra_HD':
            #     # Ultra HD (highest quality)
            #     encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 100]
            # elif quality == 'HD':
            #     # HD (moderate quality)
            #     encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 90]
            # else:  # Default to 'SD'
            #     # SD (lower quality)
            #     encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 50]
            encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 50]

            # Encode frame as JPEG with the selected quality
            _, buffer = cv2.imencode('.jpg', frame, encode_param)
            frame = buffer.tobytes()

            # Yield frame in byte format
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

            # Simulate buffering (optional)
            time.sleep(buffering_rate)
            
        if isStop:
            camera.release()
            # Yield an empty frame or wait
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + b'\r\n')
            break 

        # Apply FPS control by adjusting the delay between frames
        time.sleep(1 / fps)

@app.route('/video')
def video_feed():
    global isStop
    isStop = True
    global fps, buffering_rate, video_resolution
    
    # Get parameters from request
    fps = request.args.get('fps', type=int)
    buffering_rate = request.args.get('buffering_rate', type=float)
    video_resolution = request.args.get('video_resolution', type=int)
    # connection_speed = st.download() / 1_000_000
    connection_speed = st.upload() / 1_000_000

    if fps is not None and buffering_rate is not None and video_resolution is not None:
        fps = fps
        buffering_rate = buffering_rate
        video_resolution = video_resolution
        
    elif fps is not None and buffering_rate is not None:
        video_resolution = predict_resolutions(connection_speed, fps, buffering_rate)
        
    elif fps is not None and video_resolution is not None:
        buffering_rate = predict_buffering_rate(connection_speed, fps, video_resolution)
        
    elif buffering_rate is not None and video_resolution is not None:
        fps = predict_fps(connection_speed, buffering_rate, video_resolution)
        
    elif fps is not None:
        fps = fps
        buffering_rate = DEFAULT_BUFFERING_RATE
        video_resolution = DEFAULT_VIDEO_RESOLUTION
        
    elif buffering_rate is not None:
        fps = DEFAULT_FPS
        buffering_rate = buffering_rate
        video_resolution = DEFAULT_VIDEO_RESOLUTION
        
    elif video_resolution is not None:
        fps = DEFAULT_FPS
        buffering_rate = DEFAULT_BUFFERING_RATE
        video_resolution = video_resolution
        
    else:
        fps = DEFAULT_FPS
        buffering_rate = DEFAULT_BUFFERING_RATE
        video_resolution = DEFAULT_VIDEO_RESOLUTION
        
    print(f"FPS: {fps}, Buffering Rate: {buffering_rate}, Resolution: {video_resolution}, Connection Speed: {connection_speed}")
    ref.set({"fps": fps, "buffering_rate": buffering_rate, "resolution": video_resolution, "connection_speed": connection_speed})
    isStop = False
    # Return the streaming response
    return Response(generate_frames(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/start_stream')
def start_stream():
    global isStop
    isStop = False
    return "Stream started"

@app.route('/stop_stream')
def stop_stream():
    global isStop
    isStop = True
    return "Stream stopped"

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=5000, debug=True)