import cv2
import base64
import asyncio
import websockets

async def stream(websocket):  # ← path は不要
    cap = cv2.VideoCapture(0)
    try:
        while True:
            ret, frame = cap.read()
            if not ret:
                break
            _, buffer = cv2.imencode('.jpg', frame)
            jpg_as_text = base64.b64encode(buffer).decode('utf-8')
            await websocket.send(jpg_as_text)
            await asyncio.sleep(1 / 30)  # 30fps相当で送信
    finally:
        cap.release()

async def main():
    server = await websockets.serve(
        handler=stream,
        host="0.0.0.0",
        port=8080,
    )
    print("WebSocket server started on ws://0.0.0.0:8080")
    await server.wait_closed()

asyncio.run(main())
