import cv2
import asyncio
import websockets

async def stream(websocket):
    cap = cv2.VideoCapture(0)
    try:
        while True:
            ret, frame = cap.read()
            if not ret:
                break
            # JPEGエンコード（バイナリ形式）
            _, buffer = cv2.imencode('.jpg', frame)
            await websocket.send(buffer.tobytes())  # バイナリ送信
    finally:
        cap.release()

async def main():
    print("WebSocket server started on ws://0.0.0.0:8080")
    async with websockets.serve(stream, "0.0.0.0", 8080):
        await asyncio.Future()  # 永久に待機

asyncio.run(main())
